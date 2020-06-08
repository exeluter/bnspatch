#include <phnt_windows.h>
#include <phnt.h>

#include "hooks.h"
#include "versioninfo.h"
#include <detours.h>
#include <fnv1a.h>
#include <pe/export_directory.h>
#include <pe/module.h>
#include <xorstr.hpp>

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  if ( fdwReason == DLL_PROCESS_ATTACH )
    DisableThreadLibraryCalls(hInstance);
  return TRUE;
}

LONG DetourAttach(
  pe::module *module,
  const char *pProcName,
  PVOID *pPointer,
  PVOID pDetour)
{
  if ( !module ) return ERROR_INVALID_PARAMETER;
  if ( !pPointer ) return ERROR_INVALID_PARAMETER;

  if ( *pPointer = module->function(pProcName) ) {
    return DetourAttachEx(pPointer, pDetour, nullptr, nullptr, nullptr);
  }
  return ERROR_PROC_NOT_FOUND;
}

typedef struct _PLUGIN_INFO
{
  const wchar_t *pwzName;
  const wchar_t *pwzVersion;
  const wchar_t *pwzDescription;
  void(__cdecl *pfnInit)(void);
} PLUGIN_INFO;
typedef void(__cdecl *PFN_GETPLUGININFO)(PLUGIN_INFO *);

void __cdecl PluginInit(void)
{
  const wchar_t *fileName;

  if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
    DetourTransactionBegin();
    DetourUpdateThread(NtCurrentThread());
    if ( const auto pfnLdrRegisterDllNotification = reinterpret_cast<decltype(&LdrRegisterDllNotification)>(
      module->function(xorstr_("LdrRegisterDllNotification"))) ) {
      pfnLdrRegisterDllNotification(0, &DllNotification, nullptr, &g_pvDllNotificationCookie);
    }
    if ( GetModuleVersionInfo(nullptr, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)fileName) >= 0 ) {
      switch ( fnv1a::make_hash(fileName, towupper) ) {
        case L"Client.exe"_fnv1au:
        case L"BNSR.exe"_fnv1au:
          NtCurrentPeb()->BeingDebugged = FALSE;
#ifdef _M_IX86
          DetourAttach(module, xorstr_("LdrGetDllHandle"), &(PVOID &)g_pfnLdrGetDllHandle, &LdrGetDllHandle_hook);
#endif
          DetourAttach(module, xorstr_("LdrLoadDll"), &(PVOID &)g_pfnLdrLoadDll, &LdrLoadDll_hook);
          DetourAttach(module, xorstr_("NtCreateFile"), &(PVOID &)g_pfnNtCreateFile, &NtCreateFile_hook);
          DetourAttach(module, xorstr_("NtCreateMutant"), &(PVOID &)g_pfnNtCreateMutant, &NtCreateMutant_hook);
          DetourAttach(module, xorstr_("NtOpenKeyEx"), &(PVOID &)g_pfnNtOpenKeyEx, &NtOpenKeyEx_hook);
          g_ReadOnlyAddresses[0] = (ULONG_PTR)module->function(xorstr_("DbgBreakPoint"));
          g_ReadOnlyAddresses[1] = (ULONG_PTR)module->function(xorstr_("DbgUiRemoteBreakin"));
          DetourAttach(module, xorstr_("NtProtectVirtualMemory"), &(PVOID &)g_pfnNtProtectVirtualMemory, &NtProtectVirtualMemory_hook);
          DetourAttach(module, xorstr_("NtQuerySystemInformation"), &(PVOID &)g_pfnNtQuerySystemInformation, &NtQuerySystemInformation_hook);
#ifdef _M_X64
          DetourAttach(module, xorstr_("NtQueryInformationProcess"), &(PVOID &)g_pfnNtQueryInformationProcess, &NtQueryInformationProcess_hook);
          DetourAttach(module, xorstr_("NtSetInformationThread"), &(PVOID &)g_pfnNtSetInformationThread, &NtSetInformationThread_hook);
#endif
          if ( const auto module = pe::get_module(xorstr_(L"user32.dll")) )
            DetourAttach(module, xorstr_("FindWindowA"), &(PVOID &)g_pfnFindWindowA, &FindWindowA_hook);
          break;
      }
      DetourTransactionCommit();
    }
  }
}

__declspec(dllexport)
void __cdecl GetPluginInfo(PLUGIN_INFO *pluginInfo)
{
  static std::once_flag once_flag;
  static auto name = xorstr(L"bnspatch");
  static auto version = xorstr(L"20200607");
  static auto description = xorstr(L"XML patching, multi-client, and bypasses some Themida/WL protections");

  std::call_once(once_flag, [](auto &name, auto &version, auto &description) {
    name.crypt();
    version.crypt();
    description.crypt();
  }, name, version, description);

  pluginInfo->pwzName = name.get();
  pluginInfo->pwzVersion = version.get();
  pluginInfo->pwzDescription = description.get();
  pluginInfo->pfnInit = &PluginInit;
}
