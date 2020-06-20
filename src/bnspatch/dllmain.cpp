#include <phnt_windows.h>
#include <phnt.h>

#include "hooks.h"
#include "pluginsdk.h"
#include "versioninfo.h" 
#include "xmlreader_hooks.h"

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

void __cdecl DllLoadedNotification(const struct DllNotificationData *Data, void *Context)
{
  const wchar_t *original_filename;
  if ( GetModuleVersionInfo(Data->BaseOfImage, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)original_filename) >= 0 ) {
    switch ( fnv1a::make_hash(original_filename, towupper) ) {
      case L"XmlReader.dll"_fnv1au: {
        const auto module = static_cast<pe::module *>(Data->BaseOfImage);
        auto const pfnGetInterfaceVersion = reinterpret_cast<wchar_t const *(__cdecl *)()>(module->function(xorstr_("GetInterfaceVersion")));
        auto const pfnCreateXmlReader = reinterpret_cast<void *(__cdecl *)()>(module->function(xorstr_("CreateXmlReader")));
        auto const pfnDestroyXmlReader = reinterpret_cast<void(__cdecl *)(void *)>(module->function(xorstr_("DestroyXmlReader")));
        if ( pfnGetInterfaceVersion && pfnCreateXmlReader && pfnDestroyXmlReader ) {
          Data->Detours->TransactionBegin();
          Data->Detours->UpdateThread(NtCurrentThread());
          auto xmlReader = std::unique_ptr<void, decltype(pfnDestroyXmlReader)>(pfnCreateXmlReader(), pfnDestroyXmlReader);
          auto vfptr = *reinterpret_cast<void ***>(xmlReader.get());
          switch ( _wtoi(pfnGetInterfaceVersion()) ) {
            case 13:
              g_pfnReadFromFile13 = reinterpret_cast<decltype(g_pfnReadFromFile13)>(vfptr[6]);
              Data->Detours->Attach(&(PVOID &)g_pfnReadFromFile13, ReadFromFile13_hook);
              g_pfnReadFromBuffer13 = reinterpret_cast<decltype(g_pfnReadFromBuffer13)>(vfptr[7]);
              Data->Detours->Attach(&(PVOID &)g_pfnReadFromBuffer13, ReadFromBuffer13_hook);
              break;
            case 14:
            case 15: // no known difference between interface 14 and 15...
              g_pfnReadFromFile14 = reinterpret_cast<decltype(g_pfnReadFromFile14)>(vfptr[6]);
              Data->Detours->Attach(&(PVOID &)g_pfnReadFromFile14, ReadFromFile14_hook);
              g_pfnReadFromBuffer14 = reinterpret_cast<decltype(g_pfnReadFromBuffer14)>(vfptr[7]);
              Data->Detours->Attach(&(PVOID &)g_pfnReadFromBuffer14, ReadFromBuffer14_hook);
              break;
          }
          Data->Detours->TransactionCommit();
        }
      }
    }
  }
}

void __cdecl InitNotification(const struct InitNotificationData *Data, void *Context)
{
  const wchar_t *OriginalFilename;
  if ( GetModuleVersionInfo(nullptr, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)OriginalFilename) < 0 )
    return;

  switch ( fnv1a::make_hash(OriginalFilename, towupper) ) {
    case L"Client.exe"_fnv1au:
    case L"BNSR.exe"_fnv1au:
      NtCurrentPeb()->BeingDebugged = FALSE;
      Data->Detours->TransactionBegin();
      Data->Detours->UpdateThread(NtCurrentThread());
      if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
#ifdef _M_IX86
        Data->Detours->Attach2(module, xorstr_("LdrGetDllHandle"), &(PVOID &)g_pfnLdrGetDllHandle, &LdrGetDllHandle_hook);
#endif
        Data->Detours->Attach2(module, xorstr_("LdrLoadDll"), &(PVOID &)g_pfnLdrLoadDll, &LdrLoadDll_hook);
        Data->Detours->Attach2(module, xorstr_("NtCreateFile"), &(PVOID &)g_pfnNtCreateFile, &NtCreateFile_hook);
        Data->Detours->Attach2(module, xorstr_("NtCreateMutant"), &(PVOID &)g_pfnNtCreateMutant, &NtCreateMutant_hook);
        Data->Detours->Attach2(module, xorstr_("NtOpenKeyEx"), &(PVOID &)g_pfnNtOpenKeyEx, &NtOpenKeyEx_hook);
        Data->Detours->Attach2(module, xorstr_("NtProtectVirtualMemory"), &(PVOID &)g_pfnNtProtectVirtualMemory, &NtProtectVirtualMemory_hook);
        Data->Detours->Attach2(module, xorstr_("NtQuerySystemInformation"), &(PVOID &)g_pfnNtQuerySystemInformation, &NtQuerySystemInformation_hook);
#ifdef _M_X64
        Data->Detours->Attach2(module, xorstr_("NtQueryInformationProcess"), &(PVOID &)g_pfnNtQueryInformationProcess, &NtQueryInformationProcess_hook);
        Data->Detours->Attach2(module, xorstr_("NtSetInformationThread"), &(PVOID &)g_pfnNtSetInformationThread, &NtSetInformationThread_hook);
#endif
      }
      if ( const auto module = pe::get_module(xorstr_(L"user32.dll")) )
        Data->Detours->Attach2(module, xorstr_("FindWindowA"), &(PVOID &)g_pfnFindWindowA, &FindWindowA_hook);
      Data->Detours->TransactionCommit();
      break;
  }
}

#ifndef __DATEW__
#define __DATEW__ _CRT_WIDE(__DATE__)
#endif

__declspec(dllexport)
void __cdecl GetPluginInfo2(PluginInfo2 *plgi)
{
  static std::once_flag once_flag;
  static auto name = xorstr(L"bnspatch");
  static auto version = xorstr(__DATEW__);
  static auto description = xorstr(L"XML patching, multi-client, and bypasses some Themida/WL protections");

  std::call_once(once_flag, [](auto &name, auto &version, auto &description) {
    name.crypt();
    version.crypt();
    description.crypt();
  }, name, version, description);

  plgi->Name = name.get();
  plgi->Version = version.get();
  plgi->Description = description.get();
  plgi->InitNotification = &InitNotification;
  plgi->DllLoadedNotification = &DllLoadedNotification;
}
