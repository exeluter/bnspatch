#include "xmlhooks.h"
#include <ntdll.h>
#include <cstring>
#include <queue>
#include <Shlwapi.h>

#include <fastwildcompare.hpp>
#include <gsl/span>
#include <gsl/span_ext>
#include <safeint.hpp>
#include <xorstr.hpp>

#include "xmlpatch.h"
#include "xmlreader.h"

template <class XmlElement>
void Convert(XmlElement const *src, pugi::xml_node &dst)
{
  auto node = dst.append_child(src->Name());
  for ( int i = 0; i < src->AttributeCount(); ++i )
    node.append_attribute(src->AttributeName(i)) = src->Attribute(i);
  for ( auto child = src->FirstChildElement(); child; child = child->NextElement() )
    Convert(child, node);
}

template <class XmlElement>
bool Patch(wchar_t const *DocumentName, XmlElement *root, wchar_t const *file, std::array<WCHAR, MAX_PATH> &tempFile)
{
  std::queue<pugi::xml_node> queue;
  for ( auto const &x : g_PatchesDocument.select_nodes(xorstr_(L"/patches/patch")) ) {
    if ( FastWildCompare(x.node().attribute(xorstr_(L"file")).value(), file ? file : L"") )
      queue.push(x.node());
  }
  if ( !queue.empty() ) {
    pugi::xml_document doc;
    auto decl = doc.prepend_child(pugi::node_declaration);
    decl.append_attribute(xorstr_(L"version")) = xorstr_(L"1.0");

#ifdef NDEBUG
    int flags = pugi::format_raw;
#else
    int flags = pugi::format_default;
#endif
    auto encoding = pugi::xml_encoding::encoding_auto;

    if ( file && !_wcsicmp(PathFindExtensionW(file), xorstr_(L".x16")) ) {
      decl.append_attribute(xorstr_(L"encoding")) = xorstr_(L"utf-16");
      flags |= pugi::format_write_bom;
      encoding = pugi::xml_encoding::encoding_utf16;
    } else {
      decl.append_attribute(xorstr_(L"encoding")) = xorstr_(L"utf-8");
    }
    if ( DocumentName )
      doc.append_child(pugi::node_comment).set_value(DocumentName);

    Convert(root, doc);

    do {
      auto const &patch = queue.front();
      ProcessPatch(doc.document_element(), patch.children());
      queue.pop();
    } while ( !queue.empty() );

    std::array<WCHAR, MAX_PATH> TempPath;

    return GetTempPathW(SafeInt(TempPath.size()), TempPath.data())
      && GetTempFileNameW(TempPath.data(), xorstr_(L"bns"), 0, tempFile.data())
      && doc.save_file(tempFile.data(), xorstr_(L"  "), flags, encoding);
  }
  return false;
}

v14::XmlDoc *(__thiscall *g_pfnRead14)(v14::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class v14::XmlPieceReader *);
v14::XmlDoc *__fastcall Read14_hook(
  v14::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file,
  class v14::XmlPieceReader *arg4)
{
  std::array<WCHAR, MAX_PATH> tempFile;
  auto Document = g_pfnRead14(thisptr, data, size, file, arg4);

  if ( Document
    && Document->IsValid()
    && Patch(Document->Name(), Document->Root(), file, tempFile) ) {
    thisptr->Close(Document);
    Document = thisptr->Read(tempFile.data(), arg4);
#ifdef NDEBUG
    DeleteFileW(tempFile.data());
#endif
  }
  return Document;
}

v13::XmlDoc *(__thiscall *g_pfnRead13)(v13::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *);
v13::XmlDoc *__fastcall Read13_hook(
  v13::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file)
{
  auto Document = g_pfnRead13(thisptr, data, size, file);
  std::array<WCHAR, MAX_PATH> tempFile;

  if ( Document
    && Document->IsValid()
    && Patch(Document->Name(), Document->Root(), file, tempFile) ) {

    thisptr->Close(Document);
    Document = thisptr->Read(tempFile.data());
#ifdef NDEBUG
    DeleteFileW(tempFile.data());
#endif
  }
  return Document;
}

