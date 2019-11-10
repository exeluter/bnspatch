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
  UNICODE_STRING String = RTL_CONSTANT_STRING(L"kmon.dll");

  if ( RtlEqualUnicodeString(DllName, &String, TRUE) ) {
    DllHandle = nullptr;
    return STATUS_DLL_NOT_FOUND;
  }
  return g_pfnLdrLoadDll(DllPath, DllCharacteristics, DllName, DllHandle);
}

decltype(&LdrLoadDll) g_pfnLdrLoadDll;
NTSTATUS NTAPI LdrLoadDll_hook(
  PWSTR DllPath,
  PULONG DllCharacteristics,
  PUNICODE_STRING DllName,
  PVOID *DllHandle)
{
#ifdef _WIN64
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
  UNICODE_STRING DllName = RTL_CONSTANT_STRING(RtlNtdllName);
  PVOID DllHandle;
  ANSI_STRING ProcedureName;
  PVOID ProcedureAddress;

  if ( NewProtect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
    && NT_SUCCESS(LdrGetDllHandle((PWSTR)1, nullptr, &DllName, &DllHandle)) ) {
    for ( const auto &SourceString : { "DbgBreakPoint", "DbgUiRemoteBreakin" } ) {
      RtlInitAnsiString(&ProcedureName, SourceString);
      if ( NT_SUCCESS(LdrGetProcedureAddress(DllHandle, &ProcedureName, 0, &ProcedureAddress))
        && *BaseAddress == ProcedureAddress )
        return STATUS_INVALID_PARAMETER_2;
    }
  }
  return g_pfnNtProtectVirtualMemory(ProcessHandle, BaseAddress, RegionSize, NewProtect, OldProtect);
}

decltype(&NtQueryInformationProcess) g_pfnNtQueryInformationProcess;
NTSTATUS NTAPI NtQueryInformationProcess_hook(
  HANDLE ProcessHandle,
  PROCESSINFOCLASS ProcessInformationClass,
  PVOID ProcessInformation,
  ULONG ProcessInformationLength,
  PULONG ReturnLength)
{
  thread_local wdm::srw_exclusive_lock_t lock;
  std::unique_lock<wdm::srw_exclusive_lock_t> lock_guard(lock, std::try_to_lock);
  NTSTATUS Status;
  PROCESS_BASIC_INFORMATION ProcessInfo;
  ULONG Length;
  DWORD ProcessId;

  if ( lock_guard.owns_lock() ) {
    SPDLOG_INFO(fmt("{}, {}, {}, {}, {}"),
      ProcessHandle,
      magic_enum::enum_name(ProcessInformationClass),
      ProcessInformation,
      ProcessInformationLength,
      fmt::ptr(ReturnLength));

    if ( (ProcessInformationClass != ProcessBasicInformation && ProcessHandle == NtCurrentProcess())
      || (NT_SUCCESS(Status = g_pfnNtQueryInformationProcess(ProcessHandle, ProcessBasicInformation, &ProcessInfo, sizeof(PROCESS_BASIC_INFORMATION), &Length))
        && NtCurrentTeb()->ClientId.UniqueProcess == ProcessInfo.UniqueProcessId) ) {

      switch ( ProcessInformationClass ) {
      case ProcessBasicInformation:
        if ( ProcessInformationLength != sizeof(PROCESS_BASIC_INFORMATION) )
          return STATUS_INFO_LENGTH_MISMATCH;
        *(PPROCESS_BASIC_INFORMATION)ProcessInformation = ProcessInfo;
        GetWindowThreadProcessId(GetShellWindow(), &ProcessId);
        ((PPROCESS_BASIC_INFORMATION)ProcessInformation)->InheritedFromUniqueProcessId = ULongToHandle(ProcessId);
        if ( ReturnLength )
          *ReturnLength = Length;
        return Status;

      case ProcessDebugPort:
        if ( ProcessInformationLength != sizeof(DWORD_PTR) )
          return STATUS_INFO_LENGTH_MISMATCH;
        *(PDWORD_PTR)ProcessInformation = 0;
        if ( ReturnLength )
          *ReturnLength = sizeof(DWORD_PTR);
        return STATUS_SUCCESS;

      case ProcessDebugObjectHandle:
        if ( ProcessInformationLength != sizeof(HANDLE) )
          return STATUS_INFO_LENGTH_MISMATCH;
        *(PHANDLE)ProcessInformation = nullptr;
        if ( ReturnLength )
          *ReturnLength = sizeof(HANDLE);
        return STATUS_PORT_NOT_SET;
      }
    }
  }
  return g_pfnNtQueryInformationProcess(
    ProcessHandle,
    ProcessInformationClass,
    ProcessInformation,
    ProcessInformationLength,
    ReturnLength);
}

