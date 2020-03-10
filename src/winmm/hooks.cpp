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

#include <detours/detours.h>
#include <fastwildcompare.hpp>
#include <fmt/format.h>
#include <gsl/span>
#include <gsl/span_ext>
#include <magic_enum.hpp>
#include <pugixml/pugixml.hpp>
#include <SafeInt.hpp>
#include <xorstr.hpp>

#include "fnv1a.h"
#include "ntapi/string_span.h"
#include "thread_local_lock.h"
#include "pe/module.h"
#include "xmlreader.h"
#include "xmlpatch.h"

XmlDoc *(__thiscall *g_pfnRead)(XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class XmlPieceReader *);
XmlDoc *__fastcall Read_hook(
  XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *fileName,
  class XmlPieceReader *arg4)
{
  auto xmlDoc = g_pfnRead(thisptr, data, size, fileName, arg4);

  if ( fileName && xmlDoc && xmlDoc->IsValid() ) {
    std::queue<pugi::xml_node> queue;
    for ( auto const &x : g_patches.select_nodes(xorstr_(L"/patches/patch")) ) {
      if ( FastWildCompare(x.node().attribute(xorstr_(L"filename")).value(), fileName) )
        queue.push(x.node());
    }
    if ( !queue.empty() ) {
      pugi::xml_document doc;
      auto decl = doc.prepend_child(pugi::node_declaration);
      decl.append_attribute(xorstr_(L"version")) = xorstr_(L"1.0");

#ifdef NDEBUG
      int flags = pugi::format_raw;
#else
      int flags = pugi::format_default;
#endif
      auto encoding = pugi::xml_encoding::encoding_auto;

      if ( auto ext = fs::path(fileName).extension();
        !_wcsicmp(ext.c_str(), xorstr_(L".x16")) ) {

        decl.append_attribute(xorstr_(L"encoding")) = xorstr_(L"utf-16");
        flags |= pugi::format_write_bom;
        encoding = pugi::xml_encoding::encoding_utf16;
      } else {
        decl.append_attribute(xorstr_(L"encoding")) = xorstr_(L"utf-8");
      }
      if ( auto name = xmlDoc->Name() )
        doc.append_child(pugi::node_comment).set_value(name);

      process_xmldoc(xmlDoc->Root(), doc);

      do {
        auto const &patch = queue.front();
        process_patch(doc.document_element(), patch.children());
        queue.pop();
      } while ( !queue.empty() );

      if ( std::array<WCHAR, MAX_PATH> path;
        GetTempFileNameW(fs::temp_directory_path().c_str(), xorstr_(L"bns"), 0, path.data())
        && doc.save_file(path.data(), xorstr_(L"  "), flags, encoding) ) {

        thisptr->Close(xmlDoc);
        xmlDoc = thisptr->Read(path.data(), arg4);
#ifdef NDEBUG
        DeleteFileW(path.data());
#endif
      }
    }
  }
  return xmlDoc;
}

XmlReader *(*g_pfnCreateXmlReader)(void);
XmlReader *CreateXmlReader_hook(void)
{
  auto xmlReader = g_pfnCreateXmlReader();
  auto vftable = gsl::make_span(*reinterpret_cast<void ***>(xmlReader), 12);
  auto vfcopy = new void *[vftable.size()];
  std::copy(vftable.begin(), vftable.end(), vfcopy);
  g_pfnRead = reinterpret_cast<decltype(g_pfnRead)>(
    InterlockedExchangePointer(&vfcopy[7], &Read_hook));
  *reinterpret_cast<void ***>(xmlReader) = vfcopy;
  return xmlReader;
}

void(*g_pfnDestroyXmlReader)(XmlReader *);
void DestroyXmlReader_hook(XmlReader *xmlReader)
{
  auto vfcopy = *reinterpret_cast<void ***>(xmlReader);
  g_pfnDestroyXmlReader(xmlReader);
  delete[] vfcopy;
}

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
          auto version = ic_wstring_view(pfnGetInterfaceVersion());

          if ( version == xorstr_(L"14") || version == xorstr_(L"15") ) {
            DetourTransactionBegin();
            DetourUpdateThread(NtCurrentThread());
            if ( g_pfnCreateXmlReader = reinterpret_cast<decltype(g_pfnCreateXmlReader)>(
              module->find_function(xorstr_("CreateXmlReader"))) )
              DetourAttach(&(PVOID &)g_pfnCreateXmlReader, &CreateXmlReader_hook);
            if ( g_pfnDestroyXmlReader = reinterpret_cast<decltype(g_pfnDestroyXmlReader)>(
              module->find_function(xorstr_("DestroyXmlReader"))) )
              DetourAttach(&(PVOID &)g_pfnDestroyXmlReader, &DestroyXmlReader_hook);
            DetourTransactionCommit();
          }
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
    switch ( fnv1a::hash_upper<CHAR>(lpClassName) ) {
#ifdef _M_IX86
      case "OLLYDBG"_fnv1a:
      case "GBDYLLO"_fnv1a:
      case "pediy06"_fnv1a:
#endif         
      case "FilemonClass"_fnv1a:
      case "pediyPROCMON_WINDOW_CLASS06"_fnv1a:
      case "RegmonClass"_fnv1a:
      case "18467-41"_fnv1a:
        return nullptr;
    }
  }
  if ( lpWindowName ) {
    switch ( fnv1a::hash<CHAR>(lpWindowName) ) {
      case "File Monitor - Sysinternals: www.sysinternals.com"_fnv1a:
      case "Process Monitor - Sysinternals: www.sysinternals.com"_fnv1a:
      case "Registry Monitor - Sysinternals: www.sysinternals.com"_fnv1a:
        return nullptr;
    }
  }
  return g_pfnFindWindowA(lpClassName, lpWindowName);
}
