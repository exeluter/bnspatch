#pragma once
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>
#include "..\ntapi\string_span"

namespace pe {
  class module;

  inline pe::module *get_module(const wchar_t *name = nullptr)
  {
    if ( !name )
      return reinterpret_cast<pe::module *>(NtCurrentPeb()->ImageBaseAddress);

    if ( PVOID DllHandle; NT_SUCCESS(LdrGetDllHandle(nullptr, nullptr, ntapi::ustring_span(name), &DllHandle)) )
      return reinterpret_cast<pe::module *>(DllHandle);

    return nullptr;
  }

  inline pe::module *get_module_from_address(void *pc)
  {
    void *Unused;
    return reinterpret_cast<pe::module *>(RtlPcToFileHeader(pc, &Unused));
  }

  inline const pe::module *get_module_from_address(const void *pc)
  {
    return pe::get_module_from_address(const_cast<void *>(pc));
  }


  EXTERN_C IMAGE_DOS_HEADER __ImageBase;

  inline pe::module *get_instance_module()
  {
    return reinterpret_cast<pe::module *>(&__ImageBase);
  }
}
