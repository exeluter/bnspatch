#pragma once

#include "xmlreader.h"

#ifdef _M_X64
#define thiscall_(name, thisarg, ...) name(thisarg, ## __VA_ARGS__) 
#else
#include <cstdint>
#define thiscall_(name, thisarg, ...) __fastcall name(thisarg, intptr_t, ## __VA_ARGS__) 
#endif

typedef XmlDoc* (__thiscall* PFN_XMLREADER_READMEM)(const XmlReader*, const unsigned char*, unsigned int, const wchar_t*, XmlPieceReader*);
typedef XmlDoc* (__thiscall* PFN_XMLREADER_READFILE)(const XmlReader*, const wchar_t*, XmlPieceReader*);

extern PFN_XMLREADER_READMEM g_pfnClientReadMem;
XmlDoc *thiscall_(ClientReadMem_hook, const XmlReader *thisptr, const unsigned char *mem, unsigned int size, const wchar_t *xmlFileNameForLogging, class XmlPieceReader *xmlPieceReader);

extern PFN_XMLREADER_READFILE g_pfnClientReadFile;
XmlDoc *thiscall_(ClientReadFile_hook, const XmlReader *thisptr, const wchar_t *xml, class XmlPieceReader *xmlPieceReader);

extern PFN_XMLREADER_READFILE g_pfnEngineReadFile;
XmlDoc *thiscall_(EngineReadFile_hook, const XmlReader *thisptr, const wchar_t *xml, class XmlPieceReader *xmlPieceReader);

extern PFN_XMLREADER_READMEM g_pfnEngineReadMem;
XmlDoc *thiscall_(EngineReadMem_hook, const XmlReader *thisptr, const unsigned char *mem, unsigned int size, const wchar_t *xmlFileNameForLogging, class XmlPieceReader *xmlPieceReader);
