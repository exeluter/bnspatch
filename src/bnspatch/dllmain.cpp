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

const DetoursData *g_DetoursData;

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  if ( fdwReason == DLL_PROCESS_ATTACH )
    DisableThreadLibraryCalls(hInstance);
  return TRUE;
}

BOOL(WINAPI *g_pfnDllEntryPoint)(HINSTANCE, DWORD, LPVOID);
BOOL WINAPI DllEntryPoint_hook(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  const auto res = g_pfnDllEntryPoint(hInstance, fdwReason, lpvReserved);
  if ( res && fdwReason == DLL_PROCESS_ATTACH ) {
    g_DetoursData->TransactionBegin();
    g_DetoursData->UpdateThread(NtCurrentThread());
    const auto GetInterfaceVersion = reinterpret_cast<const wchar_t *(__cdecl *)(void)>(GetProcAddress(hInstance, xorstr_("GetInterfaceVersion")));
    const auto CreateXmlReader = reinterpret_cast<void *(__cdecl *)()>(GetProcAddress(hInstance, xorstr_("CreateXmlReader")));
    const auto DestroyXmlReader = reinterpret_cast<void(__cdecl *)(void *)>(GetProcAddress(hInstance, xorstr_("DestroyXmlReader")));
    if ( GetInterfaceVersion && CreateXmlReader && DestroyXmlReader ) {
      auto xmlReader = std::unique_ptr<void, decltype(DestroyXmlReader)>(CreateXmlReader(), DestroyXmlReader);
      auto vfptr = *reinterpret_cast<void ***>(xmlReader.get());
      switch ( fnv1a::make_hash(GetInterfaceVersion()) ) {
        case L"13"_fnv1a:
          g_pfnReadFromFile13 = reinterpret_cast<decltype(g_pfnReadFromFile13)>(vfptr[6]);
          g_DetoursData->Attach(&(PVOID &)g_pfnReadFromFile13, ReadFromFile13_hook);
          g_pfnReadFromBuffer13 = reinterpret_cast<decltype(g_pfnReadFromBuffer13)>(vfptr[7]);
          g_DetoursData->Attach(&(PVOID &)g_pfnReadFromBuffer13, ReadFromBuffer13_hook);
          break;

        case L"14"_fnv1a:
        case L"15"_fnv1a:
          g_pfnReadFromFile14 = reinterpret_cast<decltype(g_pfnReadFromFile14)>(vfptr[6]);
          g_DetoursData->Attach(&(PVOID &)g_pfnReadFromFile14, ReadFromFile14_hook);
          g_pfnReadFromBuffer14 = reinterpret_cast<decltype(g_pfnReadFromBuffer14)>(vfptr[7]);
          g_DetoursData->Attach(&(PVOID &)g_pfnReadFromBuffer14, ReadFromBuffer14_hook);
          break;
      }
    }
    g_DetoursData->Detach(&(PVOID &)g_pfnDllEntryPoint, &DllEntryPoint_hook);
    g_DetoursData->TransactionCommit();
  }
  return res;
}

void __cdecl DllLoadedNotification(const struct DllNotificationData *Data, void *Context)
{
  const wchar_t *OriginalFilename;
  if ( GetModuleVersionInfo(Data->BaseOfImage, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)OriginalFilename) >= 0 ) {
    switch ( fnv1a::make_hash(OriginalFilename, fnv1a::ascii_toupper) ) {
      case L"XmlReader.dll"_fnv1au: {
        const auto module = static_cast<pe::module *>(Data->BaseOfImage);
        g_DetoursData->TransactionBegin();
        g_DetoursData->UpdateThread(NtCurrentThread());
        g_pfnDllEntryPoint = reinterpret_cast<decltype(g_pfnDllEntryPoint)>(module->rva_to(module->nt_header()->OptionalHeader.AddressOfEntryPoint));
        g_DetoursData->Attach(&(PVOID &)g_pfnDllEntryPoint, &DllEntryPoint_hook);
        g_DetoursData->TransactionCommit();
        break;
      }
    }
  }
}

void __cdecl InitNotification(const struct InitNotificationData *Data, void *Context)
{
  g_DetoursData = Data->Detours;

  const wchar_t *OriginalFilename;
  if ( GetModuleVersionInfo(nullptr, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)OriginalFilename) < 0 )
    return;

  switch ( fnv1a::make_hash(OriginalFilename, fnv1a::ascii_toupper) ) {
    case L"Client.exe"_fnv1au:
    case L"BNSR.exe"_fnv1au:
      NtCurrentPeb()->BeingDebugged = FALSE;
      g_DetoursData->TransactionBegin();
      g_DetoursData->UpdateThread(NtCurrentThread());
      if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
#ifdef _M_IX86
        g_DetoursData->Attach2(module, xorstr_("LdrGetDllHandle"), &(PVOID &)g_pfnLdrGetDllHandle, &LdrGetDllHandle_hook);
#endif
        g_DetoursData->Attach2(module, xorstr_("LdrLoadDll"), &(PVOID &)g_pfnLdrLoadDll, &LdrLoadDll_hook);
        g_DetoursData->Attach2(module, xorstr_("NtCreateFile"), &(PVOID &)g_pfnNtCreateFile, &NtCreateFile_hook);
        g_DetoursData->Attach2(module, xorstr_("NtCreateMutant"), &(PVOID &)g_pfnNtCreateMutant, &NtCreateMutant_hook);
        g_DetoursData->Attach2(module, xorstr_("NtOpenKeyEx"), &(PVOID &)g_pfnNtOpenKeyEx, &NtOpenKeyEx_hook);
        g_DetoursData->Attach2(module, xorstr_("NtProtectVirtualMemory"), &(PVOID &)g_pfnNtProtectVirtualMemory, &NtProtectVirtualMemory_hook);
        g_DetoursData->Attach2(module, xorstr_("NtQuerySystemInformation"), &(PVOID &)g_pfnNtQuerySystemInformation, &NtQuerySystemInformation_hook);
#ifdef _M_X64
        g_DetoursData->Attach2(module, xorstr_("NtQueryInformationProcess"), &(PVOID &)g_pfnNtQueryInformationProcess, &NtQueryInformationProcess_hook);
        g_DetoursData->Attach2(module, xorstr_("NtSetInformationThread"), &(PVOID &)g_pfnNtSetInformationThread, &NtSetInformationThread_hook);
#endif
      }
      if ( const auto module = pe::get_module(xorstr_(L"user32.dll")) )
        g_DetoursData->Attach2(module, xorstr_("FindWindowA"), &(PVOID &)g_pfnFindWindowA, &FindWindowA_hook);
      g_DetoursData->TransactionCommit();
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
