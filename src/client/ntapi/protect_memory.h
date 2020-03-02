#pragma once
#include <stdint.h>
#include <phnt/phnt_windows.h>
#include <phnt/phnt.h>

namespace ntapi
{
  class protect_memory
  {
    PVOID BaseAddress;
    SIZE_T RegionSize;
    ULONG OldProtect;
    NTSTATUS Status;

  public:
    protect_memory(PVOID BaseAddress, SIZE_T RegionSize, ULONG NewProtect) noexcept
    {
      this->BaseAddress = BaseAddress;
      this->RegionSize = RegionSize;
      this->OldProtect = 0;
      this->Status = NtProtectVirtualMemory(
        NtCurrentProcess(),
        &this->BaseAddress,
        &this->RegionSize,
        NewProtect,
        &this->OldProtect);
    }

    operator bool() const noexcept
    {
      return NT_SUCCESS(Status);
    }

    ~protect_memory() noexcept
    {
      if ( NT_SUCCESS(Status) ) {
        (void)NtProtectVirtualMemory(
          NtCurrentProcess(),
          &this->BaseAddress,
          &this->RegionSize,
          this->OldProtect,
          &this->OldProtect);
      }
    }
  };
}
