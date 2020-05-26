#pragma once
#include <ntdll.h>
#include "xmlreader.h"

extern v13::XmlDoc *(__thiscall *g_pfnReadFromFile13)(v13::XmlReader const *, wchar_t const *);
v13::XmlDoc *__fastcall ReadFromFile13_hook(
  v13::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  wchar_t const *file);

extern v14::XmlDoc *(__thiscall *g_pfnReadFromFile14)(v14::XmlReader const *, wchar_t const *, class v14::XmlPieceReader *);
v14::XmlDoc *__fastcall ReadFromFile14_hook(
  v14::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  wchar_t const *file,
  class v14::XmlPieceReader *arg2);

extern v15::XmlDoc *(__thiscall *g_pfnReadFromFile15)(v15::XmlReader const *, wchar_t const *, class v15::XmlPieceReader *);
v15::XmlDoc *__fastcall ReadFromFile15_hook(
  v15::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  wchar_t const *file,
  class v15::XmlPieceReader *arg2);

extern v13::XmlDoc *(__thiscall *g_pfnReadFromBuffer13)(v13::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *);
v13::XmlDoc *__fastcall ReadFromBuffer13_hook(
  v13::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file);

extern v14::XmlDoc *(__thiscall *g_pfnReadFromBuffer14)(v14::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class v14::XmlPieceReader *);
v14::XmlDoc *__fastcall ReadFromBuffer14_hook(
  v14::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file,
  class v14::XmlPieceReader *arg4);

extern v15::XmlDoc *(__thiscall *g_pfnReadFromBuffer15)(v15::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class v15::XmlPieceReader *);
v15::XmlDoc *__fastcall ReadFromBuffer15_hook(
  v15::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file,
  class v15::XmlPieceReader *arg4);
