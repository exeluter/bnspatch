#include "hooks.h"

#include <ntdll.h>
#include <ShlObj.h>
#include <KnownFolders.h>

#include <codecvt>
#include <filesystem>
namespace fs = std::filesystem;
#include <mutex>
#include <optional>
#include <queue>

#include <fastwildcompare.hpp>
#include <fmt/format.h>
#include <fnv1a.h>
#include <gsl/span>
#include <gsl/span_ext>
#include <magic_enum.hpp>
#include <pugixml/pugixml.hpp>
#include <SafeInt.hpp>
#include <xorstr.hpp>

#include "detours/detours.h"
#include "ntapi/string_span.h"
#include "thread_local_lock.h"
#include "pe/module.h"
#include "xmlhooks.h"
#include "xmlpatch.h"

void *g_pvDllNotificationCookie;
VOID CALLBACK DllNotification(
  ULONG NotificationReason,
  PCLDR_DLL_NOTIFICATION_DATA NotificationData,
  PVOID Context)
{
  static thread_local_lock mtx;
  std::unique_lock lock(mtx, std::try_to_lock);
  if ( !lock.owns_lock() )
    return;

  switch ( NotificationReason ) {
    case LDR_DLL_NOTIFICATION_REASON_LOADED: {
      const auto module = reinterpret_cast<pe::module *>(NotificationData->Loaded.DllBase);
      const auto base_name = static_cast<const ntapi::ustring_span *>(NotificationData->Loaded.BaseDllName);

#ifdef _M_X64
      if ( base_name->iequals(xorstr_(L"XmlReader_cl64.dll"))
        || base_name->iequals(xorstr_(L"XmlReader2017_cl64.dll")) ) {
#else
      if ( base_name->iequals(xorstr_(L"XmlReader_cl32.dll")) ) {
#endif
        if ( auto pfnGetInterfaceVersion = reinterpret_cast<wchar_t const *(*)()>(
          module->find_function(xorstr_("GetInterfaceVersion"))) ) {
          DetourTransactionBegin();
          DetourUpdateThread(NtCurrentThread());
          switch ( _wtoi(pfnGetInterfaceVersion()) ) {
            case 13:
              g_pfnCreateXmlReader13 = reinterpret_cast<decltype(g_pfnCreateXmlReader13)>(
                module->find_function(xorstr_("CreateXmlReader")));
              DetourAttach(&(PVOID &)g_pfnCreateXmlReader13, &CreateXmlReader13_hook);
              g_pfnDestroyXmlReader13 = reinterpret_cast<decltype(g_pfnDestroyXmlReader13)>(
                module->find_function(xorstr_("DestroyXmlReader")));
              DetourAttach(&(PVOID &)g_pfnDestroyXmlReader13, &DestroyXmlReader13_hook);
              break;
            case 14:
              g_pfnCreateXmlReader14 = reinterpret_cast<decltype(g_pfnCreateXmlReader14)>(
                module->find_function(xorstr_("CreateXmlReader")));
              DetourAttach(&(PVOID &)g_pfnCreateXmlReader14, &CreateXmlReader14_hook);
              g_pfnDestroyXmlReader14 = reinterpret_cast<decltype(g_pfnDestroyXmlReader14)>(
                module->find_function(xorstr_("DestroyXmlReader")));
              DetourAttach(&(PVOID &)g_pfnDestroyXmlReader14, &DestroyXmlReader14_hook);
              break;
            case 15:
              g_pfnCreateXmlReader15 = reinterpret_cast<decltype(g_pfnCreateXmlReader15)>(
                module->find_function(xorstr_("CreateXmlReader")));
              DetourAttach(&(PVOID &)g_pfnCreateXmlReader15, &CreateXmlReader15_hook);
              g_pfnDestroyXmlReader15 = reinterpret_cast<decltype(g_pfnDestroyXmlReader15)>(
                module->find_function(xorstr_("DestroyXmlReader")));
              DetourAttach(&(PVOID &)g_pfnDestroyXmlReader15, &DestroyXmlReader15_hook);
              break;
          }
          DetourTransactionCommit();
        }
      }
      break;
    } case LDR_DLL_NOTIFICATION_REASON_UNLOADED: {
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
    && static_cast<ntapi::ustring_span *>(DllName)->iequals(xorstr_(L"kmon.dll")) ) {
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
    if ( static_cast<ntapi::ustring_span *>(DllName)->iequals(
#ifdef _M_X64
      xorstr_(L"aegisty64.bin")
#else
      xorstr_(L"aegisty.bin")
#endif
    ) ) {
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
  if ( ObjectAttributes
    && ObjectAttributes->ObjectName
    && static_cast<ntapi::ustring_span *>(ObjectAttributes->ObjectName)->starts_with(xorstr_(L"BnSGameClient")) ) {

    ObjectAttributes->ObjectName = nullptr;
    ObjectAttributes->Attributes &= ~OBJ_OPENIF;
    ObjectAttributes->RootDirectory = nullptr;
  }
  return g_pfnNtCreateMutant(MutantHandle, DesiredAccess, ObjectAttributes, InitialOwner);
}

std::vector<void const *> g_ReadOnlyAddresses;
decltype(&NtProtectVirtualMemory) g_pfnNtProtectVirtualMemory;
NTSTATUS NTAPI NtProtectVirtualMemory_hook(
  HANDLE ProcessHandle,
  PVOID *BaseAddress,
  PSIZE_T RegionSize,
  ULONG NewProtect,
  PULONG OldProtect)
{
  PROCESS_BASIC_INFORMATION ProcessInfo;
  SYSTEM_BASIC_INFORMATION SystemInfo;
  ULONG_PTR StartingAddress;

  if ( NewProtect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY | PAGE_WRITECOMBINE)
    && (ProcessHandle == NtCurrentProcess()
      || (NT_SUCCESS(NtQueryInformationProcess(ProcessHandle, ProcessBasicInformation, &ProcessInfo, sizeof(PROCESS_BASIC_INFORMATION), nullptr))
        && ProcessInfo.UniqueProcessId == NtCurrentTeb()->ClientId.UniqueProcess))
    && NT_SUCCESS(NtQuerySystemInformation(SystemBasicInformation, &SystemInfo, sizeof(SYSTEM_BASIC_INFORMATION), nullptr)) ) {

    __try {
      StartingAddress = (ULONG_PTR)*BaseAddress & ~((ULONG_PTR)SystemInfo.PageSize - 1);
    } __except ( EXCEPTION_EXECUTE_HANDLER ) {
      return GetExceptionCode();
    }

    for ( const auto &Address : g_ReadOnlyAddresses ) {
      if ( Address && StartingAddress == ((ULONG_PTR)Address & ~((ULONG_PTR)SystemInfo.PageSize - 1)) )
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
  PROCESS_BASIC_INFORMATION ProcessInfo;

  if ( ProcessHandle == NtCurrentProcess()
    || (NT_SUCCESS(g_pfnNtQueryInformationProcess(ProcessHandle, ProcessBasicInformation, &ProcessInfo, sizeof(PROCESS_BASIC_INFORMATION), nullptr))
      && ProcessInfo.UniqueProcessId == NtCurrentTeb()->ClientId.UniqueProcess) ) {

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

    case SystemKernelDebuggerInformation: {
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

decltype(&FindWindowA) g_pfnFindWindowA;
HWND WINAPI FindWindowA_hook(
  LPCSTR lpClassName,
  LPCSTR lpWindowName)
{
  if ( lpClassName ) {
    switch ( fnv1a::make_hash_upper(lpClassName) ) {
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