v15::XmlDoc *(__thiscall *g_pfnRead15)(v15::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class v15::XmlPieceReader *);
v15::XmlDoc *__fastcall Read15_hook(
  v15::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file,
  class v15::XmlPieceReader *arg4)
{
  std::array<WCHAR, MAX_PATH> tempFile;
  auto Document = g_pfnRead15(thisptr, data, size, file, arg4);

  if ( Document
    && Document->IsValid()
    && Patch(Document->Name(), Document->Root(), file, tempFile) ) {
    thisptr->Close(Document);
    Document = thisptr->Read(tempFile.data(), arg4);
#ifdef NDEBUG
    DeleteFileW(tempFile.data());
#endif
  }
  return Document;
}

v13::XmlReader *(*g_pfnCreateXmlReader13)(void);
v13::XmlReader *CreateXmlReader13_hook(void)
{
  auto ptr = g_pfnCreateXmlReader13();
  auto vftable = gsl::make_span(*reinterpret_cast<void ***>(ptr), 12);
  auto vfcopy = new void *[vftable.size()];
  std::copy(vftable.begin(), vftable.end(), vfcopy);
  g_pfnRead13 = reinterpret_cast<decltype(g_pfnRead13)>(InterlockedExchangePointer(&vfcopy[7], &Read13_hook));
  *reinterpret_cast<void ***>(ptr) = vfcopy;
  return ptr;
}

v14::XmlReader *(*g_pfnCreateXmlReader14)(void);
v14::XmlReader *CreateXmlReader14_hook(void)
{
  auto ptr = g_pfnCreateXmlReader14();
  auto vftable = gsl::make_span(*reinterpret_cast<void ***>(ptr), 12);
  auto vfcopy = new void *[vftable.size()];
  std::copy(vftable.begin(), vftable.end(), vfcopy);
  g_pfnRead14 = reinterpret_cast<decltype(g_pfnRead14)>(InterlockedExchangePointer(&vfcopy[7], &Read14_hook));
  *reinterpret_cast<void ***>(ptr) = vfcopy;
  return ptr;
}

v15::XmlReader *(*g_pfnCreateXmlReader15)(void);
v15::XmlReader *CreateXmlReader15_hook(void)
{
  auto ptr = g_pfnCreateXmlReader15();
  auto vftable = gsl::make_span(*reinterpret_cast<void ***>(ptr), 12);
  auto vfcopy = new void *[vftable.size()];
  std::copy(vftable.begin(), vftable.end(), vfcopy);
  g_pfnRead15 = reinterpret_cast<decltype(g_pfnRead15)>(InterlockedExchangePointer(&vfcopy[7], &Read15_hook));
  *reinterpret_cast<void ***>(ptr) = vfcopy;
  return ptr;
}

void(*g_pfnDestroyXmlReader14)(v14::XmlReader *);
void DestroyXmlReader14_hook(v14::XmlReader *xmlReader)
{
  auto vfcopy = *reinterpret_cast<void ***>(xmlReader);
  g_pfnDestroyXmlReader14(xmlReader);
  delete[] vfcopy;
}

void(*g_pfnDestroyXmlReader13)(v13::XmlReader *);
void DestroyXmlReader13_hook(v13::XmlReader *xmlReader)
{
  auto vfcopy = *reinterpret_cast<void ***>(xmlReader);
  g_pfnDestroyXmlReader13(xmlReader);
  delete[] vfcopy;
}

void(*g_pfnDestroyXmlReader15)(v15::XmlReader *);
void DestroyXmlReader15_hook(v15::XmlReader *xmlReader)
{
  auto vfcopy = *reinterpret_cast<void ***>(xmlReader);
  g_pfnDestroyXmlReader15(xmlReader);
  delete[] vfcopy;
}
