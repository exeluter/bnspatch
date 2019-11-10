#include "pch.h"
#include "dllname.h"

#define EXPORT_DIRECTORY OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT]

PCSTR GetDllName(LPCVOID BaseOfImage)
{
  if ( !BaseOfImage )
    BaseOfImage = GetModuleHandleW(nullptr);

  const auto DosHeader = (const PIMAGE_DOS_HEADER)BaseOfImage;
  if ( DosHeader->e_magic != IMAGE_DOS_SIGNATURE )
    return nullptr;

  auto NtHeaders = (const PIMAGE_NT_HEADERS)((uintptr_t)BaseOfImage + DosHeader->e_lfanew);
  if ( NtHeaders->Signature != IMAGE_NT_SIGNATURE )
    return nullptr;

  if ( !NtHeaders->FileHeader.SizeOfOptionalHeader
    || NtHeaders->OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR_MAGIC
    || !NtHeaders->EXPORT_DIRECTORY.VirtualAddress
    || !NtHeaders->EXPORT_DIRECTORY.Size )
    return nullptr;

  auto ExportDirectory = (const PIMAGE_EXPORT_DIRECTORY)((uintptr_t)BaseOfImage +
    NtHeaders->EXPORT_DIRECTORY.VirtualAddress);
  if ( !ExportDirectory->Name )
    return nullptr;

  return (PCSTR)((uintptr_t)BaseOfImage + ExportDirectory->Name);
}
