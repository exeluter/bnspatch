#include "pch.h"
#include "dllname.h"
//#include "modulever.h"
#include "hooks.h"

extern "C" BOOL WINAPI DllMain(
  HINSTANCE hInstance,
  DWORD     fdwReason,
  LPVOID    lpvReserved)
{
  //PCNZWCH ProductName;
  //PCNZWCH OriginalFilename;

  switch ( fdwReason ) {
  case DLL_PROCESS_ATTACH:
    DisableThreadLibraryCalls(hInstance);
    //if ( GetModuleVersionInfo(nullptr, L"\\StringFileInfo\\*\\ProductName", &(LPCVOID &)ProductName) >= 0
    //    && !wcscmp(ProductName, L"Blade & Soul")
    //    && GetModuleVersionInfo(nullptr, L"\\StringFileInfo\\*\\OriginalFilename", &(LPCVOID &)OriginalFilename) >= 0
    //    && !wcscmp(OriginalFilename, L"Client.exe") )
    {
#ifdef _DEBUG
      auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
      auto msvc_logger = std::make_shared<spdlog::logger>("msvc_logger", sink);
      if ( GetAsyncKeyState(VK_CONTROL) & 0x8000 )
        msvc_logger->set_level(spdlog::level::debug);
      msvc_logger->set_pattern("[%l] %!: %v");
      spdlog::set_default_logger(msvc_logger);
#endif

      DetourTransactionBegin();
      DetourUpdateThread(GetCurrentThread());

      if ( auto h = GetModuleHandleW(L"ntdll.dll") ) {
        g_pfnLdrGetDllHandle = (decltype(g_pfnLdrGetDllHandle))GetProcAddress(h, "LdrGetDllHandle");
        SPDLOG_INFO(fmt("ntdll!LdrGetDllHandle: {}"), (PVOID)g_pfnLdrGetDllHandle);
        if ( g_pfnLdrGetDllHandle )
          DetourAttach(&(PVOID &)g_pfnLdrGetDllHandle, LdrGetDllHandle_hook);

        g_pfnLdrLoadDll = (decltype(g_pfnLdrLoadDll))GetProcAddress(h, "LdrLoadDll");
        SPDLOG_INFO(fmt("ntdll!LdrLoadDll: {}"), (PVOID)g_pfnLdrLoadDll);
        if ( g_pfnLdrLoadDll )
          DetourAttach(&(PVOID &)g_pfnLdrLoadDll, LdrLoadDll_hook);

        g_pfnNtCreateFile = (decltype(g_pfnNtCreateFile))GetProcAddress(h, "NtCreateFile");
        SPDLOG_INFO(fmt("ntdll!NtCreateFile: {}"), (PVOID)g_pfnNtCreateFile);
        if ( g_pfnNtCreateFile )
          DetourAttach(&(PVOID &)g_pfnNtCreateFile, NtCreateFile_hook);

        g_pfnNtCreateMutant = (decltype(g_pfnNtCreateMutant))GetProcAddress(h, "NtCreateMutant");
        SPDLOG_INFO(fmt("ntdll!NtCreateMutant: {}"), (PVOID)g_pfnNtCreateMutant);
        if ( g_pfnNtCreateMutant )
          DetourAttach(&(PVOID &)g_pfnNtCreateMutant, NtCreateMutant_hook);

        g_pfnNtProtectVirtualMemory = (decltype(g_pfnNtProtectVirtualMemory))GetProcAddress(h, "NtProtectVirtualMemory");
        SPDLOG_INFO(fmt("ntdll!NtProtectVirtualMemory: {}"), (PVOID)g_pfnNtProtectVirtualMemory);
        if ( g_pfnNtProtectVirtualMemory )
          DetourAttach(&(PVOID &)g_pfnNtProtectVirtualMemory, NtProtectVirtualMemory_hook);

        g_pfnNtQuerySystemInformation = (decltype(g_pfnNtQuerySystemInformation))GetProcAddress(h, "NtQuerySystemInformation");
        SPDLOG_INFO(fmt("ntdll!NtQuerySystemInformation: {}"), (PVOID)g_pfnNtQuerySystemInformation);
        if ( g_pfnNtQuerySystemInformation )
          DetourAttach(&(PVOID &)g_pfnNtQuerySystemInformation, NtQuerySystemInformation_hook);
      }

      if ( auto h = GetModuleHandleW(L"user32.dll") ) {
        g_pfnFindWindowA = (decltype(g_pfnFindWindowA))GetProcAddress(h, "FindWindowA");
        SPDLOG_INFO(fmt("user32!FindWindowA: {}"), (PVOID)g_pfnFindWindowA);
        if ( g_pfnFindWindowA )
          DetourAttach(&(PVOID &)g_pfnFindWindowA, FindWindowA_hook);
      }
      DetourTransactionCommit();
      break;
    }
  case DLL_PROCESS_DETACH:
    break;
  }
  return TRUE;
}

ExternC
const PfnDliHook __pfnDliNotifyHook2 = [](
  unsigned       dliNotify,
  PDelayLoadInfo pdli) -> FARPROC
{
  PCSTR DllName;
  std::wstring result;

  switch ( dliNotify ) {
  case dliStartProcessing:
    break;
  case dliNotePreLoadLibrary:
    if ( NtCurrentPeb()->BeingDebugged ) {
#ifdef _M_X64
      auto str = fmt::format(
        fmt(L"\"InjectorCLIx64.exe\" pid:{:#x} \"HookLibraryx64.dll\" nowait"), 
        GetCurrentProcessId());
#else
      auto str = fmt::format(
        fmt(L"\"InjectorCLIx86.exe\" pid:{:#x} \"HookLibraryx86.dll\" nowait"), 
        GetCurrentProcessId());
#endif
      STARTUPINFO StartupInfo = { sizeof(STARTUPINFO) };
      wil::unique_process_information ProcessInfo;
      if ( CreateProcessW(
        nullptr,
        str.data(),
        nullptr,
        nullptr,
        FALSE,
        CREATE_NO_WINDOW,
        nullptr, 
        nullptr, 
        &StartupInfo, 
        &ProcessInfo) )
        wil::handle_wait(ProcessInfo.hProcess);
    }
    if ( (DllName = GetDllName(wil::GetModuleInstanceHandle()))
      && !_stricmp(pdli->szDll, DllName)
      && SUCCEEDED(wil::GetSystemDirectoryW(result)) ) {

      auto Path = fs::path(result);
      Path /= pdli->szDll;
      return (FARPROC)LoadLibraryExW(Path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
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
