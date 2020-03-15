#pragma once
#include "export_directory.h"
#include "module.h"
#include <ntdll.h>
#include <chrono>

namespace pe
{
  const module *export_directory::module() const
  {
    return get_module_from_address(this);
  }

  module *export_directory::module()
  {
    return get_module_from_address(this);
  }

  std::chrono::system_clock::time_point export_directory::timestamp() const
  {
    return std::chrono::system_clock::from_time_t(this->TimeDateStamp);
  }

  const char *export_directory::name() const
  {
    return this->module()->rva_to<const char>(this->Name);
  }
}
