#include "pch.h"
#include "scl.h"

#ifdef _M_X64
# define SCYLLAHIDE_INJECTOR_CLI L"InjectorCLIx64.exe"
# define SCYLLAHIDE_HOOK_LIBRARY L"HookLibraryx64.dll"
#else
# define SCYLLAHIDE_INJECTOR_CLI L"InjectorCLIx86.exe"
# define SCYLLAHIDE_HOOK_LIBRARY L"HookLibraryx86.dll"
#endif

bool scl::RunInjectorCLI(DWORD dwProcessId)
{
  auto str = fmt::format(
    fmt(L"\"{}\" pid:{:#x} \"{}\" nowait"),
    SCYLLAHIDE_INJECTOR_CLI,
    dwProcessId,
    SCYLLAHIDE_HOOK_LIBRARY);

  STARTUPINFO StartupInfo = { sizeof(STARTUPINFO) };
  wil::unique_process_information ProcessInfo;
  return CreateProcessW(nullptr, str.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &StartupInfo, &ProcessInfo)
    && wil::handle_wait(ProcessInfo.hProcess);
}
