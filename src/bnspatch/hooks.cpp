#include "hooks.h"

#include <phnt_windows.h>
#include <phnt.h>
#include <ShlObj.h>
#include <KnownFolders.h>

#include <array>
#include <codecvt>
#include <filesystem>
#include <mutex>
#include <optional>
#include <queue>

#include "ntapi/mprotect.h"
#include "ntapi/string.h"
#include "versioninfo.h"
#include "xmlhooks.h"
#include "xmlpatch.h"
#include <detours.h>
#include <fastwildcompare.hpp>
#include <fmt/format.h>
#include <fnv1a.h>
#include <gsl/span_ext>
#include <gsl/span>
#include <pe/export_directory.h>
#include <pe/module.h>
#include <pugixml.hpp>
#include <SafeInt.hpp>
#include <thread_local_lock.h>
#include <wil/stl.h>
#include <wil/win32_helpers.h>
#include <xorstr.hpp>

PVOID g_pvDllNotificationCookie;
VOID CALLBACK DllNotification(
  ULONG NotificationReason,
  PLDR_DLL_NOTIFICATION_DATA NotificationData,
  PVOID Context)
{
  static thread_local_lock mtx;
  std::unique_lock lock(mtx, std::try_to_lock);
  if ( !lock.owns_lock() )
    return;

  switch ( NotificationReason ) {
    case LDR_DLL_NOTIFICATION_REASON_LOADED:
    {
      const auto module = static_cast<pe::module *>(NotificationData->Loaded.DllBase);
      const wchar_t *fileName;

      if ( GetModuleVersionInfo(module, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)fileName) >= 0 ) {
        switch ( fnv1a::make_hash(fileName, towupper) ) {
          case L"XmlReader.dll"_fnv1au: {
            auto const pfnGetInterfaceVersion = reinterpret_cast<wchar_t const *(*)()>(module->function(xorstr_("GetInterfaceVersion")));
            auto const pfnCreateXmlReader = reinterpret_cast<void *(*)()>(module->function(xorstr_("CreateXmlReader")));
            auto const pfnDestroyXmlReader = reinterpret_cast<void *(*)(void *)>(module->function(xorstr_("DestroyXmlReader")));
            if ( pfnGetInterfaceVersion && pfnCreateXmlReader && pfnDestroyXmlReader ) {
              DetourTransactionBegin();
              DetourUpdateThread(NtCurrentThread());
              auto xmlReader = pfnCreateXmlReader();
              auto vfptr = *reinterpret_cast<void ***>(xmlReader);
              switch ( _wtoi(pfnGetInterfaceVersion()) ) {
                case 13:
                  g_pfnReadFromFile13 = reinterpret_cast<decltype(g_pfnReadFromFile13)>(vfptr[6]);
                  DetourAttach(&(PVOID &)g_pfnReadFromFile13, ReadFromFile13_hook);
                  g_pfnReadFromBuffer13 = reinterpret_cast<decltype(g_pfnReadFromBuffer13)>(vfptr[7]);
                  DetourAttach(&(PVOID &)g_pfnReadFromBuffer13, ReadFromBuffer13_hook);
                  break;
                case 14:
                  g_pfnReadFromFile14 = reinterpret_cast<decltype(g_pfnReadFromFile14)>(vfptr[6]);
                  DetourAttach(&(PVOID &)g_pfnReadFromFile14, ReadFromFile14_hook);
                  g_pfnReadFromBuffer14 = reinterpret_cast<decltype(g_pfnReadFromBuffer14)>(vfptr[7]);
                  DetourAttach(&(PVOID &)g_pfnReadFromBuffer14, ReadFromBuffer14_hook);
                  break;
                case 15:
                  g_pfnReadFromFile15 = reinterpret_cast<decltype(g_pfnReadFromFile15)>(vfptr[6]);
                  DetourAttach(&(PVOID &)g_pfnReadFromFile15, ReadFromFile15_hook);
                  g_pfnReadFromBuffer15 = reinterpret_cast<decltype(g_pfnReadFromBuffer15)>(vfptr[7]);
                  DetourAttach(&(PVOID &)g_pfnReadFromBuffer15, ReadFromBuffer15_hook);
                  break;
              }
              pfnDestroyXmlReader(xmlReader);
              DetourTransactionCommit();
            }
          }
        }
      }
      break;
    }
  }
}

