#include <ntdll.h>
#include <stdio.h>
#include <delayimp.h>

#include <pe/module.h>
#include <pe/export_directory.h>

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD fdwReason, LPVOID lpvReserved)
{
  return TRUE;
}

ExternC const PfnDliHook __pfnDliNotifyHook2 = [](unsigned dliNotify, PDelayLoadInfo pdli) -> FARPROC
{
  pe::module* module;
  wchar_t fname[_MAX_FNAME];
  wchar_t ext[_MAX_EXT];
  wchar_t path[_MAX_PATH];
  wchar_t buffer[_MAX_FNAME];
  int num = -1;
  HMODULE result;
  UINT count;

  if (dliNotify == dliNotePreLoadLibrary) {
    module = pe::instance_module();
    if (!_stricmp(pdli->szDll, module->export_directory()->name())) {
      if (GetModuleFileNameW(module, path, _countof(path))
        && !_wsplitpath_s(path, nullptr, 0, nullptr, 0, fname, _countof(fname), ext, _countof(ext))
        && swscanf_s(fname, L"%[^0-9]%d", buffer, (unsigned int)_countof(buffer), &num) > 0
        && swprintf_s(fname, L"%s%03d", buffer, num + 1) >= 0
        && !_wmakepath_s(path, _countof(path), nullptr, nullptr, fname, ext)) {
        result = LoadLibraryExW(path, nullptr, LOAD_LIBRARY_SEARCH_APPLICATION_DIR);
        if (result)
          return (FARPROC)result;
      }
      count = GetSystemDirectoryW(path, _countof(path));
      if (count && swprintf_s(path + count, _countof(path) - count, L"\\%hs", pdli->szDll))
        return (FARPROC)LoadLibraryExW(path, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
    }
  }
  return nullptr;
};
