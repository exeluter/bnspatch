#include "pch.h"
#include "hooks.h"
#include "pluginsdk.h"

BOOL APIENTRY DllMain(HMODULE hModule,
  DWORD ul_reason_for_call,
  LPVOID lpReserved)
{
  if ( ul_reason_for_call == DLL_PROCESS_ATTACH ) {
    const auto resInfo = FindResourceW(nullptr, MAKEINTRESOURCEW(VS_VERSION_INFO), VS_FILE_INFO);
    if ( !resInfo ) return FALSE;

    const auto count = SizeofResource(nullptr, resInfo);
    if ( !count ) return FALSE;

    const auto ptr = LoadResource(nullptr, resInfo);
    if ( !ptr ) return FALSE;

    const std::span<UCHAR> res{reinterpret_cast<PUCHAR>(ptr), count};
    const std::vector<UCHAR> block{res.begin(), res.end()};

    LPVOID buffer;
    UINT len;
    if ( !VerQueryValueW(block.data(), L"\\StringFileInfo\\040904b0\\OriginalFilename", &buffer, &len) )
      return FALSE;

    switch ( fnv1a::make_hash(reinterpret_cast<PCWSTR>(buffer), len - 1) ) {
      case L"Client.exe"_fnv1a:
      case L"BNSR.exe"_fnv1a:
        DisableThreadLibraryCalls(hModule);
        break;
      default:
        return FALSE;
    }
  }
  return TRUE;
}

VOID __cdecl Init(VOID)
{
  NtCurrentPeb()->BeingDebugged = FALSE;

  wil::unique_handle tokenHandle;
  THROW_IF_WIN32_BOOL_FALSE(OpenProcessToken(NtCurrentProcess(), TOKEN_WRITE, &tokenHandle));
  ULONG virtualizationEnabled = TRUE;
  THROW_IF_WIN32_BOOL_FALSE(SetTokenInformation(tokenHandle.get(), TokenVirtualizationEnabled, &virtualizationEnabled, sizeof(ULONG)));

  THROW_IF_WIN32_ERROR(DetourTransactionBegin());
  THROW_IF_WIN32_ERROR(DetourUpdateThread(NtCurrentThread()));

  const auto hNtDll = GetModuleHandleW(L"ntdll.dll");
  THROW_LAST_ERROR_IF_NULL(hNtDll);
#ifdef _X86_
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "LdrGetDllHandle", &g_pfnLdrGetDllHandle, &LdrGetDllHandle_hook));
#endif
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "LdrLoadDll", &g_pfnLdrLoadDll, LdrLoadDll_hook));
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "NtCreateFile", &g_pfnNtCreateFile, NtCreateFile_hook));
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "NtCreateMutant", &g_pfnNtCreateMutant, NtCreateMutant_hook));
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "NtOpenKeyEx", &g_pfnNtOpenKeyEx, NtOpenKeyEx_hook));
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "NtProtectVirtualMemory", &g_pfnNtProtectVirtualMemory, NtProtectVirtualMemory_hook));
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "NtQuerySystemInformation", &g_pfnNtQuerySystemInformation, NtQuerySystemInformation_hook));
#ifdef _AMD64_
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "NtQueryInformationProcess", &g_pfnNtQueryInformationProcess, NtQueryInformationProcess_hook));
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "NtSetInformationThread", &g_pfnNtSetInformationThread, NtSetInformationThread_hook));
  THROW_IF_WIN32_ERROR(DetourAttach(hNtDll, "NtGetContextThread", &g_pfnNtGetContextThread, NtGetContextThread_hook));
#endif

  THROW_IF_WIN32_ERROR(DetourAttach(L"kernel32.dll", "GetPrivateProfileStringW", &g_pfnGetPrivateProfileStringW, GetPrivateProfileStringW_hook));
  THROW_IF_WIN32_ERROR(DetourAttach(L"user32.dll", "FindWindowA", &g_pfnFindWindowA, FindWindowA_hook));

  THROW_IF_WIN32_ERROR(DetourTransactionCommit());
}

__declspec(dllexport)
void __cdecl GetPluginInfo(PluginInfo *plgi)
{
  plgi->Init = &Init;
}