decltype(&NtQuerySystemInformation) g_pfnNtQuerySystemInformation;
NTSTATUS NTAPI NtQuerySystemInformation_hook(
  SYSTEM_INFORMATION_CLASS SystemInformationClass,
  PVOID SystemInformation,
  ULONG SystemInformationLength,
  PULONG ReturnLength)
{
  thread_local wdm::srw_exclusive_lock_t lock;
  std::unique_lock<wdm::srw_exclusive_lock_t> lock_guard(lock, std::try_to_lock);
  PSYSTEM_KERNEL_DEBUGGER_INFORMATION KernelDebuggerInformation;

  if ( lock_guard.owns_lock() ) {
    SPDLOG_INFO(fmt("{}, {}, {}, {}"),
      magic_enum::enum_name(SystemInformationClass),
      SystemInformation,
      SystemInformationLength,
      fmt::ptr(ReturnLength));

    switch ( SystemInformationClass ) {
    case SystemModuleInformation:
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
  }
  return g_pfnNtQuerySystemInformation(
    SystemInformationClass,
    SystemInformation,
    SystemInformationLength,
    ReturnLength);
}

decltype(&NtSetInformationThread) g_pfnNtSetInformationThread;
NTSTATUS NTAPI NtSetInformationThread_hook(
  HANDLE ThreadHandle,
  THREADINFOCLASS ThreadInformationClass,
  PVOID ThreadInformation,
  ULONG ThreadInformationLength)
{
  thread_local wdm::srw_exclusive_lock_t lock;
  std::unique_lock<wdm::srw_exclusive_lock_t> lock_guard(lock, std::try_to_lock);
  THREAD_BASIC_INFORMATION ThreadInfo;

  if ( lock_guard.owns_lock() ) {
    SPDLOG_INFO(fmt("{}, {}, {}, {}"),
      ThreadHandle,
      magic_enum::enum_name(ThreadInformationClass),
      ThreadInformation,
      ThreadInformationLength);

    switch ( ThreadInformationClass ) {
    case ThreadHideFromDebugger:
      if ( ThreadInformationLength != 0 )
        return STATUS_INFO_LENGTH_MISMATCH;
      if ( ThreadHandle == NtCurrentThread()
        || (NT_SUCCESS(NtQueryInformationThread(ThreadHandle, ThreadBasicInformation, &ThreadInfo, sizeof(THREAD_BASIC_INFORMATION), nullptr))
          && NtCurrentTeb()->ClientId.UniqueProcess == ThreadInfo.ClientId.UniqueProcess) )
        return STATUS_SUCCESS;
      break;
    }
  }
  return g_pfnNtSetInformationThread(
    ThreadHandle,
    ThreadInformationClass,
    ThreadInformation,
    ThreadInformationLength);
}

HWND(WINAPI *g_pfnNtUserFindWindowEx)(HWND, HWND, PUNICODE_STRING, PUNICODE_STRING, DWORD);
HWND WINAPI NtUserFindWindowEx_hook(
  HWND hwndParent,
  HWND hwndChild,
  PUNICODE_STRING pstrClassName,
  PUNICODE_STRING pstrWindowName,
  DWORD dwType)
{
  thread_local wdm::srw_exclusive_lock_t lock;
  std::unique_lock<wdm::srw_exclusive_lock_t> lock_guard(lock, std::try_to_lock);
  UNICODE_STRING DestinationString;

  if ( lock_guard.owns_lock() ) {
    SPDLOG_INFO(fmt(L"{}, {}, {}, {}, {}"),
      fmt::ptr(hwndParent),
      fmt::ptr(hwndChild),
      cvt::to_wstring_view(pstrClassName),
      cvt::to_wstring_view(pstrWindowName),
      dwType);

    if ( pstrClassName ) {
      for ( const auto &SourceString : {
        L"OLLYDBG", L"GBDYLLO", L"pediy06",
        L"FilemonClass", L"PROCMON_WINDOW_CLASS", L"RegmonClass", L"18467-41" } ) {
        RtlInitUnicodeString(&DestinationString, SourceString);
        if ( RtlEqualUnicodeString(pstrClassName, &DestinationString, FALSE) )
          return nullptr;
      }
    }
    if ( pstrWindowName ) {
      for ( const auto &SourceString : {
        L"File Monitor - Sysinternals: www.sysinternals.com",
        L"Process Monitor - Sysinternals: www.sysinternals.com",
        L"Registry Monitor - Sysinternals: www.sysinternals.com" } ) {
        RtlInitUnicodeString(&DestinationString, SourceString);
        if ( RtlEqualUnicodeString(pstrWindowName, &DestinationString, FALSE) )
          return nullptr;
      }
    }
  }
  return g_pfnNtUserFindWindowEx(hwndParent, hwndChild, pstrClassName, pstrWindowName, dwType);
}
