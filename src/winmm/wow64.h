#pragma once
#include <ntdll.h>
#include "pe/module.h"

inline void *__ptr32 GetKiFastSystemCallPtr()
{
#ifdef _M_IX86
  return reinterpret_cast<void *__ptr32>(__readfsdword(0xc0));
#else
  return nullptr;
#endif
}

bool IsWow64()
{
  if ( auto const module = pe::get_module(xorstr_(L"ntdll.dll")) ) {
    auto pfnIsWow64Process = reinterpret_cast<BOOL(WINAPI *)(HANDLE, PBOOL)>(
      module->find_function(xorstr_("IsWow64Process")));

    if ( pfnIsWow64Process ) {
      if ( BOOL Wow64Process;
        pfnIsWow64Process(NtCurrentProcess(), &Wow64Process) ) {
        return Wow64Process;
      }
    }
  }
  return false;
}
