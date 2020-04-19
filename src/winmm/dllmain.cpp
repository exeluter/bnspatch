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
#include <wil/stl.h>
#include <wil/win32_helpers.h>
#include "versioninfo.h"

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
  PCWSTR OriginalFilename;

  if ( fdwReason == DLL_PROCESS_ATTACH
    && GetModuleVersionInfo(nullptr, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)OriginalFilename) >= 0 ) {
    switch ( fnv1a::make_hash_upper(OriginalFilename) ) {
      case L"Client.exe"_fnv1au:
      case L"BNSR.exe"_fnv1au:
        NtCurrentPeb()->BeingDebugged = FALSE;

        DetourTransactionBegin();
        DetourUpdateThread(NtCurrentThread());
        if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
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
          detour_attach_api(module, xorstr_("NtQueryInformationProcess"), &(PVOID &)g_pfnNtQueryInformationProcess, &NtQueryInformationProcess_hook);
          detour_attach_api(module, xorstr_("NtSetInformationThread"), &(PVOID &)g_pfnNtSetInformationThread, &NtSetInformationThread_hook);
#endif
        }
        if ( const auto module = pe::get_module(xorstr_(L"user32.dll")) ) {
          detour_attach_api(module, xorstr_("FindWindowA"), &(PVOID &)g_pfnFindWindowA, &FindWindowA_hook);
        }
        DetourTransactionCommit();
        break;
    }
  }
  return TRUE;
}

ExternC const PfnDliHook __pfnDliNotifyHook2 = [](unsigned dliNotify, PDelayLoadInfo pdli) -> FARPROC
{
  switch ( dliNotify ) {
    case dliStartProcessing:
      break;
    case dliNotePreLoadLibrary: {
      if ( !_stricmp(pdli->szDll, pe::instance_module()->export_directory()->name()) ) {
        auto path = wil::GetModuleFileNameW(pe::instance_module());
        wchar_t drive[_MAX_DRIVE];
        wchar_t dir[_MAX_DIR];
        wchar_t fname[_MAX_FNAME];
        wchar_t ext[_MAX_EXT];
        if ( !_wsplitpath_s(path.get(), drive, dir, fname, ext) ) {
          wchar_t buffer[6];
          int num = -1;
          wchar_t next_path[_MAX_PATH];
          if ( swscanf_s(fname, L"%5ls%3d", buffer, (unsigned int)_countof(buffer), &num) > 0
            && num < 999
            && swprintf_s(fname, L"%.5ls%03d", buffer, num + 1) >= 0
            && !_wmakepath_s(next_path, drive, dir, fname, ext) ) {
            auto hModule = LoadLibraryExW(next_path, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
            if ( hModule )
              return reinterpret_cast<FARPROC>(hModule);
          }
        }
        std::wstring result;
        if ( SUCCEEDED(wil::GetSystemDirectoryW(result)) ) {
          auto Path = fs::path(result);
          Path /= pdli->szDll;
          return reinterpret_cast<FARPROC>(LoadLibraryExW(Path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH));
        }
        break;
      }
    case dliNotePreGetProcAddress:
    case dliFailLoadLib:
    case dliFailGetProc:
    case dliNoteEndProcessing:
      break;
    }
  }
  return nullptr;
};
