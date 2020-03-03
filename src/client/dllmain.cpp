#include "pch.h"
//#include "modulever.h"
#include "hooks.h"
#include "pe/module.h"
#include "pe/export_directory.h"
#include "SafeInt/SafeInt.hpp"

LONG DetourAttachApi(
  pe::module *module,
  PCSTR pProcName,
  PVOID *pPointer,
  PVOID pDetour)
{
  if ( !module ) return ERROR_INVALID_PARAMETER;
  if ( !pPointer ) return ERROR_INVALID_PARAMETER;

  if ( *pPointer = module->find_function(pProcName) )
    return DetourAttach(pPointer, pDetour);

  return ERROR_PROC_NOT_FOUND;
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  switch ( fdwReason ) {
    case DLL_PROCESS_ATTACH: {
      auto name = pe::get_module()->base_name();
      if ( name == xorstr_(L"Client.exe") || name == xorstr_(L"BNSR.exe") ) {
        NtCurrentPeb()->BeingDebugged = FALSE;
        if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
          if ( const auto pfn = reinterpret_cast<decltype(&LdrRegisterDllNotification)>(
            module->find_function(xorstr_("LdrRegisterDllNotification"))) ) {
            pfn(0, &DllNotification, nullptr, &g_pvDllNotificationCookie);
          }
        }

        DetourTransactionBegin();
        DetourUpdateThread(NtCurrentThread());
        if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
          DetourAttachApi(module, xorstr_("LdrLoadDll"), &(PVOID &)g_pfnLdrLoadDll, &LdrLoadDll_hook);
          DetourAttachApi(module, xorstr_("NtCreateFile"), &(PVOID &)g_pfnNtCreateFile, &NtCreateFile_hook);
          DetourAttachApi(module, xorstr_("NtCreateMutant"), &(PVOID &)g_pfnNtCreateMutant, &NtCreateMutant_hook);
          DetourAttachApi(module, xorstr_("NtProtectVirtualMemory"), &(PVOID &)g_pfnNtProtectVirtualMemory, &NtProtectVirtualMemory_hook);
          DetourAttachApi(module, xorstr_("NtQueryInformationProcess"), &(PVOID &)g_pfnNtQueryInformationProcess, &NtQueryInformationProcess_hook);
          DetourAttachApi(module, xorstr_("NtQuerySystemInformation"), &(PVOID &)g_pfnNtQuerySystemInformation, &NtQuerySystemInformation_hook);
        }
        if ( const auto module = pe::get_module(xorstr_(L"user32.dll")) ) {
          DetourAttachApi(module, xorstr_("FindWindowA"), &(PVOID &)g_pfnFindWindowA, &FindWindowA_hook);
          //DetourAttachApi(module, xorstr_("CreateWindowExW"), &(PVOID &)g_pfnCreateWindowExW, &CreateWindowExW_hook);
        }
        DetourTransactionCommit();
        break;
      }
    }
  }
  return TRUE;
}

ExternC const PfnDliHook __pfnDliNotifyHook2 = [](unsigned dliNotify, PDelayLoadInfo pdli) -> FARPROC {
  std::array<WCHAR, MAX_PATH> Buffer;

  switch ( dliNotify ) {
    case dliStartProcessing:
      break;
    case dliNotePreLoadLibrary:
      if ( !_stricmp(pdli->szDll, pe::get_instance_module()->export_directory()->name())
        && GetSystemDirectoryW(Buffer.data(), SafeInt(Buffer.size())) ) {

        auto Path = fs::path(Buffer.data());
        Path /= pdli->szDll;
        auto Module = reinterpret_cast<pe::module *>(
          LoadLibraryExW(Path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH));

        return reinterpret_cast<FARPROC>(Module);
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
