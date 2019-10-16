#include "pch.h"
#include "dllname.h"
#include "modulever.h"
#include "hooks.hpp"

EXTERN_C BOOL APIENTRY DllMain(
    const HINSTANCE instance,
    const DWORD reason,
    const PVOID reserved)
{
    HMODULE ModuleHandle;
    PCNZWCH ProductName;
    PCNZWCH OriginalFilename;

    switch ( reason ) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(instance);
        if ( GetModuleVersionInfo(nullptr, L"\\StringFileInfo\\*\\ProductName", &(LPCVOID &)ProductName) >= 0
            && !wcscmp(ProductName, L"Blade & Soul")
            && GetModuleVersionInfo(nullptr, L"\\StringFileInfo\\*\\OriginalFilename", &(LPCVOID &)OriginalFilename) >= 0
            && !wcscmp(OriginalFilename, L"Client.exe") ) {

            auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
            auto msvc_logger = std::make_shared<spdlog::logger>("msvc_logger", sink);
            msvc_logger->set_level(spdlog::level::debug);
            msvc_logger->set_pattern("[%Y-%m-%d %T.%F] [%l] %@(%!): %v");
            spdlog::set_default_logger(msvc_logger);

            DetourTransactionBegin();
            DetourUpdateThread(GetCurrentThread());
            if ( ModuleHandle = GetModuleHandleW(L"ntdll.dll") ) {
                g_pfnNtQuerySystemInformation = (decltype(g_pfnNtQuerySystemInformation))GetProcAddress(ModuleHandle, "NtQuerySystemInformation");
                SPDLOG_DEBUG("NtQuerySystemInformation: {}", (PVOID)g_pfnNtQuerySystemInformation);
                if ( g_pfnNtQuerySystemInformation )
                    DetourAttach(&(PVOID &)g_pfnNtQuerySystemInformation, NtQuerySystemInformation_hook);

                g_pfnNtCreateMutant = (decltype(g_pfnNtCreateMutant))GetProcAddress(ModuleHandle, "NtCreateMutant");
                SPDLOG_DEBUG("NtCreateMutant: {}", (PVOID)g_pfnNtCreateMutant);
                if ( g_pfnNtCreateMutant )
                    DetourAttach(&(PVOID &)g_pfnNtCreateMutant, NtCreateMutant_hook);

                g_pfnNtCreateFile = (decltype(g_pfnNtCreateFile))GetProcAddress(ModuleHandle, "NtCreateFile");
                SPDLOG_DEBUG("NtCreateFile: {}", (PVOID)g_pfnNtCreateFile);
                if ( g_pfnNtCreateFile )
                    DetourAttach(&(PVOID &)g_pfnNtCreateFile, NtCreateFile_hook);

                g_pfnLdrLoadDll = (decltype(g_pfnLdrLoadDll))GetProcAddress(ModuleHandle, "LdrLoadDll");
                SPDLOG_DEBUG("LdrLoadDll: {}", (PVOID)g_pfnLdrLoadDll);
                if ( g_pfnLdrLoadDll )
                    DetourAttach(&(PVOID &)g_pfnLdrLoadDll, LdrLoadDll_hook);
            }
            if ( (ModuleHandle = GetModuleHandleW(L"win32u.dll")) || (ModuleHandle = GetModuleHandleW(L"user32.dll")) ) {
                g_pfnNtUserFindWindowEx = (decltype(g_pfnNtUserFindWindowEx))GetProcAddress(ModuleHandle, "NtUserFindWindowEx");
                SPDLOG_DEBUG("NtUserFindWindowEx: {}", (PVOID)g_pfnNtUserFindWindowEx);
                if ( g_pfnNtUserFindWindowEx ) {
                    DetourAttach(&(PVOID &)g_pfnNtUserFindWindowEx, NtUserFindWindowEx_hook);
                }
                DetourTransactionCommit();
                break;
            }
        }
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

EXTERN_C
const PfnDliHook __pfnDliNotifyHook2 = [](unsigned dliNotify, PDelayLoadInfo pdli) -> FARPROC
{
    PCSTR pszDllName;
    WCHAR wszPath[MAX_PATH];
    int Count;

    switch ( dliNotify ) {
    case dliStartProcessing:
        break;
    case dliNotePreLoadLibrary:
        pszDllName = GetDllName(&__ImageBase);
        if ( pszDllName && !_stricmp(pdli->szDll, pszDllName) ) {
            Count = GetSystemDirectoryW(wszPath, ARRAYSIZE(wszPath));
            if ( !Count || (size_t)Count + 1 > ARRAYSIZE(wszPath) )
                return nullptr;

            wszPath[Count++] = '\\';

            if ( MultiByteToWideChar(CP_ACP, 0, pdli->szDll, -1, wszPath + Count, ARRAYSIZE(wszPath) - Count) )
                return (FARPROC)LoadLibraryExW(wszPath, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
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
