#pragma once

#include <Windows.h>
#include <type_traits>
#include <detours.h>
#include <wil/result.h>

template<class Fn, typename = std::enable_if_t<std::is_function_v<Fn>>>
inline LONG WINAPI DetourAttach(HMODULE hModule, PCSTR pProcName, Fn **ppPointer, Fn *pDetour)
{
  if ( !hModule ) return ERROR_INVALID_PARAMETER;
  if ( !ppPointer ) return ERROR_INVALID_PARAMETER;

  *ppPointer = reinterpret_cast<Fn *>(GetProcAddress(hModule, pProcName));
  RETURN_LAST_ERROR_IF_NULL(*ppPointer);

  return DetourAttachEx(reinterpret_cast<PVOID *>(ppPointer), pDetour, nullptr, nullptr, nullptr);
}

template<class Fn, typename = std::enable_if_t<std::is_function_v<Fn>>>
inline LONG WINAPI DetourAttach(PCWSTR pModuleName, PCSTR pProcName, Fn **ppPointer, Fn *pDetour)
{
  if ( !pModuleName ) return ERROR_INVALID_PARAMETER;

  const auto hModule = GetModuleHandleW(pModuleName);
  RETURN_LAST_ERROR_IF_NULL(hModule);

  return DetourAttach(hModule, pProcName, ppPointer, pDetour);
}

template<class Fn, typename = std::enable_if_t<std::is_function_v<Fn>>>
inline LONG WINAPI DetourAttach(PCSTR pModuleName, PCSTR pProcName, Fn **ppPointer, Fn *pDetour)
{
  if ( !pModuleName ) return ERROR_INVALID_PARAMETER;

  const auto hModule = GetModuleHandleA(pModuleName);
  RETURN_LAST_ERROR_IF_NULL(hModule);

  return DetourAttach(hModule, pProcName, ppPointer, pDetour);
}
