#include <phnt_windows.h>
#include <phnt.h>

#include <cstddef>
#include <algorithm>
#include <span>
#include <type_traits>
#include <string_view>
#include <map>

#include <ehdata.h>
#include <rttidata.h>

#include <SafeInt.hpp>

#include "hooks.h"
#include "pluginsdk.h"
#include "versioninfo.h" 
#include "xml/hooks.h"

#include <fnv1a.h>
#include <pe/exports.h>
#include <pe/module.h>
#include <xorstr.hpp>
#include <detours.h>

#include <ntrtl.hpp>


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  if ( fdwReason == DLL_PROCESS_ATTACH )
    DisableThreadLibraryCalls(hInstance);
  return TRUE;
}

template<class Fn, typename = std::enable_if_t<std::is_function_v<Fn>>>
inline LONG WINAPI DetourAttach(HMODULE hModule, PCSTR pProcName, Fn **ppPointer, Fn *pDetour)
{
  if ( !hModule ) return ERROR_INVALID_PARAMETER;
  if ( !ppPointer ) return ERROR_INVALID_PARAMETER;

  if ( *ppPointer = reinterpret_cast<Fn *>(GetProcAddress(hModule, pProcName)) ) {
    return DetourAttachEx(reinterpret_cast<PVOID *>(ppPointer), pDetour, nullptr, nullptr, nullptr);
  }
  return ERROR_PROC_NOT_FOUND;
}

void hook_xmlreader_from_rtti(
  PVOID ib,
  const std::span<uint8_t> &data,
  const std::span<uint8_t> &rdata,
  PVOID *ppfnReadFile,
  PVOID pfnReadFileHook,
  PVOID *ppfnReadMem,
  PVOID pfnReadMemHook)
{
  const char Name[] = ".?AVXmlReaderImpl@@";
  for ( auto Iter = data.begin();; std::advance(Iter, sizeof(Name)); ) {
    Iter = std::search(data.begin(), data.end(), std::begin(Name), std::end(Name));
    if ( Iter == data.end() )
      break;

    const auto tmp = reinterpret_cast<TypeDescriptor *>(&*std::prev(Iter, offsetof(TypeDescriptor, name)));
    if ( (reinterpret_cast<uintptr_t>(tmp) & (sizeof(uintptr_t) - 1)) == 0 ) {
#if _RTTI_RELATIVE_TYPEINFO
      const auto typeDescriptor = static_cast<int>(reinterpret_cast<uintptr_t>(tmp) - reinterpret_cast<uintptr_t>(ib));
#else
      const auto typeDescriptor = tmp;
#endif
      for ( auto Iter2 = rdata.begin();; std::advance(Iter2, sizeof(typeDescriptor)) ) {
        Iter2 = std::search(rdata.begin(), rdata.end(), reinterpret_cast<const uint8_t *>(&typeDescriptor), reinterpret_cast<const uint8_t *>(&typeDescriptor) + sizeof(typeDescriptor));
        if ( Iter2 == rdata.end() )
          break;

        const auto completeObjectLocator = reinterpret_cast<_RTTICompleteObjectLocator *>(&*std::prev(Iter2, offsetof(_RTTICompleteObjectLocator, pTypeDescriptor)));
        if ( (reinterpret_cast<uintptr_t>(completeObjectLocator) & 3) == 0 ) {
          for ( auto Iter3 = rdata.begin();; std::advance(Iter3, sizeof(completeObjectLocator)) ) {
            Iter3 = std::search(rdata.begin(), rdata.end(), reinterpret_cast<const uint8_t *>(&completeObjectLocator), reinterpret_cast<const uint8_t *>(&completeObjectLocator) + sizeof(completeObjectLocator));
            if ( Iter3 == rdata.end() )
              break;

            if ( (reinterpret_cast<uintptr_t>(&*Iter3) & (sizeof(uintptr_t) - 1)) == 0 ) {
              const auto vfptr = reinterpret_cast<void **>(&*std::next(Iter3, sizeof(_RTTICompleteObjectLocator *)));

              THROW_IF_WIN32_ERROR(DetourTransactionBegin());
              THROW_IF_WIN32_ERROR(DetourUpdateThread(NtCurrentThread()));
              *ppfnReadFile = vfptr[6];
              THROW_IF_WIN32_ERROR(DetourAttach(ppfnReadFile, pfnReadFileHook));
              *ppfnReadMem = vfptr[7];
              THROW_IF_WIN32_ERROR(DetourAttach(ppfnReadMem, pfnReadMemHook));
              THROW_IF_WIN32_ERROR(DetourTransactionCommit());
              break;
            }
          }
          break;
        }
      }
      break;
    }
  }
}

