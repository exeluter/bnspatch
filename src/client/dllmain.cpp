#include "pch.h"
//#include "modulever.h"
#include "hooks.h"
#include "pe/module.h"
#include "pe/export_directory.h"
#include "ntapi.h"

template <size_t Size>
inline void AttachMultipleDetours(
  const wchar_t *module_name,
  const std::array<std::tuple<const char *, void **, void *>, Size> &detours)
{
  if ( const auto module = pe::get_module(module_name) ) {
    for ( const auto &det : detours ) {
      if ( *std::get<1>(det) = module->find_function(std::get<0>(det)) )
        DetourAttach(std::get<1>(det), std::get<2>(det));
    }
  }
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  switch ( fdwReason ) {
    case DLL_PROCESS_ATTACH: {
      if ( pe::get_module()->base_name() == L"Client.exe" ) {
        NtCurrentPeb()->BeingDebugged = FALSE;

        auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        auto logger = std::make_shared<spdlog::logger>("msvc_logger", sink);
        if ( GetAsyncKeyState(VK_CONTROL) & 0x8000 )
          logger->set_level(spdlog::level::debug);
        logger->set_pattern("[%l] %!: %v");
        spdlog::set_default_logger(logger);

        // Register Dll Notification
        if ( const auto module = pe::get_module(L"ntdll.dll") ) {
          if ( const auto pfn = reinterpret_cast<decltype(&LdrRegisterDllNotification)>(
            module->find_function("LdrRegisterDllNotification")) ) {
            pfn(0, &DllNotification, nullptr, &g_pvDllNotificationCookie);
          }
        }

        // Attach Detours
        DetourTransactionBegin();
        DetourUpdateThread(NtCurrentThread());
        AttachMultipleDetours(L"ntdll.dll",
          std::array {
            std::make_tuple("LdrLoadDll", &(void *&)g_pfnLdrLoadDll, (void *)&LdrLoadDll_hook),
            std::make_tuple("NtCreateFile", &(void *&)g_pfnNtCreateFile, (void *)&NtCreateFile_hook),
            std::make_tuple("NtCreateMutant", &(void *&)g_pfnNtCreateMutant, (void *)&NtCreateMutant_hook),
            std::make_tuple("NtProtectVirtualMemory", &(void *&)g_pfnNtProtectVirtualMemory, (void *)&NtProtectVirtualMemory_hook),
            std::make_tuple("NtQueryInformationProcess", &(void *&)g_pfnNtQueryInformationProcess, (void *)&NtQueryInformationProcess_hook),
            std::make_tuple("NtQuerySystemInformation", &(void *&)g_pfnNtQuerySystemInformation, (void *)&NtQuerySystemInformation_hook)
          });
        AttachMultipleDetours(L"user32.dll",
          std::array {
            std::make_tuple("FindWindowA", &(PVOID &)g_pfnFindWindowA, (PVOID)&FindWindowA_hook)
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
    case dliNotePreLoadLibrary:
      if ( !_stricmp(pe::get_instance_module()->export_directory()->name(), pdli->szDll)
        && GetSystemDirectoryW(buffer.data(), SafeInt(buffer.size())) ) {

        auto path = fs::path(buffer.begin(), buffer.end());
        path /= pdli->szDll;
        return reinterpret_cast<FARPROC>(
          LoadLibraryExW(path.c_str(), nullptr, LOAD_WITH_ALTERED_SEARCH_PATH));
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
