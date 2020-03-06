#include "pch.h"
#include "hooks.h"
#include "pe/module.h"
#include "ntapi/string_span"
#include "locks.h"
#include "xmlreader.h"
#include "FastWildCompare.h"

void process_patch(
  pugi::xpath_node const &context,
  pugi::xml_object_range<pugi::xml_node_iterator> const &children)
{
  using pugi::xml_document;
  using pugi::xml_node;
  using pugi::xml_node_iterator;
  using pugi::xml_node_type;
  using pugi::xml_object_range;

  for ( auto const &current : children ) {
    if ( !_wcsicmp(current.name(), xorstr_(L"parent")) ) {
      process_patch(context.parent(), current.children());
    } else if ( auto context_attr = context.attribute() ) {
      if ( !_wcsicmp(current.name(), xorstr_(L"set-name")) ) {
        context_attr.set_name(current.attribute(xorstr_(L"name")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"set-value")) ) {
        context_attr.set_value(current.attribute(xorstr_(L"value")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"previous-attribute")) ) {
        process_patch({ context_attr.previous_attribute(), context.parent() }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"next-attribute")) ) {
        process_patch({ context_attr.next_attribute(), context.parent() }, current.children());
      }
    } else if ( auto context_node = context.node() ) {
      if ( !_wcsicmp(current.name(), xorstr_(L"select-node")) ) {
        process_patch(context_node.select_node(current.attribute(xorstr_(L"query")).value()), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"select-nodes")) ) {
        for ( auto &xn : context_node.select_nodes(current.attribute(xorstr_(L"query")).value()) ) {
          process_patch(xn, current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-attribute")) ) {
        auto const name = current.attribute(xorstr_(L"name"));
        process_patch({ context_node.prepend_attribute(name.value()), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-attribute")) ) {
        auto const name = current.attribute(xorstr_(L"name"));
        process_patch({ context_node.append_attribute(name.value()), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-child")) ) {
        if ( auto const name = current.attribute(xorstr_(L"name")) ) {
          process_patch(context_node.append_child(name.value()), current.children());
        } else {
          std::optional<xml_node_type> t;
          if ( auto const type = current.attribute(xorstr_(L"type")) ) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            auto s = converter.to_bytes(type.value());
            t = magic_enum::enum_cast<xml_node_type>(s);
          }
          process_patch(context_node.append_child(t.value_or(xml_node_type::node_element)), current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-child")) ) {
        if ( auto const name = current.attribute(xorstr_(L"name")) ) {
          process_patch(context_node.append_child(name.value()), current.children());
        } else {
          std::optional<xml_node_type> t;
          if ( auto const type = current.attribute(xorstr_(L"type")) ) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            auto s = converter.to_bytes(type.value());
            t = magic_enum::enum_cast<xml_node_type>(s);
          }
          process_patch(context_node.append_child(t.value_or(xml_node_type::node_element)), current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-children")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-children")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-copy")) ) {
        auto xn = context_node.select_node(current.attribute(xorstr_(L"proto-query")).value());
        if ( auto attr = xn.attribute() ) {
          process_patch({ context_node.prepend_copy(attr), xn.parent() }, current.children());
        } else if ( auto node = xn.node() ) {
          process_patch(context_node.prepend_copy(node), current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-copy")) ) {
        auto xn = context_node.select_node(current.attribute(xorstr_(L"proto-query")).value());
        if ( auto attr = xn.attribute() ) {
          process_patch({ context_node.append_copy(attr), xn.parent() }, current.children());
        } else if ( auto node = xn.node() ) {
          process_patch(context_node.append_copy(node), current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-move")) ) {
        process_patch(
          context_node.prepend_move(context_node.select_node(current.attribute(xorstr_(L"moved-query")).value()).node()),
          current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-move")) ) {
        process_patch(context_node.append_move(context_node.select_node(current.attribute(xorstr_(L"moved-query")).value()).node()), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"attribute")) ) {
        process_patch({ context_node.attribute(current.attribute(xorstr_(L"name")).value()), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"attributes")) ) {
        for ( auto &attr : context_node.attributes() ) {
          process_patch({ attr, context_node }, current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"child")) ) {
        process_patch(context_node.child(current.attribute(xorstr_(L"name")).value()), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"children")) ) {
        for ( auto &child : context_node.children() ) {
          process_patch(child, current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"find-child-by-attribute")) ) {
        process_patch(
          context_node.find_child_by_attribute(
            current.attribute(xorstr_(L"name")).value(),
            current.attribute(xorstr_(L"attr-name")).value(),
            current.attribute(xorstr_(L"attr-value")).value()),
          current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"first-attribute")) ) {
        process_patch({ context_node.first_attribute(), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"last-attribute")) ) {
        process_patch({ context_node.last_attribute(), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"first-child")) ) {
        process_patch(context_node.first_child(), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"last-child")) ) {
        process_patch(context_node.last_child(), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"first-element-by-path")) ) {
        process_patch(context_node.first_element_by_path(current.attribute(xorstr_(L"path")).value()), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-attribute-before")) ) {
        auto name = current.attribute(xorstr_(L"name")).value();
        auto attr_query = current.attribute(xorstr_(L"attr-query")).value();
        auto xn = context_node.select_node(attr_query);
        process_patch({ context_node.insert_attribute_before(name, xn.attribute()), xn.parent() }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-attribute-after")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-child-after")) ) {
        process_patch(
          context_node.insert_child_after(
            current.attribute(xorstr_(L"name")).value(),
            context_node.select_node(current.attribute(xorstr_(L"query")).value()).node()),
          current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-child-before")) ) {
        process_patch(
          context_node.insert_child_before(
            current.attribute(xorstr_(L"name")).value(),
            context_node.select_node(current.attribute(xorstr_(L"query")).value()).node()),
          current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-copy-after")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-copy-before")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-move-after")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-move-before")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"next-sibling")) ) {
        process_patch(context_node.next_sibling(), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-attribute")) ) {
        context_node.remove_attribute(current.attribute(xorstr_(L"name")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-attributes")) ) {
        context_node.remove_attributes();
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-child")) ) {
        context_node.remove_child(current.attribute(xorstr_(L"name")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-children")) ) {
        context_node.remove_children();
      } else if ( !_wcsicmp(current.name(), xorstr_(L"root")) ) {
        process_patch(context_node.root(), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"set-name")) ) {
        context_node.set_name(current.attribute(xorstr_(L"value")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"set-value")) ) {
        context_node.set_value(current.attribute(xorstr_(L"value")).value());
      }
    }
  }
}

static pugi::xml_document const s_xml_patches = []() {
  PWSTR pszPath;
  if ( SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &pszPath) == S_OK ) {
    fs::path path(pszPath);
    CoTaskMemFree(pszPath);
#ifdef BNSR
    path /= xorstr_(L"BNSR\\patches.xml");
#else
    path /= xorstr_(L"BnS\\patches.xml");
#endif
    if ( pugi::xml_document doc; doc.load_file(path.c_str()) )
      return doc;
  }
  return pugi::xml_document();
}();

void process_xmldoc(XmlElement const *src, pugi::xml_node &dst)
{
  auto node = dst.append_child(src->Name());
  for ( int i = 0; i < src->AttributeCount(); ++i )
    node.append_attribute(src->AttributeName(i)) = src->Attribute(i);
  for ( auto child = src->FirstChildElement(); child; child = child->NextElement() )
    process_xmldoc(child, node);
}

XmlDoc *(__thiscall *g_pfnRead)(XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class XmlPieceReader *);
XmlDoc *__fastcall Read_hook(
  XmlReader const *thisptr,
#ifdef _M_IX86
  uintptr_t, // unused EDX register
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *fileName,
  class XmlPieceReader *arg4)
{
  auto xmlDoc = g_pfnRead(thisptr, data, size, fileName, arg4);

  if ( fileName && xmlDoc && xmlDoc->IsValid() ) {
    std::vector<pugi::xml_node> vec;
    for ( auto const &x : s_xml_patches.select_nodes(xorstr_(L"/patches/patch")) ) {
      if ( FastWildCompare(x.node().attribute(xorstr_(L"filename")).value(), fileName) ) {
        vec.push_back(x.node());
      }
    }
    if ( !vec.empty() ) {
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

      for ( auto const &node : vec )
        process_patch(doc.document_element(), node.children());

      if ( std::array<WCHAR, MAX_PATH> temp_path;
        GetTempPathW(SafeInt(temp_path.size()), temp_path.data()) ) {
        if ( std::array<WCHAR, MAX_PATH> temp_file;
          GetTempFileNameW(temp_path.data(), xorstr_(L"bns"), 0, temp_file.data())
          && doc.save_file(temp_file.data(), xorstr_(L"  "), flags, encoding) ) {

          thisptr->Close(xmlDoc);
          xmlDoc = thisptr->Read(temp_file.data(), arg4);
#ifdef NDEBUG
          DeleteFile(temp_file.data());
#endif
        }
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
          auto version = pfnGetInterfaceVersion();

          if ( !_wcsicmp(version, xorstr_(L"14"))
            || !_wcsicmp(version, xorstr_(L"15")) ) {
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
    && static_cast<ntapi::ustring_span *>(DllName)->iequals(L"kmon.dll") ) {
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
#ifdef _M_X64
    if ( static_cast<ntapi::ustring_span *>(DllName)->iequals(xorstr_(L"aegisty64.bin")) ) {
#else
    if ( static_cast<ntapi::ustring_span *>(DllName)->iequals(xorstr_(L"aegisty.bin")) ) {
#endif
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

void *g_pfnDbgBreakPoint;
void *g_pfnDbgUiRemoteBreakin;
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

    __try {
      StartingAddress = (ULONG_PTR)*BaseAddress & ~((ULONG_PTR)SystemInfo.PageSize - 1);
    } __except ( EXCEPTION_EXECUTE_HANDLER ) {
      return GetExceptionCode();
    }

    for ( const auto &Address : { g_pfnDbgBreakPoint, g_pfnDbgUiRemoteBreakin } ) {
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
#ifdef _M_IX86
    auto xs0 = xorstr("OLLYDBG");
    auto xs1 = xorstr("GBDYLLO");
    auto xs2 = xorstr("pediy06");
#endif         
    auto xs3 = xorstr("FilemonClass");
    auto xs4 = xorstr("PROCMON_WINDOW_CLASS");
    auto xs5 = xorstr("RegmonClass");
    auto xs6 = xorstr("18467-41");
    for ( const auto &String : {
#ifdef _M_IX86
      xs0.crypt_get(), xs1.crypt_get(), xs2.crypt_get(),
#endif
      xs3.crypt_get(), xs4.crypt_get(), xs5.crypt_get(), xs6.crypt_get() } ) {
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

//WNDPROC g_pfnWndProc;
//LRESULT CALLBACK WndProc_hook(
//  HWND hwnd,
//  UINT uMsg,
//  WPARAM wParam,
//  LPARAM lParam)
//{
//  //if ( ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam) == ERROR_SUCCESS ) {
//  //}
//  return CallWindowProcW(g_pfnWndProc, hwnd, uMsg, wParam, lParam);
//}

//decltype(&CreateWindowExW) g_pfnCreateWindowExW;
//HWND WINAPI CreateWindowExW_hook(
//  DWORD dwExStyle,
//  LPCWSTR lpClassName,
//  LPCWSTR lpWindowName,
//  DWORD dwStyle,
//  int X,
//  int Y,
//  int nWidth,
//  int nHeight,
//  HWND hWndParent,
//  HMENU hMenu,
//  HINSTANCE hInstance,
//  LPVOID lpParam)
//{
//  auto hWnd = g_pfnCreateWindowExW(
//    dwExStyle,
//    lpClassName,
//    lpWindowName,
//    dwStyle,
//    X,
//    Y,
//    nWidth,
//    nHeight,
//    hWndParent,
//    hMenu,
//    hInstance,
//    lpParam);
//
//  //if ( hWnd
//  //  && HIWORD(lpClassName)
//  //  && (!_wcsicmp(lpClassName, xorstr_(L"LaunchUnrealUWindowsClient"))
//  //    || !_wcsicmp(lpClassName, xorstr_(L"UnrealWindow"))) ) {
//
//  //  g_pfnWndProc = reinterpret_cast<WNDPROC>(
//  //    SetWindowLongPtrW(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WndProc_hook)));
//  //}
//  return hWnd;
//}