PVOID g_DllNotificationCookie;
VOID NTAPI DllNotification(
  ULONG NotificationReason,
  PLDR_DLL_NOTIFICATION_DATA NotificationData,
  PVOID Context)
{
  if ( NotificationReason != LDR_DLL_NOTIFICATION_REASON_LOADED )
    return;

  if ( static_cast<nt::rtl::unicode_string_view *>(NotificationData->Loaded.BaseDllName)->istarts_with(xorstr_(L"bsengine")) ) {
    static std::once_flag once;
    std::call_once(once, [NotificationData]() {
      const auto clsections = nt::rtl::image_sections(nullptr);
      const auto header = nt::rtl::find_image_section(clsections, nullptr, IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ);
      const std::span<uint8_t> section{nt::rtl::image_rva_to_va<uint8_t>(nullptr, header->VirtualAddress), header->Misc.VirtualSize};
      hook_xmlreader_from_rtti(NtCurrentPeb()->ImageBaseAddress, section, section, reinterpret_cast<PVOID *>(&g_pfnClientReadFile), ClientReadFile_hook, reinterpret_cast<PVOID *>(&g_pfnClientReadMem), ClientReadMem_hook);

      const auto esections = nt::rtl::image_sections(NotificationData->Loaded.DllBase);
      const auto data_header = nt::rtl::find_image_section(esections, xorstr_(".data"), IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE);
      const std::span<uint8_t> data{nt::rtl::image_rva_to_va<uint8_t>(NotificationData->Loaded.DllBase, data_header->VirtualAddress), data_header->Misc.VirtualSize};
      const auto rdata_header = nt::rtl::find_image_section(esections, xorstr_(".rdata"), IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ);
      const std::span<uint8_t> rdata{nt::rtl::image_rva_to_va<uint8_t>(NotificationData->Loaded.DllBase, data_header->VirtualAddress), data_header->Misc.VirtualSize};
      hook_xmlreader_from_rtti(NotificationData->Loaded.DllBase, data, rdata, reinterpret_cast<PVOID *>(&g_pfnEngineReadFile), EngineReadFile_hook, reinterpret_cast<PVOID *>(&g_pfnEngineReadMem), EngineReadMem_hook);
    });
  }
}

VOID __cdecl Init(VOID)
{
  PCWSTR OriginalFilename;
  if ( GetModuleVersionInfo(nullptr,
    xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"),
    reinterpret_cast<LPCVOID *>(&OriginalFilename)) < 0 )
    return;

  switch ( fnv1a::make_hash(OriginalFilename, fnv1a::ascii_toupper) ) {
    case L"Client.exe"_fnv1au:
    case L"BNSR.exe"_fnv1au:
      NtCurrentPeb()->BeingDebugged = FALSE;
      DetourTransactionBegin();
      DetourUpdateThread(NtCurrentThread());
      if ( const auto hModule = GetModuleHandleW(xorstr_(L"ntdll.dll")) ) {
        const auto pfnLdrRegisterDllNotification = reinterpret_cast<NTSTATUS(NTAPI *)(ULONG, PLDR_DLL_NOTIFICATION_FUNCTION, PVOID, PVOID *)>(
          GetProcAddress(hModule, xorstr_("LdrRegisterDllNotification")));
        THROW_IF_NTSTATUS_FAILED(pfnLdrRegisterDllNotification(0, &DllNotification, nullptr, &g_DllNotificationCookie));
#ifdef _X86_
        DetourAttach(hModule, xorstr_("LdrGetDllHandle"), &g_pfnLdrGetDllHandle, &LdrGetDllHandle_hook);
#endif
        DetourAttach(hModule, xorstr_("LdrLoadDll"), &g_pfnLdrLoadDll, &LdrLoadDll_hook);
        DetourAttach(hModule, xorstr_("NtCreateFile"), &g_pfnNtCreateFile, &NtCreateFile_hook);
        DetourAttach(hModule, xorstr_("NtCreateMutant"), &g_pfnNtCreateMutant, &NtCreateMutant_hook);
        DetourAttach(hModule, xorstr_("NtOpenKeyEx"), &g_pfnNtOpenKeyEx, &NtOpenKeyEx_hook);
        DetourAttach(hModule, xorstr_("NtProtectVirtualMemory"), &g_pfnNtProtectVirtualMemory, &NtProtectVirtualMemory_hook);
        DetourAttach(hModule, xorstr_("NtQuerySystemInformation"), &g_pfnNtQuerySystemInformation, &NtQuerySystemInformation_hook);
#ifdef _AMD64_
        DetourAttach(hModule, xorstr_("NtQueryInformationProcess"), &g_pfnNtQueryInformationProcess, &NtQueryInformationProcess_hook);
        DetourAttach(hModule, xorstr_("NtSetInformationThread"), &g_pfnNtSetInformationThread, &NtSetInformationThread_hook);
#endif
      }
      if ( const auto hModule = GetModuleHandleW(xorstr_(L"user32.dll")) )
        DetourAttach(hModule, xorstr_("FindWindowA"), &g_pfnFindWindowA, &FindWindowA_hook);
      DetourTransactionCommit();
      break;
  }
}

__declspec(dllexport)
void __cdecl GetPluginInfo(PluginInfo *plgi)
{
  plgi->Init = &Init;
}
