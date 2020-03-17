#pragma once
#include "export_directory.h"
#include "module.h"
#include <ntdll.h>

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

  __time32_t export_directory::timestamp() const
  {
    return this->TimeDateStamp;
  }

  const char *export_directory::name() const
  {
    return this->module()->rva_to<const char>(this->Name);
  }
}
