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
#include <pe/module.h>
#include <pe/export_directory.h>
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

  if ( fdwReason == DLL_PRrOCESS_ATTACH
    && GetModuleVersionInfo(nullptr, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)OriginalFilename) >= 0 ) {
    switch ( fnv1a::make_hash(OriginalFilename, false) ) {
      case L"Client.exe"_fnv1ai:
      case L"BNSR.exe"_fnv1ai:
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
  pe::module* module;
  const char* name;
  wchar_t drive[_MAX_DRIVE];
  wchar_t dir[_MAX_DIR];
  wchar_t fname[_MAX_FNAME];
  wchar_t ext[_MAX_EXT];
  wchar_t path[_MAX_PATH];
  wchar_t buffer[_MAX_FNAME];
  unsigned num = UINT_MAX;
  HMODULE result;
  UINT count;

  switch (dliNotify) {
  case dliStartProcessing:
    break;
  case dliNotePreLoadLibrary:
    module = pe::instance_module();
    name = module->export_directory()->name();
    if (!_stricmp(pdli->szDll, name)) {
      if (GetModuleFileNameW(module, path, _countof(path))
        && !_wsplitpath_s(path, drive, dir, fname, ext)
        && swscanf_s(fname, L"%ls%u", buffer, (unsigned int)_countof(buffer), &num) > 0
        && swprintf_s(fname, L"%ls%03u", buffer, num + 1) >= 0
        && !_wmakepath_s(path, drive, dir, fname, ext)) {
        result = LoadLibraryExW(path, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (result)
          return (FARPROC)result;
      }
      count = GetSystemDirectoryW(path, (UINT)_countof(path));
      if (count
        && swprintf_s(path + count, _countof(path) - count, L"\\%hs", name) >= 0) {
        return (FARPROC)LoadLibraryExW(path, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
      }
    }
    break;
  case dliNotePreGetProcAddress:
  case dliFailLoadLib:
  case dliFailGetProc:
  case dliNoteEndProcessing:
    break;
  }
  return nullptr;
};
