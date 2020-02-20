#pragma once
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>
#include <string>
#include "traits.h"
#include "segment.h"
#include "..\ntapi\string_span"

namespace pe
{
  class segment;
  class export_directory;

  class module : public IMAGE_DOS_HEADER
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

  private:
    inline const ntapi::ustring_span *try_base_name() const
    {
      PVOID Cookie = nullptr;
      ntapi::ustring_span *u = nullptr;

      __try {
        (void)LdrLockLoaderLock(LDR_LOCK_LOADER_LOCK_FLAG_RAISE_ON_ERRORS, NULL, &Cookie);
        const auto ModuleListHead = &NtCurrentPeb()->Ldr->InLoadOrderModuleList;

        for ( auto Entry = ModuleListHead->Flink; Entry != ModuleListHead; Entry = Entry->Flink ) {
          auto Module = CONTAINING_RECORD(Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

          if ( Module->DllBase == this ) {
            u = static_cast<ntapi::ustring_span *>(&Module->BaseDllName);
            break;
          }
          Entry = Entry->Flink;
        }
      } __except ( EXCEPTION_EXECUTE_HANDLER ) {
      }
      (void)LdrUnlockLoaderLock(LDR_LOCK_LOADER_LOCK_FLAG_RAISE_ON_ERRORS, Cookie);
      return u;
    }

    inline const ntapi::ustring_span *try_full_name() const
    {
      PVOID Cookie = nullptr;
      ntapi::ustring_span *u = nullptr;

      __try {
        (void)LdrLockLoaderLock(LDR_LOCK_LOADER_LOCK_FLAG_RAISE_ON_ERRORS, NULL, &Cookie);
        const auto ModuleListHead = &NtCurrentPeb()->Ldr->InLoadOrderModuleList;

        for ( auto Entry = ModuleListHead->Flink; Entry != ModuleListHead; Entry = Entry->Flink ) {
          auto Module = CONTAINING_RECORD(Entry, LDR_DATA_TABLE_ENTRY, InLoadOrderLinks);

          if ( Module->DllBase == this ) {
            u = static_cast<ntapi::ustring_span *>(&Module->FullDllName);
            break;
          }
          Entry = Entry->Flink;
        }
      } __except ( EXCEPTION_EXECUTE_HANDLER ) {
      }
      (void)LdrUnlockLoaderLock(LDR_LOCK_LOADER_LOCK_FLAG_RAISE_ON_ERRORS, Cookie);
      return u;
    }

  public:
    inline pe::ic_wstring base_name() const
    {
      PVOID Cookie = nullptr;
      pe::ic_wstring s;

      if ( const auto u = try_base_name() )
        s.assign(u->begin(), u->end());

      return s;
    }

    inline pe::ic_wstring full_name() const
    {
      PVOID Cookie = nullptr;
      pe::ic_wstring s;

      if ( const auto u = try_full_name() )
        s.assign(u->begin(), u->end());

      return s;
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
