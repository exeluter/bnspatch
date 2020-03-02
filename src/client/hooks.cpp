#include "pch.h"
#include "hooks.h"
#include "pe/module.h"
#include "pe/segment.h"
#include "searchers.h"
#include "ntapi/string_span"
#include "locks.h"
#include <imgui/imgui.h>
#include <imgui/imgui_impl_win32.h>
#include <imgui/imgui_impl_dx9.h>
#include "xmlreader.h"

//void process_element(XmlElement const *element, pugi::xml_node &parent)
//{
//  auto node = parent.append_child(element->Name());
//  for ( int i = 0; i < element->AttributeCount(); ++i ) {
//    auto attr = node.append_attribute(element->AttributeName(i));
//    attr.set_value(element->Attribute(i));
//  }
//  for ( auto next = element->FirstChildElement(); next; next = next->NextElement() ) {
//    process_element(next, node);
//  }
//}

XmlDoc *(*g_pfnRead)(XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class XmlPieceReader *);
XmlDoc *Read_hook(
  XmlReader const *thisptr,
  unsigned char const *data,
  unsigned int size,
  wchar_t const *fileName,
  class XmlPieceReader *arg4)
{
  static fs::path folder;

  if ( folder.empty() ) {
    std::array<WCHAR, _MAX_DIR + 1> Publisher;
    auto file = fs::absolute(xorstr_(L"local.ini"));
    
    if ( GetPrivateProfileStringW(xorstr_(L"Locale"), xorstr_(L"Publisher"), nullptr, Publisher.data(), SafeInt(Publisher.size()), file.c_str()) )
      folder = fmt::format(xorstr_(L"..\\contents\\Local\\{}\\data"), Publisher.data());
  }

  if ( fileName ) {
    auto file = folder / fileName;
    if ( GetFileAttributesW(file.c_str()) != INVALID_FILE_ATTRIBUTES )
      return thisptr->Read(file.c_str(), arg4);
  }
  return g_pfnRead(thisptr, data, size, fileName, arg4);
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
      const auto module = reinterpret_cast<pe::module *>(NotificationData->Loaded.DllBase);
      const auto base_name = static_cast<const ntapi::ustring_span *>(NotificationData->Loaded.BaseDllName);

      if ( base_name->iequals(xorstr_(L"XmlReader_cl64.dll")) || base_name->iequals(xorstr_(L"XmlReader2017_cl64.dll")) ) {
        if ( auto GetInterfaceVersion = reinterpret_cast<wchar_t const *(*)()>(
          module->find_function(xorstr_("GetInterfaceVersion"))) ) {
          switch ( _wtoi(GetInterfaceVersion()) ) {
            case 14:
            case 15:
              DetourTransactionBegin();
              DetourUpdateThread(NtCurrentThread());
              if ( g_pfnCreateXmlReader = reinterpret_cast<decltype(g_pfnCreateXmlReader)>(
                module->find_function(xorstr_("CreateXmlReader"))) )
                DetourAttach(&(PVOID &)g_pfnCreateXmlReader, &CreateXmlReader_hook);
              if ( g_pfnDestroyXmlReader = reinterpret_cast<decltype(g_pfnDestroyXmlReader)>(
                module->find_function(xorstr_("DestroyXmlReader"))) )
                DetourAttach(&(PVOID &)g_pfnDestroyXmlReader, &DestroyXmlReader_hook);
              DetourTransactionCommit();
              break;
          }
        }
      }
      break;
    } case LDR_DLL_NOTIFICATION_REASON_UNLOADED: {
      break;
    }
  }
}

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

      auto xs1 = xorstr("DbgBreakPoint");
      auto xs2 = xorstr("DbgUiRemoteBreakin");
      for ( const auto &Name : { xs1.crypt_get(), xs2.crypt_get() } ) {
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
    auto xs1 = xorstr("FilemonClass");
    auto xs2 = xorstr("PROCMON_WINDOW_CLASS");
    auto xs3 = xorstr("RegmonClass");
    auto xs4 = xorstr("18467-41");
    for ( const auto &String : { xs1.crypt_get(), xs2.crypt_get(), xs3.crypt_get(), xs4.crypt_get() } ) {
      if ( !_stricmp(lpClassName, String) )
        return nullptr;
    }
  }
  if ( lpWindowName ) {
    auto xs1 = xorstr("File Monitor - Sysinternals: www.sysinternals.com");
    auto xs2 = xorstr("Process Monitor - Sysinternals: www.sysinternals.com");
    auto xs3 = xorstr("Registry Monitor - Sysinternals: www.sysinternals.com");
    for ( const auto &String : { xs1.crypt_get(), xs2.crypt_get(), xs3.crypt_get() } ) {
      if ( !strcmp(lpWindowName, String) )
        return nullptr;
    }
  }
  return g_pfnFindWindowA(lpClassName, lpWindowName);
}

decltype(&GetProcAddress) g_pfnGetProcAddress;
FARPROC WINAPI GetProcAddress_hook(
  HMODULE hModule,
  LPCSTR lpProcName)
{
  //auto name = reinterpret_cast<pe::module *>(hModule)->base_name();

  //if ( (name == L"XmlReader_cl64.dll"|| name == L"XmlReader2017_cl64.dll") ) {
  //  if ( !strcmp(lpProcName, "CreateXmlReader") )
  //  return reinterpret_cast<FARPROC>(&CreateXmlReader_hook);
  //}
  return g_pfnGetProcAddress(hModule, lpProcName);
}

WNDPROC g_pfnWndProc;
LRESULT CALLBACK WndProc_hook(
  HWND hwnd,
  UINT uMsg,
  WPARAM wParam,
  LPARAM lParam)
{
  //if ( ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam) == ERROR_SUCCESS ) {
  //}
  return CallWindowProcW(g_pfnWndProc, hwnd, uMsg, wParam, lParam);
}

decltype(&CreateWindowExW) g_pfnCreateWindowExW;
HWND WINAPI CreateWindowExW_hook(
  DWORD dwExStyle,
  LPCWSTR lpClassName,
  LPCWSTR lpWindowName,
  DWORD dwStyle,
  int X,
  int Y,
  int nWidth,
  int nHeight,
  HWND hWndParent,
  HMENU hMenu,
  HINSTANCE hInstance,
  LPVOID lpParam)
{
  auto hWnd = g_pfnCreateWindowExW(
    dwExStyle,
    lpClassName,
    lpWindowName,
    dwStyle,
    X,
    Y,
    nWidth,
    nHeight,
    hWndParent,
    hMenu,
    hInstance,
    lpParam);

  if ( hWnd
    && HIWORD(lpClassName)
    && (!_wcsicmp(lpClassName, xorstr_(L"LaunchUnrealUWindowsClient"))
      || !_wcsicmp(lpClassName, xorstr_(L"UnrealWindow"))) ) {

    g_pfnWndProc = reinterpret_cast<WNDPROC>(
      SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc_hook)));
  }
  return hWnd;
}