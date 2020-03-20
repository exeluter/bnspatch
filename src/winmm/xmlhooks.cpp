#include "xmlhooks.h"
#include <ntdll.h>
#include <cstring>
#include <queue>
#include <Shlwapi.h>

#include <sstream>

#include <fmt/format.h>
#include <fastwildcompare.hpp>
#include <gsl/span>
#include <gsl/span_ext>
#include <safeint.hpp>
#ifdef NDEBUG
#include <xorstr.hpp>
#else
#define xorstr_(str) (str)
#endif

#include "xmlpatch.h"
#include "xmlreader.h"

template<class Char>
struct memory_buffer_writer : pugi::xml_writer
{
  fmt::basic_memory_buffer<Char> result;

  virtual void write(const void *data, size_t size)
  {
    result.append(reinterpret_cast<const Char *>(data),
      reinterpret_cast<const Char *>(reinterpret_cast<const unsigned char *>(data) + size));
  }
};

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
bool Patch(wchar_t const *DocumentName, XmlElement *root, wchar_t const *file, pugi::xml_writer &writer)
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
    decl.append_attribute(xorstr_(L"encoding")) = xorstr_(L"UTF-16");

    if ( DocumentName )
      doc.append_child(pugi::node_comment).set_value(DocumentName);

    Convert(root, doc);

    do {
      auto const &patch = queue.front();
      ProcessPatch(doc.document_element(), patch.children());
      queue.pop();
    } while ( !queue.empty() );

    doc.save(writer, L"", pugi::format_raw, pugi::xml_encoding::encoding_utf16);
    return true;
  }
  return false;
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

  if ( Document && Document->IsValid() ) {
    memory_buffer_writer<unsigned char> writer;
    if ( Patch(Document->Name(), Document->Root(), file, writer) ) {
      thisptr->Close(Document);
      Document = g_pfnRead13(thisptr, writer.result.data(), SafeInt(writer.result.size()), file);
    }
  }
  return Document;
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
  auto Document = g_pfnRead14(thisptr, data, size, file, arg4);

  if ( Document && Document->IsValid() ) {
    memory_buffer_writer<unsigned char> writer;
    if ( Patch(Document->Name(), Document->Root(), file, writer) ) {
      thisptr->Close(Document);
      Document = g_pfnRead14(thisptr, writer.result.data(), SafeInt(writer.result.size()), file, arg4);
    }
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
  auto Document = g_pfnRead15(thisptr, data, size, file, arg4);

  if ( Document && Document->IsValid() ) {
    memory_buffer_writer<unsigned char> writer;
    if ( Patch(Document->Name(), Document->Root(), file, writer) ) {
      thisptr->Close(Document);
      Document = g_pfnRead15(thisptr, writer.result.data(), SafeInt(writer.result.size()), file, arg4);
    }
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

void(*g_pfnDestroyXmlReader13)(v13::XmlReader *);
void DestroyXmlReader13_hook(v13::XmlReader *xmlReader)
{
  auto vfcopy = *reinterpret_cast<void ***>(xmlReader);
  g_pfnDestroyXmlReader13(xmlReader);
  delete[] vfcopy;
}

void(*g_pfnDestroyXmlReader14)(v14::XmlReader *);
void DestroyXmlReader14_hook(v14::XmlReader *xmlReader)
{
  auto vfcopy = *reinterpret_cast<void ***>(xmlReader);
  g_pfnDestroyXmlReader14(xmlReader);
  delete[] vfcopy;
}

void(*g_pfnDestroyXmlReader15)(v15::XmlReader *);
void DestroyXmlReader15_hook(v15::XmlReader *xmlReader)
{
  auto vfcopy = *reinterpret_cast<void ***>(xmlReader);
  g_pfnDestroyXmlReader15(xmlReader);
  delete[] vfcopy;
}
