#pragma once
#include <ntdll.h>

namespace pe
{
  class export_directory : private IMAGE_EXPORT_DIRECTORY
  {
  public:
    export_directory() = delete;
    const class module *module() const;
    class module *module();
    __time32_t timestamp() const;
    const char *name() const;
  };
}