#ifdef _M_IX86
decltype(&LdrGetDllHandle) g_pfnLdrGetDllHandle;
NTSTATUS NTAPI LdrGetDllHandle_hook(
  PWSTR DllPath,
  PULONG DllCharacteristics,
  PUNICODE_STRING DllName,
  PVOID *DllHandle)
{
  static thread_local_lock mtx;
  std::unique_lock lock(mtx, std::try_to_lock);

  if ( lock.owns_lock()
    && static_cast<ntapi::ustring *>(DllName)->iequals(xorstr_(L"kmon.dll")) ) {
    DllHandle = nullptr;
    return STATUS_DLL_NOT_FOUND;
  }
  return g_pfnLdrLoadDll(DllPath, DllCharacteristics, DllName, DllHandle);
}
#endif

decltype(&LdrLoadDll) g_pfnLdrLoadDll;
NTSTATUS NTAPI LdrLoadDll_hook(
  PWSTR DllPath,
  PULONG DllCharacteristics,
  PUNICODE_STRING DllName,
  PVOID *DllHandle)
{
  static thread_local_lock mtx;
  std::unique_lock lock(mtx, std::try_to_lock);

  if ( lock.owns_lock() ) {
    if ( static_cast<ntapi::ustring *>(DllName)->istarts_with(xorstr_(L"aegisty")) ) {
      *DllHandle = nullptr;
      return STATUS_DLL_NOT_FOUND;
    }
  }
  return g_pfnLdrLoadDll(DllPath, DllCharacteristics, DllName, DllHandle);
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
#ifdef _M_IX86
  if ( auto const ObjectName = static_cast<ntapi::ustring *>(ObjectAttributes->ObjectName) ) {
    switch ( fnv1a::make_hash(ObjectName->data(), ObjectName->size(), towupper) ) {
      case L"\\\\.\\SICE"_fnv1au:
      case L"\\\\.\\SIWVID"_fnv1au:
      case L"\\\\.\\NTICE"_fnv1au:
        return STATUS_OBJECT_NAME_NOT_FOUND;
    }
  }
#endif
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
  if ( ObjectAttributes && ObjectAttributes->ObjectName ) {
    const auto objectName = static_cast<ntapi::ustring *>(ObjectAttributes->ObjectName);
    if ( objectName->starts_with(xorstr_(L"BnSGameClient"))
      || objectName->starts_with(xorstr_(L"Global\\MtxNPG")) ) {
      ObjectAttributes->ObjectName = nullptr;
      ObjectAttributes->Attributes &= ~OBJ_OPENIF;
      ObjectAttributes->RootDirectory = nullptr;
    }
  }
  return g_pfnNtCreateMutant(MutantHandle, DesiredAccess, ObjectAttributes, InitialOwner);
}

decltype(&NtOpenKeyEx) g_pfnNtOpenKeyEx;
NTSTATUS NTAPI NtOpenKeyEx_hook(
  PHANDLE KeyHandle,
  ACCESS_MASK DesiredAccess,
  POBJECT_ATTRIBUTES ObjectAttributes,
  ULONG OpenOptions)
{
  if ( auto const ObjectName = static_cast<ntapi::ustring *>(ObjectAttributes->ObjectName) ) {
    switch ( fnv1a::make_hash(ObjectName->data(), ObjectName->size(), towupper) ) {
      case L"Software\\Wine"_fnv1au:
      case L"HARDWARE\\ACPI\\DSDT\\VBOX__"_fnv1au:
        return STATUS_OBJECT_NAME_NOT_FOUND;
    }
  }
  return g_pfnNtOpenKeyEx(KeyHandle, DesiredAccess, ObjectAttributes, OpenOptions);
}

