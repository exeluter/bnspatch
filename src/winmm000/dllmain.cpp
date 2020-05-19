#include <Windows.h>
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
  const char* name;
  wchar_t drive[_MAX_DRIVE];
  wchar_t dir[_MAX_DIR];
  wchar_t fname[_MAX_FNAME];
  wchar_t ext[_MAX_EXT];
  wchar_t path[_MAX_PATH];
  wchar_t buffer[_MAX_FNAME];
  unsigned num = UINT_MAX;
  HMODULE result;
  
  switch (dliNotify) {
  case dliStartProcessing:
    break;
  case dliNotePreLoadLibrary:
    module = pe::instance_module();
    name = module->export_directory()->name();
    if (!_stricmp(pdli->szDll, name)) {
      if (GetModuleFileNameW(module, path, _countof(path))
        && !_wsplitpath_s(path, drive, dir, fname, ext)
        && swscanf_s(fname, L"%ls%u", buffer, (unsigned int)_countof(buffer), &num) > 0
        && swprintf_s(fname, L"%ls%03u", buffer, num + 1) >= 0
        && !_wmakepath_s(path, drive, dir, fname, ext)) {
        result = LoadLibraryExW(path, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
        if (result)
          return (FARPROC)result;
      }
      if (GetSystemDirectoryW(path, (UINT)_countof(path))
        && swprintf_s(path, L"\\%hs", name) >= 0) {
        return (FARPROC)LoadLibraryExW(path, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);
      }
    }
    break;
  case dliNotePreGetProcAddress:
  case dliFailLoadLib:
  case dliFailGetProc:
  case dliNoteEndProcessing:
    break;
  }
  return nullptr;
};
