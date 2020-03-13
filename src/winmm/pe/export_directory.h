#pragma once
#include <ntdll.h>
#include <chrono>

namespace pe
{
  class export_directory : private IMAGE_EXPORT_DIRECTORY
  {
  public:
    export_directory() = delete;
    const class module *module() const;
    class module *module();
    std::chrono::system_clock::time_point timestamp() const;
    const char *name() const;
  };
}
