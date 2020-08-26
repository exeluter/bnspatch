#include <phnt_windows.h>
#include <phnt.h>

#include <algorithm>
#include <span>
#include <intrin.h>

#pragma intrinsic(_ReturnAddress)

#include "hooks.h"
#include "pluginsdk.h"
#include "versioninfo.h" 
#include "xmlreader_hooks.h"

#include <fnv1a.h>
#include <pe/exports.h>
#include <pe/module.h>
#include <xorstr.hpp>


BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  if ( fdwReason == DLL_PROCESS_ATTACH )
    DisableThreadLibraryCalls(hInstance);
  return TRUE;
}

decltype(&_initterm) gpfn_initterm;
void _initterm_hook(_PVFV *_First,
                    _PVFV *_Last)
{
  pe::get_module_from_address(_ReturnAddress());

  for ( ; _First < _Last; ++_First ) {
    if ( _First ) {

    }
  }
  _X86_
  gpfn_initterm(_First, _Last);
}

void __cdecl DllLoadedNotification(const struct DllNotificationData *Data, void *Context)
{
  const wchar_t *OriginalFilename;
  if ( GetModuleVersionInfo(Data->BaseOfImage, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)OriginalFilename) >= 0 ) {
    switch ( fnv1a::make_hash(OriginalFilename, fnv1a::ascii_toupper) ) {
      case L"Client.exe"_fnv1au:
      case L"T1Game.exe"_fnv1au:
      case L"XmlReader.dll"_fnv1au: {
        const auto module = static_cast<pe::module *>(Data->BaseOfImage);
        if ( auto sections = module->sections() ) {
          auto section = std::find_if(sections->begin(), sections->end(), [](const pe::section &section) {
            return section.readable() && section.contains_initialized_data() && section.name() == xorstr_(".rdata");
          });
          if ( section != sections->end() ) {
            const auto rdata = std::span{section->data(), section->size()};
            const wchar_t str[] = L"invalid xml reader,. could not load libxml library.";
            const auto needle = std::span{reinterpret_cast<const std::uint8_t *>(str), sizeof(str)};

            std::search(rdata.begin(), rdata.end(), needle.begin(), needle.end());
          }
        }
      }
    }

    VOID InitNotification(VOID)
    {
      const wchar_t *OriginalFilename;
      if ( GetModuleVersionInfo(nullptr, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)OriginalFilename) < 0 )
        return;

      switch ( fnv1a::make_hash(OriginalFilename, fnv1a::ascii_toupper) ) {
        case L"Client.exe"_fnv1au:
        case L"BNSR.exe"_fnv1au:
          NtCurrentPeb()->BeingDebugged = FALSE;
          DetourTransactionBegin();
          DetourUpdateThread(NtCurrentThread());
          if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
#ifdef _M_IX86
            DetourAttach2(module, xorstr_("LdrGetDllHandle"), &(PVOID &)g_pfnLdrGetDllHandle, &LdrGetDllHandle_hook);
#endif
            DetourAttach2(module, xorstr_("LdrLoadDll"), &(PVOID &)g_pfnLdrLoadDll, &LdrLoadDll_hook);
            DetourAttach2(module, xorstr_("NtCreateFile"), &(PVOID &)g_pfnNtCreateFile, &NtCreateFile_hook);
            DetourAttach2(module, xorstr_("NtCreateMutant"), &(PVOID &)g_pfnNtCreateMutant, &NtCreateMutant_hook);
            DetourAttach2(module, xorstr_("NtOpenKeyEx"), &(PVOID &)g_pfnNtOpenKeyEx, &NtOpenKeyEx_hook);
            DetourAttach2(module, xorstr_("NtProtectVirtualMemory"), &(PVOID &)g_pfnNtProtectVirtualMemory, &NtProtectVirtualMemory_hook);
            DetourAttach2(module, xorstr_("NtQuerySystemInformation"), &(PVOID &)g_pfnNtQuerySystemInformation, &NtQuerySystemInformation_hook);
#ifdef _M_X64
            DetourAttach2(module, xorstr_("NtQueryInformationProcess"), &(PVOID &)g_pfnNtQueryInformationProcess, &NtQueryInformationProcess_hook);
            DetourAttach2(module, xorstr_("NtSetInformationThread"), &(PVOID &)g_pfnNtSetInformationThread, &NtSetInformationThread_hook);
#endif
          }
          if ( const auto module = pe::get_module(xorstr_(L"user32.dll")) )
            DetourAttach2(module, xorstr_("FindWindowA"), &(PVOID &)g_pfnFindWindowA, &FindWindowA_hook);
          DetourTransactionCommit();
          break;
      }
    }

#ifndef __DATEW__
#define __DATEW__ _CRT_WIDE(__DATE__)
#endif

    __declspec(dllexport)
      void __cdecl GetPluginInfo(PluginInfo * plgi)
    {
      plgi->Init = &InitNotification;
    }
