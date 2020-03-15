#pragma once
#include <ntdll.h>
#include <gsl/span>
#include <gsl/span_ext>
#include "module.h"
#include "segment.h"

namespace pe
{
  const class module *segment::module() const
  {
    return get_module_from_address(this);
  }

  class module *segment::module()
  {
    return get_module_from_address(this);
  }

  std::string_view segment::name() const
  {
    size_t count;
    for ( count = 0; count < IMAGE_SIZEOF_SHORT_NAME; ++count ) {
      if ( !this->Name[count] )
        break;
    }
    return { reinterpret_cast<const char *>(this->Name), count };
  }

  gsl::span<uint8_t> segment::as_bytes()
  {
    if ( auto module = this->module() ) {
      return gsl::make_span(reinterpret_cast<uint8_t *>(
        reinterpret_cast<uintptr_t>(module) + this->VirtualAddress), this->Misc.VirtualSize);
    }
    return {};
  }

  gsl::span<const uint8_t> segment::as_bytes() const
  {
    if ( auto module = this->module() ) {
      return gsl::make_span(reinterpret_cast<const uint8_t *>(
        reinterpret_cast<uintptr_t>(module) + this->VirtualAddress), this->Misc.VirtualSize);
    }
    return {};
  }

  bool segment::contains_code() const
  {
    return this->Characteristics & IMAGE_SCN_CNT_CODE;
  }

  bool segment::contains_initialized_data() const
  {
    return this->Characteristics & IMAGE_SCN_CNT_INITIALIZED_DATA;
  }

  bool segment::contains_uninitialized_data() const
  {
    return this->Characteristics & IMAGE_SCN_CNT_UNINITIALIZED_DATA;
  }

  uint32_t segment::relocation_count() const
  {
    if ( (this->Characteristics & IMAGE_SCN_LNK_NRELOC_OVFL)
      && this->NumberOfRelocations == 0xFFFF ) {

      return reinterpret_cast<const IMAGE_RELOCATION *>(
        reinterpret_cast<uintptr_t>(this->module()) + this->PointerToRelocations)->RelocCount;
    }
    return this->NumberOfRelocations;
  }

  bool segment::discardable() const
  {
    return this->Characteristics & IMAGE_SCN_MEM_DISCARDABLE;
  }

  bool segment::not_cached() const
  {
    return this->Characteristics & IMAGE_SCN_MEM_NOT_CACHED;
  }

  bool segment::not_paged() const
  {
    return this->Characteristics & IMAGE_SCN_MEM_NOT_PAGED;
  }

  bool segment::shared() const
  {
    return this->Characteristics & IMAGE_SCN_MEM_SHARED;
  }

  bool segment::executable() const
  {
    return this->Characteristics & IMAGE_SCN_MEM_EXECUTE;
  }

  bool segment::readable() const
  {
    return this->Characteristics & IMAGE_SCN_MEM_READ;
  }

  bool segment::writable() const
  {
    return this->Characteristics & IMAGE_SCN_MEM_WRITE;
  }
}
