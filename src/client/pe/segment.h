#pragma once
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>
#include <gsl/span>
#include "pe.h"

namespace pe
{
  class module;

  class segment : public IMAGE_SECTION_HEADER
  {
  public:
    segment() = delete;

    inline const pe::module *module() const
    {
      return pe::get_module_from_address(this);
    }

    inline pe::module *module()
    {
      return pe::get_module_from_address(this);
    }

    inline std::string_view name() const
    {
      size_t count;
      for ( count = 0; count < IMAGE_SIZEOF_SHORT_NAME; ++count ) {
        if ( !this->Name[count] )
          break;
      }
      return { (char *)this->Name, count };
    }

    inline gsl::span<uint8_t> as_bytes()
    {
      if ( auto module = this->module() ) {
        return gsl::make_span(reinterpret_cast<uint8_t *>(
          reinterpret_cast<uintptr_t>(module) + this->VirtualAddress), this->Misc.VirtualSize);
      }
      return {};
    }

    inline gsl::span<const uint8_t> as_bytes() const
    {
      if ( auto module = this->module() ) {
        return gsl::make_span(reinterpret_cast<const uint8_t *>(
          reinterpret_cast<uintptr_t>(module) + this->VirtualAddress), this->Misc.VirtualSize);
      }
      return {};
    }

    inline bool contains_code() const
    {
      return this->Characteristics & IMAGE_SCN_CNT_CODE;
    }

    inline bool contains_initialized_data() const
    {
      return this->Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA;
    }

    inline bool contains_uninitialized_data() const
    {
      return this->Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA;
    }

    inline uint32_t relocation_count() const
    {
      if ( (this->Characteristics & IMAGE_SCN_LNK_NRELOC_OVFL)
        && this->NumberOfRelocations == 0xFFFF ) {

        return reinterpret_cast<const IMAGE_RELOCATION *>(
          reinterpret_cast<uintptr_t>(this->module()) + this->PointerToRelocations)->RelocCount;
      }
      return this->NumberOfRelocations;
    }

    inline bool discardable() const
    {
      return this->Characteristics & IMAGE_SCN_MEM_DISCARDABLE;
    }

    inline bool not_cached() const
    {
      return this->Characteristics & IMAGE_SCN_MEM_NOT_CACHED;
    }

    inline bool not_paged() const
    {
      return this->Characteristics & IMAGE_SCN_MEM_NOT_PAGED;
    }

    inline bool shared() const
    {
      return this->Characteristics & IMAGE_SCN_MEM_SHARED;
    }

    inline bool executable() const
    {
      return this->Characteristics & IMAGE_SCN_MEM_EXECUTE;
    }

    inline bool readable() const
    {
      return this->Characteristics & IMAGE_SCN_MEM_READ;
    }

    inline bool writable() const
    {
      return this->Characteristics & IMAGE_SCN_MEM_WRITE;
    }
  };
}