#pragma once
#include <ntdll.h>
#include "xmlreader.h"

extern v13::XmlDoc *(__thiscall *g_pfnRead13)(v13::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *);
v13::XmlDoc *__fastcall Read13_hook(
  v13::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file);

extern v14::XmlDoc *(__thiscall *g_pfnRead14)(v14::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class v14::XmlPieceReader *);
v14::XmlDoc *__fastcall Read14_hook(
  v14::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file,
  class v14::XmlPieceReader *arg4);

extern v15::XmlDoc *(__thiscall *g_pfnRead15)(v15::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class v15::XmlPieceReader *);
v15::XmlDoc *__fastcall Read15_hook(
  v15::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file,
  class v15::XmlPieceReader *arg4);

extern v13::XmlReader *(*g_pfnCreateXmlReader13)(void);
v13::XmlReader *CreateXmlReader13_hook(void);
extern v14::XmlReader *(*g_pfnCreateXmlReader14)(void);
v14::XmlReader *CreateXmlReader14_hook(void);
extern v15::XmlReader *(*g_pfnCreateXmlReader15)(void);
v15::XmlReader *CreateXmlReader15_hook(void);

extern void(*g_pfnDestroyXmlReader13)(v13::XmlReader *);
void DestroyXmlReader13_hook(v13::XmlReader *xmlReader);
extern void(*g_pfnDestroyXmlReader14)(v14::XmlReader *);
void DestroyXmlReader14_hook(v14::XmlReader *xmlReader);
extern void(*g_pfnDestroyXmlReader15)(v15::XmlReader *);
void DestroyXmlReader15_hook(v15::XmlReader *xmlReader);
