#include "pch.h"
//#include "modulever.h"
#include "hooks.h"
#include "pe/module.h"
#include "pe/export_directory.h"
#include "SafeInt/SafeInt.hpp"

struct hook_t
{
  const char *name;
  void **ptr;
  void *detour;
};

template <size_t Size>
void AttachMultipleDetours(
  const wchar_t *module_name,
  const std::array<hook_t, Size> &detours)
{
  if ( const auto module = pe::get_module(module_name) ) {
    for ( const auto &det : detours ) {
      if ( *det.ptr = module->find_function(det.name) )
        DetourAttach(det.ptr, det.detour);
    }
  }
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  switch ( fdwReason ) {
    case DLL_PROCESS_ATTACH: {
      if ( pe::get_module()->base_name() == xorstr_(L"Client.exe") ) {
        NtCurrentPeb()->BeingDebugged = FALSE;

        // Register Dll Notification
        if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
          if ( const auto pfn = reinterpret_cast<decltype(&LdrRegisterDllNotification)>(
            module->find_function(xorstr_("LdrRegisterDllNotification"))) ) {
            pfn(0, &DllNotification, nullptr, &g_pvDllNotificationCookie);
          }
        }

        // Attach Detours
        DetourTransactionBegin();
        DetourUpdateThread(NtCurrentThread());
        AttachMultipleDetours(xorstr_(L"ntdll.dll"),
          std::array {
            hook_t { xorstr_("LdrLoadDll"), &(void *&)g_pfnLdrLoadDll, (void *)&LdrLoadDll_hook },
            hook_t { xorstr_("NtCreateFile"), &(void *&)g_pfnNtCreateFile, (void *)&NtCreateFile_hook },
            hook_t { xorstr_("NtCreateMutant"), &(void *&)g_pfnNtCreateMutant, (void *)&NtCreateMutant_hook },
            hook_t { xorstr_("NtProtectVirtualMemory"), &(void *&)g_pfnNtProtectVirtualMemory, (void *)&NtProtectVirtualMemory_hook },
            hook_t { xorstr_("NtQueryInformationProcess"), &(void *&)g_pfnNtQueryInformationProcess, (void *)&NtQueryInformationProcess_hook },
            hook_t { xorstr_("NtQuerySystemInformation"), &(void *&)g_pfnNtQuerySystemInformation, (void *)&NtQuerySystemInformation_hook },
          });
        AttachMultipleDetours(xorstr_(L"user32.dll"),
          std::array {
            hook_t { xorstr_("FindWindowA"), &(PVOID &)g_pfnFindWindowA, (PVOID)&FindWindowA_hook }
          });
        DetourTransactionCommit();
        break;
      }
    }
  }
  return TRUE;
}

ExternC const PfnDliHook __pfnDliNotifyHook2 = [](unsigned dliNotify, PDelayLoadInfo pdli) -> FARPROC {
  std::array<WCHAR, MAX_PATH> buffer;

  switch ( dliNotify ) {
    case dliStartProcessing:
      break;
    case dliNotePreLoadLibrary: {
      if ( !_stricmp(pdli->szDll, pe::get_instance_module()->export_directory()->name())
        && GetSystemDirectoryW(buffer.data(), SafeInt(buffer.size())) ) {

        auto path = fs::path(buffer.data());
        path /= pdli->szDll;

        return reinterpret_cast<FARPROC>(
          LoadLibraryExW(path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH));
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
