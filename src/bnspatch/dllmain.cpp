#include <phnt_windows.h>
#include <phnt.h>

#include <algorithm>
#include <span>
#include <type_traits>
#include <map>

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
#include <ntamd64.hpp>
#include <util.hpp>

#include <intrin.h>
#pragma intrinsic(_ReturnAddress)

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
    DisableThreadLibraryCalls(hInstance);
  return TRUE;
}

template<class Fn, typename = std::enable_if_t<std::is_function_v<Fn>>>
inline LONG WINAPI DetourAttach(HMODULE hModule, PCSTR pProcName, Fn** ppPointer, Fn* pDetour)
{
  if (!hModule) return ERROR_INVALID_PARAMETER;
  if (!ppPointer) return ERROR_INVALID_PARAMETER;

  if (*ppPointer = reinterpret_cast<Fn*>(GetProcAddress(hModule, pProcName))) {
    return DetourAttachEx(reinterpret_cast<PVOID*>(ppPointer), pDetour, nullptr, nullptr, nullptr);
  }
  return ERROR_PROC_NOT_FOUND;
}

PVOID g_DllNotificationCookie;
VOID NTAPI DllNotification(
  ULONG NotificationReason,
  PLDR_DLL_NOTIFICATION_DATA NotificationData,
  PVOID Context)
{
  if (NotificationReason != LDR_DLL_NOTIFICATION_REASON_LOADED)
    return;

  if (static_cast<nt::rtl::unicode_string_view*>(NotificationData->Loaded.BaseDllName)->istarts_with(xorstr_(L"bsengine"))) {
    static std::once_flag once;
    std::call_once(once, [NotificationData]() {
#if defined(_AMD64_)
      // 48 83 EC 38 33 C9 E8 ?? ?? ?? ?? 41 B8 25 00 00 00 48 8D 15 ?? ?? ?? ?? B9 18 00 00 00 E8 ?? ?? ?? ??
      const auto Pat = std::array<patternbyte, 34>{0x48, 0x83, 0xEC, 0x38, 0x33, 0xC9, 0xE8, {}, {}, {}, {}, 0x41, 0xB8, 0x25, 0x00, 0x00, 0x00, 0x48, 0x8D, 0x15, {}, {}, {}, {}, 0xB9, 0x18, 0x00, 0x00, 0x00, 0xE8, {}, {}, {}, {}};
#elif defined(_X86_)
      // E8 ?? ?? ?? ?? A1 ?? ?? ?? ?? 85 C0 6A 0C 74 07 FF D0 83 C4 04 EB ?? A1 ?? ?? ?? ?? 8B 0D ?? ?? ?? ?? 50 51
      const auto Pat = std::array<patternbyte, 36>{0xE8, {}, {}, {}, {}, 0xA1, {}, {}, {}, {}, 0x85, 0xC0, 0x6A, 0x0C, 0x74, 0x07, 0xFF, 0xD0, 0x83, 0xC4, 0x04, 0xEB, {}, 0xA1, {}, {}, {}, {}, 0x8B, 0x0D, {}, {}, {}, {}, 0x50, 0x51};
#endif

      for (const auto& t : std::array{
        std::tuple{NtCurrentPeb()->ImageBaseAddress, &g_pfnClientCreateXmlReader, ClientCreateXmlReader_hook},
        std::tuple{NotificationData->Loaded.DllBase, &g_pfnEngineCreateXmlReader, EngineCreateXmlReader_hook}
        }) {
        const auto NtHeaders = nt::rtl::image_nt_headers(std::get<0>(t));
        std::span NtSections{ IMAGE_FIRST_SECTION(NtHeaders), NtHeaders->FileHeader.NumberOfSections };
        const auto NtSection = std::find_if(NtSections.begin(), NtSections.end(), [](const IMAGE_SECTION_HEADER& NtSection) {
          constexpr DWORD Flags = IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_MEM_READ;
          return (NtSection.Characteristics & Flags) == Flags;
          });
        if (NtSection != NtSections.end()) {
          const std::span Haystack{ nt::rtl::image_rva_to_va<BYTE>(std::get<0>(t), NtSection->VirtualAddress), NtSection->Misc.VirtualSize };
          auto Iter = std::search(Haystack.begin(), Haystack.end(), Pat.begin(), Pat.end());
          if (Iter != Haystack.end()) {
            DetourTransactionBegin();
            DetourUpdateThread(NtCurrentThread());
            *std::get<1>(t) = reinterpret_cast<PFN_CREATEXMLREADER>(std::addressof(*Iter));
            DetourAttach(reinterpret_cast<PVOID*>(std::get<1>(t)), std::get<2>(t));
            DetourTransactionCommit();
          }
        }
      }
      });
  }
}

VOID __cdecl Init(VOID)
{
  PCWSTR OriginalFilename;
  if (GetModuleVersionInfo(nullptr,
    xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"),
    reinterpret_cast<LPCVOID*>(&OriginalFilename)) < 0)
    return;

  switch (fnv1a::make_hash(OriginalFilename, fnv1a::ascii_toupper)) {
  case L"Client.exe"_fnv1au:
  case L"BNSR.exe"_fnv1au:
    NtCurrentPeb()->BeingDebugged = FALSE;
    DetourTransactionBegin();
    DetourUpdateThread(NtCurrentThread());
    if (const auto hModule = GetModuleHandleW(xorstr_(L"ntdll.dll"))) {
      const auto pfnLdrRegisterDllNotification = reinterpret_cast<NTSTATUS(NTAPI*)(ULONG, PLDR_DLL_NOTIFICATION_FUNCTION, PVOID, PVOID*)>(
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
    if (const auto hModule = GetModuleHandleW(xorstr_(L"user32.dll")))
      DetourAttach(hModule, xorstr_("FindWindowA"), &g_pfnFindWindowA, &FindWindowA_hook);
    DetourTransactionCommit();
    break;
  }
}

__declspec(dllexport)
void __cdecl GetPluginInfo(PluginInfo* plgi)
{
  plgi->Init = &Init;
}
