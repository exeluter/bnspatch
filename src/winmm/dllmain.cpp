#include <ntdll.h>
#include <delayimp.h>

#include <filesystem>
namespace fs = std::filesystem;

#include <SafeInt.hpp>
#ifdef NDEBUG
#include <xorstr.hpp>
#else
#define xorstr_(str) (str)
#endif

#include "detours/detours.h"
#include <fnv1a.h>
#include "hooks.h"
#include "pe/module.h"
#include "pe/export_directory.h"

LONG detour_attach_api(
  pe::module *module,
  const char *pProcName,
  PVOID *pPointer,
  PVOID pDetour)
{
  if ( !module ) return ERROR_INVALID_PARAMETER;
  if ( !pPointer ) return ERROR_INVALID_PARAMETER;

  if ( *pPointer = module->find_function(pProcName) ) {
    return DetourAttachEx(pPointer, pDetour, nullptr, nullptr, nullptr);
  }
  return ERROR_PROC_NOT_FOUND;
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  if ( fdwReason == DLL_PROCESS_ATTACH ) {
    const auto name = pe::get_module()->base_name();
    switch ( fnv1a::make_hash_upper(name.c_str()) ) {
      case L"Client.exe"_fnv1au:
      case L"BNSR.exe"_fnv1au:
        NtCurrentPeb()->BeingDebugged = FALSE;

        DetourTransactionBegin();
        DetourUpdateThread(NtCurrentThread());
        if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
          prevent_tmd_apiw(module);
          if ( const auto pfnLdrRegisterDllNotification = reinterpret_cast<decltype(&LdrRegisterDllNotification)>(
            module->find_function(xorstr_("LdrRegisterDllNotification"))) ) {
            pfnLdrRegisterDllNotification(0, &DllNotification, nullptr, &g_pvDllNotificationCookie);
          }
#ifdef _M_IX86
          detour_attach_api(module, xorstr_("LdrGetDllHandle"), &(PVOID &)g_pfnLdrGetDllHandle, &LdrGetDllHandle_hook);
#endif
          detour_attach_api(module, xorstr_("LdrLoadDll"), &(PVOID &)g_pfnLdrLoadDll, &LdrLoadDll_hook);
          detour_attach_api(module, xorstr_("NtCreateFile"), &(PVOID &)g_pfnNtCreateFile, &NtCreateFile_hook);
          detour_attach_api(module, xorstr_("NtCreateMutant"), &(PVOID &)g_pfnNtCreateMutant, &NtCreateMutant_hook);
          detour_attach_api(module, xorstr_("NtOpenKeyEx"), &(PVOID &)g_pfnNtOpenKeyEx, &NtOpenKeyEx_hook);
          g_ReadOnlyAddresses.push_back(module->find_function(xorstr_("DbgBreakPoint")));
          g_ReadOnlyAddresses.push_back(module->find_function(xorstr_("DbgUiRemoteBreakin")));
          detour_attach_api(module, xorstr_("NtProtectVirtualMemory"), &(PVOID &)g_pfnNtProtectVirtualMemory, &NtProtectVirtualMemory_hook);
          detour_attach_api(module, xorstr_("NtQuerySystemInformation"), &(PVOID &)g_pfnNtQuerySystemInformation, &NtQuerySystemInformation_hook);
#ifdef _M_X64
          DetourAttachApi(module, xorstr_("NtQueryInformationProcess"), &(PVOID &)g_pfnNtQueryInformationProcess, &NtQueryInformationProcess_hook);
          DetourAttachApi(module, xorstr_("NtSetInformationThread"), &(PVOID &)g_pfnNtSetInformationThread, &NtSetInformationThread_hook);
#endif
        }
        prevent_tmd_apiw(pe::get_module(xorstr_(L"kernel32.dll")));
        if ( const auto module = pe::get_module(xorstr_(L"user32.dll")) ) {
          prevent_tmd_apiw(module);
          detour_attach_api(module, xorstr_("FindWindowA"), &(PVOID &)g_pfnFindWindowA, &FindWindowA_hook);
        }
        DetourTransactionCommit();
        break;
    }
  }
  return TRUE;
}

ExternC const PfnDliHook __pfnDliNotifyHook2 = [](unsigned dliNotify, PDelayLoadInfo pdli) -> FARPROC {

  switch ( dliNotify ) {
    case dliStartProcessing:
      break;
    case dliNotePreLoadLibrary: {
      std::array<WCHAR, MAX_PATH> Buffer;
      if ( !_stricmp(pdli->szDll, pe::instance_module()->export_directory()->name())
        && GetSystemDirectoryW(Buffer.data(), SafeInt(Buffer.size())) ) {

        auto Path = fs::path(Buffer.data());
        Path /= pdli->szDll;
        auto Module = reinterpret_cast<pe::module *>(
          LoadLibraryExW(Path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH));

        return reinterpret_cast<FARPROC>(Module);
      }
      break;
    }
    case dliNotePreGetProcAddress:
    case dliFailLoadLib:
    case dliFailGetProc:
    case dliNoteEndProcessing:
      break;
  }
  return nullptr;
};
