#pragma once
#include <stdint.h>
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>

namespace ntapi
{
  class critical_section
  {
  public:
    using native_handle_type = RTL_CRITICAL_SECTION *;

  private:
    native_handle_type handle;

  public:
    critical_section(RTL_CRITICAL_SECTION *crit)
    {
      this->handle = crit;
    }

    void lock()
    {
      (void)RtlEnterCriticalSection(this->handle);
    }

    bool try_lock()
    {
      return RtlTryEnterCriticalSection(this->handle);
    }

    void unlock()
    {
      (void)RtlLeaveCriticalSection(this->handle);
    }

    native_handle_type native_handle()
    {
      return this->handle;
    }
  };
}
