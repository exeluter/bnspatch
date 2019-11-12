#include "pch.h"
#include "hooks.h"
#include "srw_exclusive_lock.h"
#include "dllname.h"
#include "cvt.h"

decltype(&LdrGetDllHandle) g_pfnLdrGetDllHandle;
NTSTATUS NTAPI LdrGetDllHandle_hook(
  PWSTR DllPath,
  PULONG DllCharacteristics,
  PUNICODE_STRING DllName,
  PVOID *DllHandle)
{
#ifdef _M_IX86
  UNICODE_STRING String = RTL_CONSTANT_STRING(L"kmon.dll");

  if ( RtlEqualUnicodeString(DllName, &String, TRUE) ) {
    DllHandle = nullptr;
    return STATUS_DLL_NOT_FOUND;
  }
#endif
  return g_pfnLdrLoadDll(DllPath, DllCharacteristics, DllName, DllHandle);
}

decltype(&LdrLoadDll) g_pfnLdrLoadDll;
NTSTATUS NTAPI LdrLoadDll_hook(
  PWSTR DllPath,
  PULONG DllCharacteristics,
  PUNICODE_STRING DllName,
  PVOID *DllHandle)
{
#ifdef _M_X64
  UNICODE_STRING String = RTL_CONSTANT_STRING(L"aegisty64.bin");
#else
  UNICODE_STRING String = RTL_CONSTANT_STRING(L"aegisty.bin");
#endif

  if ( RtlEqualUnicodeString(DllName, &String, TRUE) ) {
    *DllHandle = nullptr;
    return STATUS_DLL_NOT_FOUND;
  }
  return g_pfnLdrLoadDll(DllPath, DllCharacteristics, DllName, DllHandle);;
}

decltype(&NtCreateFile) g_pfnNtCreateFile;
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
  ULONG EaLength)
{
  return g_pfnNtCreateFile(
    FileHandle,
    DesiredAccess,
    ObjectAttributes,
    IoStatusBlock,
    AllocationSize,
    FileAttributes,
    ShareAccess ? ShareAccess : FILE_SHARE_READ,
    CreateDisposition,
    CreateOptions,
    EaBuffer,
    EaLength);
}

decltype(&NtCreateMutant) g_pfnNtCreateMutant;
NTSTATUS NTAPI NtCreateMutant_hook(
  PHANDLE MutantHandle,
  ACCESS_MASK DesiredAccess,
  POBJECT_ATTRIBUTES ObjectAttributes,
  BOOLEAN InitialOwner)
{
  UNICODE_STRING String = RTL_CONSTANT_STRING(L"BnSGameClient");

  if ( ObjectAttributes && ObjectAttributes->ObjectName
    && RtlEqualUnicodeString(ObjectAttributes->ObjectName, &String, FALSE) ) {
    ObjectAttributes->ObjectName = nullptr;
    ObjectAttributes->Attributes &= ~OBJ_OPENIF;
    ObjectAttributes->RootDirectory = nullptr;
  }
  return g_pfnNtCreateMutant(MutantHandle, DesiredAccess, ObjectAttributes, InitialOwner);
}

