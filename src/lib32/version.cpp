#define DLLEXPORT DLLEXPORT
#include <Windows.h>

EXTERN_C_START

DLLEXPORT DWORD APIENTRY VerFindFileA(
    DWORD uFlags,
    LPCSTR szFileName,
    LPCSTR szWinDir,
    LPCSTR szAppDir,
    LPSTR szCurDir,
    PUINT puCurDirLen,
    LPSTR szDestDir,
    PUINT puDestDirLen)
{
    return 0;
}

DLLEXPORT DWORD APIENTRY VerFindFileW(
    DWORD uFlags,
    LPCWSTR szFileName,
    LPCWSTR szWinDir,
    LPCWSTR szAppDir,
    LPWSTR szCurDir,
    PUINT puCurDirLen,
    LPWSTR szDestDir,
    PUINT puDestDirLen)
{
    return 0;
}

DLLEXPORT DWORD APIENTRY VerInstallFileA(
    DWORD uFlags,
    LPCSTR szSrcFileName,
    LPCSTR szDestFileName,
    LPCSTR szSrcDir,
    LPCSTR szDestDir,
    LPCSTR szCurDir,
    LPSTR szTmpFile,
    PUINT puTmpFileLen)
{
    return 0;
}

DLLEXPORT DWORD APIENTRY VerInstallFileW(
    DWORD uFlags,
    LPCWSTR szSrcFileName,
    LPCWSTR szDestFileName,
    LPCWSTR szSrcDir,
    LPCWSTR szDestDir,
    LPCWSTR szCurDir,
    LPWSTR szTmpFile,
    PUINT puTmpFileLen)
{
    return 0;
}

DLLEXPORT DWORD APIENTRY GetFileVersionInfoSizeA(
    LPCSTR lptstrFilename,
    LPDWORD lpdwHandle)
{
    return 0;
}

DLLEXPORT DWORD APIENTRY GetFileVersionInfoSizeW(
    LPCWSTR lptstrFilename,
    LPDWORD lpdwHandle)
{
    return 0;
}

DLLEXPORT BOOL APIENTRY GetFileVersionInfoA(
    LPCSTR lptstrFilename,
    DWORD dwHandle,
    DWORD dwLen,
    LPVOID lpData)
{
    return FALSE;
}

DLLEXPORT BOOL APIENTRY GetFileVersionInfoW(
    LPCWSTR lptstrFilename,
    DWORD dwHandle,
    DWORD dwLen,
    LPVOID lpData)
{
    return FALSE;
}

DLLEXPORT DWORD APIENTRY GetFileVersionInfoSizeExA(
    DWORD dwFlags,
    LPCSTR lpwstrFilename,
    LPDWORD lpdwHandle)
{
    return 0;
}

DLLEXPORT DWORD APIENTRY GetFileVersionInfoSizeExW(
    DWORD dwFlags,
    LPCWSTR lpwstrFilename,
    LPDWORD lpdwHandle)
{
    return 0;
}

DLLEXPORT BOOL APIENTRY GetFileVersionInfoExA(
    DWORD dwFlags,
    LPCSTR lpwstrFilename,
    DWORD dwHandle,
    DWORD dwLen,
    LPVOID lpData)
{
    return FALSE;
}

DLLEXPORT BOOL APIENTRY GetFileVersionInfoExW(
    DWORD dwFlags,
    LPCWSTR lpwstrFilename,
    DWORD dwHandle,
    DWORD dwLen,
    LPVOID lpData)
{
    return FALSE;
}

DLLEXPORT BOOL APIENTRY GetFileVersionInfoByHandle(
    DWORD dwFlags,
    HANDLE hFile,
    LPVOID *lplpBuffer,
    PUINT puLen)
{
    return FALSE;
}

DLLEXPORT DWORD APIENTRY VerLanguageNameA(
    DWORD wLang,
    LPSTR szLang,
    DWORD cchLang)
{
    return 0;
}

DLLEXPORT DWORD APIENTRY VerLanguageNameW(
    DWORD wLang,
    LPWSTR szLang,
    DWORD cchLang)
{
    return 0;
}

DLLEXPORT BOOL APIENTRY VerQueryValueA(
    LPCVOID pBlock,
    LPCSTR lpSubBlock,
    LPVOID *lplpBuffer,
    PUINT puLen)
{
    return FALSE;
}

DLLEXPORT BOOL APIENTRY VerQueryValueW(
    LPCVOID pBlock,
    LPCWSTR lpSubBlock,
    LPVOID *lplpBuffer,
    PUINT puLen)
{
    return FALSE;
}

EXTERN_C_END
