#pragma once
#include <phnt_windows.h>
#include <phnt.h>

#include <cstdint>

#include "xmlreader.h"

#ifdef _M_X64
#define THISCALL
#define THISPTR(x) x thisptr
#else
#define THISCALL __fastcall
#define THISPTR(x) x thisptr, intptr_t
#endif

extern v13::XmlDoc *(__thiscall *g_pfnReadFromFile13)(v13::XmlReader const *, const wchar_t *);
v13::XmlDoc *THISCALL ReadFromFile13_hook(THISPTR(v13::XmlReader const *), const wchar_t *xmlFileNameForLogging);

extern v14::XmlDoc *(__thiscall *g_pfnReadFromFile14)(v14::XmlReader const *, const wchar_t *, class v14::XmlPieceReader *);
v14::XmlDoc *THISCALL ReadFromFile14_hook(THISPTR(v14::XmlReader const *), const wchar_t *xmlFileNameForLogging, class v14::XmlPieceReader *xmlPieceReader);

extern v13::XmlDoc *(__thiscall *g_pfnReadFromBuffer13)(v13::XmlReader const *, unsigned char const *, unsigned int, const wchar_t *);
v13::XmlDoc *THISCALL ReadFromBuffer13_hook(THISPTR(v13::XmlReader const *), unsigned char const *mem, unsigned int size, const wchar_t *file);

extern v14::XmlDoc *(__thiscall *g_pfnReadFromBuffer14)(v14::XmlReader const *, unsigned char const *, unsigned int, const wchar_t *, class v14::XmlPieceReader *);
v14::XmlDoc *THISCALL ReadFromBuffer14_hook(THISPTR(v14::XmlReader const *), unsigned char const *mem, unsigned int size, const wchar_t *xmlFileNameForLogging, class v14::XmlPieceReader *xmlPieceReader);