std::array<ULONG_PTR, 2> g_ReadOnlyAddresses;
decltype(&NtProtectVirtualMemory) g_pfnNtProtectVirtualMemory;
NTSTATUS NTAPI NtProtectVirtualMemory_hook(
  HANDLE ProcessHandle,
  PVOID *BaseAddress,
  PSIZE_T RegionSize,
  ULONG NewProtect,
  PULONG OldProtect)
{
  PROCESS_BASIC_INFORMATION pbi;
  SYSTEM_BASIC_INFORMATION sbi;
  ULONG_PTR StartingAddress;

  if ( NewProtect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_WRITECOMBINE)
    && (ProcessHandle == NtCurrentProcess()
      || (NT_SUCCESS(NtQueryInformationProcess(ProcessHandle, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), nullptr))
        && pbi.UniqueProcessId == NtCurrentTeb()->ClientId.UniqueProcess))
    && NT_SUCCESS(NtQuerySystemInformation(SystemBasicInformation, &sbi, sizeof(SYSTEM_BASIC_INFORMATION), nullptr)) ) {

    __try {
      StartingAddress = (ULONG_PTR)*BaseAddress & ~((ULONG_PTR)sbi.PageSize - 1);
    } __except ( EXCEPTION_EXECUTE_HANDLER ) {
      return GetExceptionCode();
    }

    for ( const auto Address : std::array { (ULONG_PTR)&DbgBreakPoint, (ULONG_PTR)&DbgUiRemoteBreakin } ) {
      if ( Address && StartingAddress == (Address & ~((ULONG_PTR)sbi.PageSize - 1)) )
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
  PROCESS_BASIC_INFORMATION pbi;

  if ( ProcessHandle == NtCurrentProcess()
    || (NT_SUCCESS(g_pfnNtQueryInformationProcess(ProcessHandle, ProcessBasicInformation, &pbi, sizeof(PROCESS_BASIC_INFORMATION), nullptr))
      && pbi.UniqueProcessId == NtCurrentTeb()->ClientId.UniqueProcess) ) {

    switch ( ProcessInformationClass ) {
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
  switch ( SystemInformationClass ) {
    case SystemProcessInformation:
    case SystemModuleInformation:
    case SystemModuleInformationEx:
      return STATUS_ACCESS_DENIED;

    case SystemKernelDebuggerInformation:
    {
      if ( SystemInformationLength < sizeof(SYSTEM_KERNEL_DEBUGGER_INFORMATION) )
        return STATUS_INFO_LENGTH_MISMATCH;

      auto KernelDebuggerInformation = (PSYSTEM_KERNEL_DEBUGGER_INFORMATION)SystemInformation;
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
  THREAD_BASIC_INFORMATION tbi;

  if ( ThreadInformationClass == ThreadHideFromDebugger
    && ThreadInformationLength == 0 ) {
    if ( ThreadHandle == NtCurrentThread()
      || (NT_SUCCESS(NtQueryInformationThread(ThreadHandle, ThreadBasicInformation, &tbi, sizeof(THREAD_BASIC_INFORMATION), 0))
        && tbi.ClientId.UniqueProcess == NtCurrentTeb()->ClientId.UniqueProcess) ) {
      return STATUS_SUCCESS;
    }
  }
  return g_pfnNtSetInformationThread(ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength);
}

decltype(&FindWindowA) g_pfnFindWindowA;
HWND WINAPI FindWindowA_hook(
  LPCSTR lpClassName,
  LPCSTR lpWindowName)
{
  if ( lpClassName ) {
    switch ( fnv1a::make_hash(lpClassName, toupper) ) {
#ifdef _M_IX86
      case "OLLYDBG"_fnv1au:
      case "GBDYLLO"_fnv1au:
      case "pediy06"_fnv1au:
#endif         
      case "FilemonClass"_fnv1au:
      case "PROCMON_WINDOW_CLASS"_fnv1au:
      case "RegmonClass"_fnv1au:
      case "18467-41"_fnv1au:
        return nullptr;
    }
  }
  if ( lpWindowName ) {
    switch ( fnv1a::make_hash(lpWindowName) ) {
      case "File Monitor - Sysinternals: www.sysinternals.com"_fnv1a:
      case "Process Monitor - Sysinternals: www.sysinternals.com"_fnv1a:
      case "Registry Monitor - Sysinternals: www.sysinternals.com"_fnv1a:
        return nullptr;
    }
  }
  return g_pfnFindWindowA(lpClassName, lpWindowName);
}
