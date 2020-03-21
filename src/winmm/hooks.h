#pragma once
#include <ntdll.h>
#include <vector>
#include "pe/module.h"

void prevent_tmd_apiw(pe::module *module);

extern PVOID g_pvDllNotificationCookie;
VOID CALLBACK DllNotification(
  ULONG NotificationReason,
  PCLDR_DLL_NOTIFICATION_DATA NotificationData,
  PVOID Context);

extern decltype(&LdrGetDllHandle) g_pfnLdrGetDllHandle;
NTSTATUS NTAPI LdrGetDllHandle_hook(
  PWSTR DllPath,
  PULONG DllCharacteristics,
  PUNICODE_STRING DllName,
  PVOID *DllHandle);

extern decltype(&LdrLoadDll) g_pfnLdrLoadDll;
NTSTATUS NTAPI LdrLoadDll_hook(
  PWSTR DllPath,
  PULONG DllCharacteristics,
  PUNICODE_STRING DllName,
  PVOID *DllHandle);

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

extern decltype(&NtOpenKeyEx) g_pfnNtOpenKeyEx;
NTSTATUS NTAPI NtOpenKeyEx_hook(
  PHANDLE KeyHandle,
  ACCESS_MASK DesiredAccess,
  POBJECT_ATTRIBUTES ObjectAttributes,
  ULONG OpenOptions);

extern std::vector<void const *> g_ReadOnlyAddresses;
extern decltype(&NtProtectVirtualMemory) g_pfnNtProtectVirtualMemory;
NTSTATUS NTAPI NtProtectVirtualMemory_hook(
  HANDLE ProcessHandle,
  PVOID *BaseAddress,
  PSIZE_T RegionSize,
  ULONG NewProtect,
  PULONG OldProtect);

extern decltype(&NtQueryInformationProcess) g_pfnNtQueryInformationProcess;
NTSTATUS NTAPI NtQueryInformationProcess_hook(
  HANDLE ProcessHandle,
  PROCESSINFOCLASS ProcessInformationClass,
  PVOID ProcessInformation,
  ULONG ProcessInformationLength,
  PULONG ReturnLength);

extern decltype(&NtQuerySystemInformation) g_pfnNtQuerySystemInformation;
NTSTATUS NTAPI NtQuerySystemInformation_hook(
  SYSTEM_INFORMATION_CLASS SystemInformationClass,
  PVOID SystemInformation,
  ULONG SystemInformationLength,
  PULONG ReturnLength);

extern decltype(&NtSetInformationThread) g_pfnNtSetInformationThread;
NTSTATUS NTAPI NtSetInformationThread_hook(
  HANDLE ThreadHandle,
  THREADINFOCLASS ThreadInformationClass,
  PVOID ThreadInformation,
  ULONG ThreadInformationLength);

extern decltype(&FindWindowA) g_pfnFindWindowA;
HWND WINAPI FindWindowA_hook(
  LPCSTR lpClassName,
  LPCSTR lpWindowName);
