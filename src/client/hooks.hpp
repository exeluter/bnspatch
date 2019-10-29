#pragma once

HWND WINAPI NtUserFindWindowEx(
    HWND hwndParent,
    HWND hwndChild,
    PUNICODE_STRING pstrClassName,
    PUNICODE_STRING pstrWindowName,
    DWORD dwType);

#ifdef _WIN64
extern decltype(&NtQueryInformationProcess) g_pfnNtQueryInformationProcess;
NTSTATUS NTAPI NtQueryInformationProcess_hook(
    HANDLE ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength);
#endif

extern decltype(&NtQuerySystemInformation) g_pfnNtQuerySystemInformation;
NTSTATUS NTAPI NtQuerySystemInformation_hook(
    SYSTEM_INFORMATION_CLASS SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength);

extern decltype(&NtCreateFile) g_pfnNtCreateFile;
NTSTATUS NTAPI NtCreateFile_hook(
    PHANDLE FileHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    PIO_STATUS_BLOCK IoStatusBlock,
    PLARGE_INTEGER AllocationSize,
    ULONG FileAttributes,
    ULONG ShareAccess,
    ULONG CreateDisposition,
    ULONG CreateOptions,
    PVOID EaBuffer,
    ULONG EaLength);

extern decltype(&NtCreateMutant) g_pfnNtCreateMutant;
NTSTATUS NTAPI NtCreateMutant_hook(
    PHANDLE MutantHandle,
    ACCESS_MASK DesiredAccess,
    POBJECT_ATTRIBUTES ObjectAttributes,
    BOOLEAN InitialOwner);

extern decltype(&LdrLoadDll) g_pfnLdrLoadDll;
NTSTATUS NTAPI LdrLoadDll_hook(
    PWSTR DllPath,
    PULONG DllCharacteristics,
    PUNICODE_STRING DllName,
    PVOID *DllHandle);

extern decltype(&LdrGetDllHandle) g_pfnLdrGetDllHandle;
NTSTATUS NTAPI LdrGetDllHandle_hook(
    PWSTR DllPath,
    PULONG DllCharacteristics,
    PUNICODE_STRING DllName,
    PVOID *DllHandle);

extern decltype(&NtUserFindWindowEx) g_pfnNtUserFindWindowEx;
HWND WINAPI NtUserFindWindowEx_hook(
    HWND hwndParent,
    HWND hwndChild,
    PUNICODE_STRING pstrClassName,
    PUNICODE_STRING pstrWindowName,
    DWORD dwType);
