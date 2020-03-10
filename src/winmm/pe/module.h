#pragma once
#include <ntdll.h>
#include <string>
#include <mutex>
#include "..\ic_char_traits.h"
#include "segment.h"
#include "..\ntapi\string_span.h"
#include "..\ntapi\critical_section.h"

namespace pe
{
  class segment;
  class export_directory;

  class module : private IMAGE_DOS_HEADER
  {
  public:
    module() = delete;

    inline uintptr_t handle() const
    {
      return reinterpret_cast<uintptr_t>(this);
    }

    inline operator HINSTANCE() const
    {
      return reinterpret_cast<HINSTANCE>(const_cast<pe::module *>(this));
    }

    template <class T>
    inline T *rva_to(uint32_t rva)
    {
      return reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(this) + rva);
    }

    template <class T>
    inline T *rva_to(uint32_t rva) const
    {
      static_assert(std::is_const_v<T>);

      return reinterpret_cast<const T *>(reinterpret_cast<uintptr_t>(this) + rva);
    }

    inline ic_wstring base_name() const
    {
      ntapi::critical_section crit(NtCurrentPeb()->LoaderLock);
      std::lock_guard<ntapi::critical_section> guard(crit);

      const auto ModuleListHead = &NtCurrentPeb()->Ldr->InLoadOrderModuleList;
      for ( auto Entry = ModuleListHead->Flink; Entry != ModuleListHead; Entry = Entry->Flink ) {
        auto Module = CONTAINING_RECORD(Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

        if ( Module->DllBase == this ) {
          const auto name = static_cast<ntapi::ustring_span *>(&Module->BaseDllName);
          return ic_wstring(name->begin(), name->end());
        }
      }
      return {};
    }

    inline ic_wstring full_name() const
    {
      ntapi::critical_section crit(NtCurrentPeb()->LoaderLock);
      std::lock_guard<ntapi::critical_section> guard(crit);

      const auto ModuleListHead = &NtCurrentPeb()->Ldr->InLoadOrderModuleList;
      for ( auto Entry = ModuleListHead->Flink; Entry != ModuleListHead; Entry = Entry->Flink ) {
        auto Module = CONTAINING_RECORD(Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

        if ( Module->DllBase == this ) {
          const auto name = static_cast<ntapi::ustring_span *>(&Module->FullDllName);
          return ic_wstring(name->begin(), name->end());
        }
      }
      return {};
    }

    inline IMAGE_DOS_HEADER *dos_header()
    {
      return reinterpret_cast<IMAGE_DOS_HEADER *>(this);
    }

    inline const IMAGE_DOS_HEADER *dos_header() const
    {
      return reinterpret_cast<const IMAGE_DOS_HEADER *>(this);
    }

    inline IMAGE_NT_HEADERS *nt_header()
    {
      if ( this->e_magic != IMAGE_DOS_SIGNATURE )
        return nullptr;

      const auto ntheader = this->rva_to<IMAGE_NT_HEADERS>(this->e_lfanew);
      if ( ntheader->Signature != IMAGE_NT_SIGNATURE )
        return nullptr;

      return ntheader;
    }

    inline const IMAGE_NT_HEADERS *nt_header() const
    {
      return const_cast<pe::module *>(this)->nt_header();
    }

    inline size_t size() const
    {
      if ( this->e_magic != IMAGE_DOS_SIGNATURE )
        return 0;

      const auto ntheader = this->nt_header();
      if ( !ntheader
        || !ntheader->FileHeader.SizeOfOptionalHeader
        || ntheader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC )
        return 0;

      return ntheader->OptionalHeader.SizeOfImage;
    }

    inline gsl::span<class pe::segment> segments()
    {
      const auto ntheader = this->nt_header();
      if ( !ntheader )
        return {};

      return gsl::make_span(
        reinterpret_cast<class pe::segment *>(IMAGE_FIRST_SECTION(ntheader)),
        ntheader->FileHeader.NumberOfSections);
    }

    inline gsl::span<const class pe::segment> segments() const
    {
      return const_cast<pe::module *>(this)->segments();
    }

    inline class pe::segment *segment(const std::string_view &name)
    {
      const auto segments = this->segments();
      const auto &it = std::find_if(segments.begin(), segments.end(), [&](const pe::segment &x) {
        return x.name() == name;
      });
      if ( it != segments.end() )
        return &*it;

      throw nullptr;
    }

    inline const class pe::segment *segment(const std::string_view &name) const
    {
      return const_cast<pe::module *>(this)->segment(name);
    }

    inline pe::export_directory *export_directory()
    {
      const auto ntheader = this->nt_header();
      if ( !ntheader
        || !ntheader->FileHeader.SizeOfOptionalHeader
        || ntheader->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC
        || !ntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress
        || !ntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size )
        return nullptr;

      return this->rva_to<pe::export_directory>(
        ntheader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    }

    inline const pe::export_directory *export_directory() const
    {
      return const_cast<pe::module *>(this)->export_directory();
    }

    inline void *find_function(const char *name) const
    {
      if ( !name ) return nullptr;

      if ( PVOID ProcedureAddress;
        NT_SUCCESS(LdrGetProcedureAddress(const_cast<pe::module *>(this), ntapi::string_span(name), 0, &ProcedureAddress)) ) {
        return ProcedureAddress;
      }
      return nullptr;
    }

    inline void *find_function(uint32_t num) const
    {
      if ( !num ) return nullptr;

      if ( PVOID ProcedureAddress;
        NT_SUCCESS(LdrGetProcedureAddress(const_cast<pe::module *>(this), nullptr, num, &ProcedureAddress)) ) {
        return ProcedureAddress;
      }
      return nullptr;
    };
  };
}
