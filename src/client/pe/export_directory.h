#pragma once
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>
#include <chrono>
#include "pe.h"

namespace pe
{
  class module;

  class export_directory : private IMAGE_EXPORT_DIRECTORY
  {
  public:
    export_directory() = delete;

    inline const pe::module *module() const
    {
      return pe::get_module_from_address(this);
    }

    inline pe::module *module()
    {
      return pe::get_module_from_address(this);
    }

    inline std::chrono::system_clock::time_point timestamp() const
    {
      return std::chrono::system_clock::from_time_t(this->TimeDateStamp);
    }

    inline const char *name() const
    {
      return this->module()->rva_to<const char>(this->Name);
    }
  };
}