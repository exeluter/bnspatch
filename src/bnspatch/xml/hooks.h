#pragma once

#include "xmlreader.h"

typedef XmlDoc* (__thiscall* PFN_XMLREADER_READMEM)(const XmlReader*, const unsigned char*, unsigned int, const wchar_t*, XmlPieceReader*);
typedef XmlDoc* (__thiscall* PFN_XMLREADER_READFILE)(const XmlReader*, const wchar_t*, XmlPieceReader*);
typedef XmlReader* (__cdecl* PFN_CREATEXMLREADER)();

extern PFN_CREATEXMLREADER g_pfnEngineCreateXmlReader;
XmlReader* __cdecl EngineCreateXmlReader_hook();

extern PFN_CREATEXMLREADER g_pfnClientCreateXmlReader;
XmlReader* __cdecl ClientCreateXmlReader_hook();
