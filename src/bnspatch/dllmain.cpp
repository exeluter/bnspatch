#include <ntdll.h>
#ifdef NDEBUG
#include <xorstr.hpp>
#else
#define xorstr_(str) (str)
#endif

#include <pe/module.h>
#include <pe/export_directory.h>
#include <fnv1a.h>
#include "detours/detours.h"
#include "hooks.h"
#include "versioninfo.h"

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  if (fdwReason == DLL_PROCESS_ATTACH)
    DisableThreadLibraryCalls(hInstance);
  return TRUE;
}

LONG DetourAttach(
  pe::module* module,
  const char* pProcName,
  PVOID* pPointer,
  PVOID pDetour)
{
  if (!module) return ERROR_INVALID_PARAMETER;
  if (!pPointer) return ERROR_INVALID_PARAMETER;

  if (*pPointer = module->find_function(pProcName)) {
    return DetourAttachEx(pPointer, pDetour, nullptr, nullptr, nullptr);
  }
  return ERROR_PROC_NOT_FOUND;
}

typedef void(__cdecl* PFN_PLUGIN_INIT)(void);
struct PLUGIN_INFO {
  const wchar_t* pwzName;
  const wchar_t* pwzVersion;
  const wchar_t* pwzDescription;
  PFN_PLUGIN_INIT pfnInit;
};

void __cdecl PluginInit(void) {
  const wchar_t* fileName;

  if (GetModuleVersionInfo(nullptr, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID&)fileName) >= 0) {
    switch (fnv1a::make_hash(fileName, false)) {
      case L"Client.exe"_fnv1ai:
      case L"BNSR.exe"_fnv1ai:
        NtCurrentPeb()->BeingDebugged = FALSE;

        DetourTransactionBegin();
        DetourUpdateThread(NtCurrentThread());
        if (const auto module = pe::get_module(xorstr_(L"ntdll.dll"))) {
          if (const auto pfnLdrRegisterDllNotification = reinterpret_cast<decltype(&LdrRegisterDllNotification)>(
            module->find_function(xorstr_("LdrRegisterDllNotification")))) {
            pfnLdrRegisterDllNotification(0, &DllNotification, nullptr, &g_pvDllNotificationCookie);
          }
#ifdef _M_IX86
          DetourAttach(module, xorstr_("LdrGetDllHandle"), &(PVOID&)g_pfnLdrGetDllHandle, &LdrGetDllHandle_hook);
#endif
          DetourAttach(module, xorstr_("LdrLoadDll"), &(PVOID&)g_pfnLdrLoadDll, &LdrLoadDll_hook);
          DetourAttach(module, xorstr_("NtCreateFile"), &(PVOID&)g_pfnNtCreateFile, &NtCreateFile_hook);
          DetourAttach(module, xorstr_("NtCreateMutant"), &(PVOID&)g_pfnNtCreateMutant, &NtCreateMutant_hook);
          DetourAttach(module, xorstr_("NtOpenKeyEx"), &(PVOID&)g_pfnNtOpenKeyEx, &NtOpenKeyEx_hook);
          g_ReadOnlyAddresses.push_back(module->find_function(xorstr_("DbgBreakPoint")));
          g_ReadOnlyAddresses.push_back(module->find_function(xorstr_("DbgUiRemoteBreakin")));
          DetourAttach(module, xorstr_("NtProtectVirtualMemory"), &(PVOID&)g_pfnNtProtectVirtualMemory, &NtProtectVirtualMemory_hook);
          DetourAttach(module, xorstr_("NtQuerySystemInformation"), &(PVOID&)g_pfnNtQuerySystemInformation, &NtQuerySystemInformation_hook);
#ifdef _M_X64
          DetourAttach(module, xorstr_("NtQueryInformationProcess"), &(PVOID&)g_pfnNtQueryInformationProcess, &NtQueryInformationProcess_hook);
          DetourAttach(module, xorstr_("NtSetInformationThread"), &(PVOID&)g_pfnNtSetInformationThread, &NtSetInformationThread_hook);
#endif
        }
        if (const auto module = pe::get_module(xorstr_(L"user32.dll")))
          DetourAttach(module, xorstr_("FindWindowA"), &(PVOID&)g_pfnFindWindowA, &FindWindowA_hook);
        DetourTransactionCommit();
        break;
    }
  }
}

__declspec(dllexport)
void __cdecl GetPluginInfo(PLUGIN_INFO* pluginInfo) {
  pluginInfo->pwzName = L"bnspatch";
  pluginInfo->pwzVersion = L"1.0";
  pluginInfo->pwzDescription = L"XML patching, multi-client, and bypasses some Themida/WL protections";
  pluginInfo->pfnInit = &PluginInit;
}
