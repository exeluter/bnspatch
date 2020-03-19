#pragma once
#include <ntdll.h>
#include <string_view>
#include <gsl/span>

#include "..\ic_char_traits.h"

namespace pe
{
  class module : private IMAGE_DOS_HEADER
  {
  public:
    module() = delete;
    uintptr_t handle() const;
    operator HINSTANCE() const;
    template <class T>
    inline auto rva_to(uint32_t rva)
    {
      return reinterpret_cast<T *>(reinterpret_cast<uintptr_t>(this) + rva);
    }
    template <class T>
    inline auto rva_to(uint32_t rva) const
    {
      return reinterpret_cast<const T *>(reinterpret_cast<uintptr_t>(this) + rva);
    }
    ic_wstring base_name() const;
    ic_wstring full_name() const;
    IMAGE_DOS_HEADER *dos_header();
    const IMAGE_DOS_HEADER *dos_header() const;
    IMAGE_NT_HEADERS *nt_header();
    const IMAGE_NT_HEADERS *nt_header() const;
    size_t size() const;
    gsl::span<class segment> segments();
    gsl::span<const class segment> segments() const;
    class segment *segment(const std::string_view &name);
    const class segment *segment(const std::string_view &name) const;
    class export_directory *export_directory();
    const class export_directory *export_directory() const;
    void *find_function(const char *name) const;
    void *find_function(uint32_t num) const;
  };
  class module *get_module(const wchar_t *name = nullptr);
  class module *get_module_from_address(void *pc);
  const class module *get_module_from_address(const void *pc);
  class module *instance_module();
}