decltype(&NtProtectVirtualMemory) g_pfnNtProtectVirtualMemory;
NTSTATUS NTAPI NtProtectVirtualMemory_hook(
  HANDLE ProcessHandle,
  PVOID *BaseAddress,
  PSIZE_T RegionSize,
  ULONG NewProtect,
  PULONG OldProtect)
{
  PROCESS_BASIC_INFORMATION ProcessInfo;
  UNICODE_STRING DllName = RTL_CONSTANT_STRING(RtlNtdllName);
  PVOID DllHandle;
  SYSTEM_BASIC_INFORMATION SystemInfo;
  ULONG_PTR StartingAddress;
  PVOID ProcedureAddress;

  if ( NewProtect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
    && (ProcessHandle == NtCurrentProcess()
      || (NT_SUCCESS(NtQueryInformationProcess(ProcessHandle, ProcessBasicInformation, &ProcessInfo, sizeof(PROCESS_BASIC_INFORMATION), nullptr))
        && ProcessInfo.UniqueProcessId == NtCurrentTeb()->ClientId.UniqueProcess))
    && NT_SUCCESS(g_pfnLdrGetDllHandle(nullptr, nullptr, &DllName, &DllHandle))
    && NT_SUCCESS(g_pfnNtQuerySystemInformation(SystemBasicInformation, &SystemInfo, sizeof(SYSTEM_BASIC_INFORMATION), nullptr)) ) {

    __try {
      StartingAddress = (ULONG_PTR)*BaseAddress & ~((ULONG_PTR)SystemInfo.PageSize - 1);
    } __except ( EXCEPTION_EXECUTE_HANDLER ) {
      return GetExceptionCode();
    }

    for ( auto &ProcedureName : std::array {
      ANSI_STRING RTL_CONSTANT_STRING("DbgBreakPoint"),
      ANSI_STRING RTL_CONSTANT_STRING("DbgUiRemoteBreakin") } ) {
      if ( NT_SUCCESS(LdrGetProcedureAddress(DllHandle, &ProcedureName, 0, &ProcedureAddress))
        && StartingAddress == ((ULONG_PTR)ProcedureAddress & ~((ULONG_PTR)SystemInfo.PageSize - 1)) )
        return STATUS_INVALID_PARAMETER_2;
    }
  }
  return g_pfnNtProtectVirtualMemory(ProcessHandle, BaseAddress, RegionSize, NewProtect, OldProtect);
}

decltype(&NtQuerySystemInformation) g_pfnNtQuerySystemInformation;
NTSTATUS NTAPI NtQuerySystemInformation_hook(
  SYSTEM_INFORMATION_CLASS SystemInformationClass,
  PVOID SystemInformation,
  ULONG SystemInformationLength,
  PULONG ReturnLength)
{
  PSYSTEM_KERNEL_DEBUGGER_INFORMATION KernelDebuggerInformation;

  switch ( SystemInformationClass ) {
  case SystemProcessInformation:
  case SystemModuleInformation:
  case SystemModuleInformationEx:
    return STATUS_ACCESS_DENIED;

  case SystemKernelDebuggerInformation:
    if ( SystemInformationLength < sizeof(SYSTEM_KERNEL_DEBUGGER_INFORMATION) )
      return STATUS_INFO_LENGTH_MISMATCH;
    KernelDebuggerInformation =
      (PSYSTEM_KERNEL_DEBUGGER_INFORMATION)SystemInformation;
    KernelDebuggerInformation->KernelDebuggerEnabled = FALSE;
    KernelDebuggerInformation->KernelDebuggerNotPresent = TRUE;

    if ( ReturnLength )
      *ReturnLength = sizeof(SYSTEM_KERNEL_DEBUGGER_INFORMATION);
    return STATUS_SUCCESS;
  }
  return g_pfnNtQuerySystemInformation(
    SystemInformationClass,
    SystemInformation,
    SystemInformationLength,
    ReturnLength);
}

decltype(&FindWindowA) g_pfnFindWindowA;
HWND WINAPI FindWindowA_hook(
  LPCSTR lpClassName,
  LPCSTR lpWindowName)
{
  if ( lpClassName ) {
    for ( const auto &String : {
#ifdef _M_IX86
      "OLLYDBG", "GBDYLLO", "pediy06",
#endif
      "FilemonClass", "PROCMON_WINDOW_CLASS", "RegmonClass", "18467-41" } ) {
      if ( !_stricmp(lpClassName, String) )
        return nullptr;
    }
  }
  if ( lpWindowName ) {
    for ( const auto &String : {
      "File Monitor - Sysinternals: www.sysinternals.com",
      "Process Monitor - Sysinternals: www.sysinternals.com",
      "Registry Monitor - Sysinternals: www.sysinternals.com" } ) {
      if ( !strcmp(lpWindowName, String) )
        return nullptr;
    }
  }
  return g_pfnFindWindowA(lpClassName, lpWindowName);
}
