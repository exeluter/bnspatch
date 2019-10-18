#include "pch.h"
#include "..\common\dllname.h"
#include "..\common\modulever.h"
#include "hooks.hpp"

extern "C" BOOL WINAPI DllMain(
    HINSTANCE hInstance,
    DWORD     fdwReason,
    LPVOID    lpvReserved)
{
    HMODULE ModuleHandle;
    PCNZWCH ProductName;
    PCNZWCH OriginalFilename;

    switch ( fdwReason ) {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(hInstance);
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

ExternC
const PfnDliHook __pfnDliNotifyHook2 = [](
    unsigned       dliNotify,
    PDelayLoadInfo pdli) -> FARPROC
{
    PCSTR DllName;
    std::array<WCHAR, MAX_PATH> Buffer;

    switch ( dliNotify ) {
    case dliStartProcessing:
        break;
    case dliNotePreLoadLibrary:
        DllName = GetDllName(&__ImageBase);
        if ( DllName && !_stricmp(pdli->szDll, DllName)
            && GetSystemDirectoryW(Buffer.data(), (UINT)Buffer.size()) ) {

            auto Path = fs::path(Buffer.data());
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
