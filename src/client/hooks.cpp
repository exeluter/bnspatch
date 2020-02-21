#include "pch.h"
#include "hooks.h"
#include "pe/module.h"
#include "pe/segment.h"
#include "searchers.h"
//#include "Unreal.h"
#include "ntapi\string_span"
#include "locks.h"
#include <imgui/imgui.h>

void *g_pvDllNotificationCookie;
VOID CALLBACK DllNotification(
  ULONG                       NotificationReason,
  PCLDR_DLL_NOTIFICATION_DATA NotificationData,
  PVOID                       Context)
{
  static thread_local_lock lock;
  std::unique_lock<thread_local_lock> u(lock, std::try_to_lock);
  if ( !u.owns_lock() )
    return;

  switch ( NotificationReason ) {
    case LDR_DLL_NOTIFICATION_REASON_LOADED: {
      const auto Module = reinterpret_cast<pe::module *>(NotificationData->Loaded.DllBase);
      const auto BaseDllName = static_cast<const ntapi::ustring_span *>(NotificationData->Loaded.BaseDllName);

      //if ( BaseDllName->iequals(L"bsengine_Shipping64.dll") ) {
      //  if ( const auto segment = Module->segment(".text") ) {
      //    const auto data = segment->as_bytes();
      //    std::chrono::duration<double, std::milli> timer;

      //    if ( const auto &it = std::search(data.begin(), data.end(),
      //       pattern_searcher("48 8B 05 ?? ?? ?? ?? 48 63 C9", &timer)); it != data.end() ) {
      //      UObject::GObjObjects = (TArray<UObject *> *)(&it[7] + *(int32_t *)&it[3]);
      //    }

      //    if ( const auto &it = std::search(data.begin(), data.end(),
      //       pattern_searcher("48 8B 0D ?? ?? ?? ?? 48 89 04 D1", &timer)); it != data.end() ) {
      //      FName::Names = (TArray<FNameEntry *> *)(&it[7] + *(int32_t *)&it[3]);
      //    }

      //    if ( const auto &it = std::search(data.begin(), data.end(),
      //       pattern_searcher("48 8B 05 ?? ?? ?? ?? C1 E2 12", &timer)); it != data.end() ) {
      //      GEngine = (UEngine *)(&it[7] + *(int32_t *)&it[3]);
      //    }

      //    if ( const auto &it = std::search(data.begin(), data.end(),
      //       pattern_searcher("4C 8B CF 4C 8B C6 48 8B D5 48 8B CB E8", &timer)); it != data.end() ) {

      //      g_pfnProcessEvent = (decltype(g_pfnProcessEvent))(&it[0x11] + *(int32_t *)&it[0xd]);
      //      DetourTransactionBegin();
      //      DetourUpdateThread(NtCurrentThread());
      //      DetourAttach(&(void *&)g_pfnProcessEvent, &ProcessEvent_hook);
      //      DetourTransactionCommit();
      //    }
      //  }
      //} else if ( BaseDllName->iequals(L"XmlReader_cl64.dll") ) {
      //}
      break;
    } case LDR_DLL_NOTIFICATION_REASON_UNLOADED: {
      break;
    }
  }
}

//void(*g_pfnProcessEvent)(UObject *, UFunction *, void *, void *);
//void ProcessEvent_hook(UObject *Object, UFunction *Function, void *Parms, void *Result)
//{
  //if ( ObjectNameEquals(Function, std::array { 21, 2756, 6152 }) ) { // Engine.AnimatedCamera.ApplyCameraModifiers
  //  auto Params = (ACamera_ApplyCameraModifiers_Params *)Parms;
  //  Params->OutPOV.FOV = 105.0f;
  //} else if ( ObjectNameEquals(Function, std::array { 21, 162, 6194 }) ) { // Engine.Camera.eventSetFOV
  //  auto Params = (ACamera_eventSetFOV_Params *)Parms;
  //  Params->NewFOV = 105.0f;
  //} else if ( ObjectNameEquals(Function, std::array { 21, 2810, 6323 }) ) { // Engine.PlayerController.SetIgnoreLookInput
  //  return;
  //} else if ( ObjectNameEquals(Function, std::array { 6030, 6043, 337 }) ) { // T1Game.T1PlayerController.PlayerTick
  //}
  //g_pfnProcessEvent(Object, Function, Parms, Result);
//}


decltype(&LdrLoadDll) g_pfnLdrLoadDll;
NTSTATUS NTAPI LdrLoadDll_hook(
  PWSTR DllPath,
  PULONG DllCharacteristics,
  PUNICODE_STRING DllName,
  PVOID *DllHandle)
{
  static thread_local_lock lock;
  std::unique_lock<thread_local_lock> u(lock, std::try_to_lock);

  if ( u.owns_lock()
    && static_cast<ntapi::ustring_span *>(DllName)->iequals(xorstr_(L"aegisty64.bin")) ) {

    *DllHandle = nullptr;
    return STATUS_DLL_NOT_FOUND;
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

  if ( NewProtect & (PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY)
    && (ProcessHandle == NtCurrentProcess()
      || (NT_SUCCESS(NtQueryInformationProcess(ProcessHandle, ProcessBasicInformation, &ProcessInfo, sizeof(PROCESS_BASIC_INFORMATION), nullptr))
        && ProcessInfo.UniqueProcessId == NtCurrentTeb()->ClientId.UniqueProcess))
    && NT_SUCCESS(NtQuerySystemInformation(SystemBasicInformation, &SystemInfo, sizeof(SYSTEM_BASIC_INFORMATION), nullptr)) ) {

    if ( const auto module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
      __try {
        StartingAddress = (ULONG_PTR)*BaseAddress & ~((ULONG_PTR)SystemInfo.PageSize - 1);
      } __except ( EXCEPTION_EXECUTE_HANDLER ) {
        return GetExceptionCode();
      }

      for ( const auto &Name : std::array { xorstr_("DbgBreakPoint"), xorstr_("DbgUiRemoteBreakin") } ) {
        if ( const auto ProcedureAddress = module->find_function(Name);
          ProcedureAddress && StartingAddress == ((ULONG_PTR)ProcedureAddress & ~((ULONG_PTR)SystemInfo.PageSize - 1)) )
          return STATUS_INVALID_PARAMETER_2;
      }
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
    for ( const auto &String : {
      xorstr_("FilemonClass"), xorstr_("PROCMON_WINDOW_CLASS"), xorstr_("RegmonClass"), xorstr_("18467-41") } ) {
      if ( !_stricmp(lpClassName, String) )
        return nullptr;
    }
  }
  if ( lpWindowName ) {
    for ( const auto &String : {
      xorstr_("File Monitor - Sysinternals: www.sysinternals.com"),
      xorstr_("Process Monitor - Sysinternals: www.sysinternals.com"),
      xorstr_("Registry Monitor - Sysinternals: www.sysinternals.com") } ) {
      if ( !strcmp(lpWindowName, String) )
        return nullptr;
    }
  }
  return g_pfnFindWindowA(lpClassName, lpWindowName);
}
