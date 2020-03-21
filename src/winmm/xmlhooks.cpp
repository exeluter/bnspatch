#include "xmlhooks.h"
#include <ntdll.h>
#include <cstring>
#include <queue>
#include <Shlwapi.h>

#include <sstream>

#include <fmt/format.h>
#include <fastwildcompare.hpp>
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
void convert_impl(XmlElement const *src, pugi::xml_node &dst)
{
  auto node = dst.append_child(src->Name());
  for ( int i = 0; i < src->AttributeCount(); ++i )
    node.append_attribute(src->AttributeName(i)) = src->Attribute(i);

  for ( auto child = src->FirstChildElement(); child; child = child->NextElement() )
    convert_impl(child, node);
}

template <class XmlDoc>
bool xmldoc_to_pugixml(XmlDoc *src, pugi::xml_document &dst)
{
  if ( src && src->IsValid() ) {
    auto decl = dst.prepend_child(pugi::node_declaration);
    decl.append_attribute(xorstr_(L"version")) = xorstr_(L"1.0");
    decl.append_attribute(xorstr_(L"encoding")) = xorstr_(L"UTF-16");

    if ( src->Name() )
      dst.append_child(pugi::node_comment).set_value(src->Name());

    convert_impl(src->Root(), dst);
    return true;
  }
  return false;
}

std::queue<pugi::xml_node> get_xml_patches(wchar_t const *file)
{
  std::queue<pugi::xml_node> queue;
  for ( auto const &patch : g_doc.select_nodes(xorstr_(L"/patches/patch")) ) {
    auto filename = patch.node().attribute(xorstr_(L"file"));
    if ( !filename )
      filename = patch.node().attribute(xorstr_(L"filename"));
    if ( FastWildCompare(filename.value(), file ? file : L"") )
      queue.push(patch.node());
  }
  return queue;
}

void patch_xml(pugi::xml_document &src, std::queue<pugi::xml_node> queue, pugi::xml_writer &writer)
{
  while ( !queue.empty() ) {
    auto const &patch = queue.front();
    process_patch(src.document_element(), patch.children());
    queue.pop();
  }
  src.save(writer, L"", pugi::format_raw, pugi::xml_encoding::encoding_utf16);
}

v13::XmlDoc *(__thiscall *g_pfnReadFromFile13)(v13::XmlReader const *, wchar_t const *);
v13::XmlDoc *__fastcall ReadFromFile13_hook(
  v13::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  wchar_t const *file)
{
  auto queue = get_xml_patches(file);
  if ( queue.empty() )
    return g_pfnReadFromFile13(thisptr, file);

  pugi::xml_document doc;
  if ( !doc.load_file(file) )
    return nullptr;

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer13(thisptr, writer.result.data(), SafeInt(writer.result.size()), file);
}

v14::XmlDoc *(__thiscall *g_pfnReadFromFile14)(v14::XmlReader const *, wchar_t const *, class v14::XmlPieceReader *);
v14::XmlDoc *__fastcall ReadFromFile14_hook(
  v14::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  wchar_t const *file,
  class v14::XmlPieceReader *arg2)
{
  auto queue = get_xml_patches(file);
  if ( queue.empty() )
    return g_pfnReadFromFile14(thisptr, file, arg2);

  pugi::xml_document doc;
  if ( !doc.load_file(file) )
    return nullptr;

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer14(thisptr, writer.result.data(), SafeInt(writer.result.size()), file, arg2);
}

v15::XmlDoc *(__thiscall *g_pfnReadFromFile15)(v15::XmlReader const *, wchar_t const *, class v15::XmlPieceReader *);
v15::XmlDoc *__fastcall ReadFromFile15_hook(
  v15::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  wchar_t const *file,
  class v15::XmlPieceReader *arg2)
{
  auto queue = get_xml_patches(file);
  if ( queue.empty() )
    return g_pfnReadFromFile15(thisptr, file, arg2);

  pugi::xml_document doc;
  if ( !doc.load_file(file) )
    return nullptr;

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer15(thisptr, writer.result.data(), SafeInt(writer.result.size()), file, arg2);
}


v13::XmlDoc *(__thiscall *g_pfnReadFromBuffer13)(v13::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *);
v13::XmlDoc *__fastcall ReadFromBuffer13_hook(
  v13::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file)
{
  if ( !data || !size )
    return nullptr;

  auto queue = get_xml_patches(file);
  if ( queue.empty() )
    return g_pfnReadFromBuffer13(thisptr, data, size, file);

  pugi::xml_document doc;
  if ( size >= sizeof(int64_t)
    && *reinterpret_cast<int64_t const *>(data) == 0x424C534F42584D4Ci64 ) {
    auto xmlDoc = g_pfnReadFromBuffer13(thisptr, data, size, file);
    if ( !xmldoc_to_pugixml(xmlDoc, doc) )
      return xmlDoc;
    thisptr->Close(xmlDoc);
  } else {
    if ( !doc.load_buffer(data, size) )
      return nullptr;
  }

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer13(thisptr, writer.result.data(), SafeInt(writer.result.size()), file);
}

v14::XmlDoc *(__thiscall *g_pfnReadFromBuffer14)(v14::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class v14::XmlPieceReader *);
v14::XmlDoc *__fastcall ReadFromBuffer14_hook(
  v14::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file,
  class v14::XmlPieceReader *arg4)
{
  if ( !data || !size )
    return nullptr;

  auto queue = get_xml_patches(file);
  if ( queue.empty() )
    return g_pfnReadFromBuffer14(thisptr, data, size, file, arg4);

  pugi::xml_document doc;
  if ( size >= sizeof(int64_t)
    && *reinterpret_cast<int64_t const *>(data) == 0x424C534F42584D4Ci64 ) {
    auto xmlDoc = g_pfnReadFromBuffer14(thisptr, data, size, file, arg4);
    if ( !xmldoc_to_pugixml(xmlDoc, doc) )
      return xmlDoc;
    thisptr->Close(xmlDoc);
  } else {
    if ( !doc.load_buffer(data, size) )
      return nullptr;
  }

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer14(thisptr, writer.result.data(), SafeInt(writer.result.size()), file, arg4);
}

v15::XmlDoc *(__thiscall *g_pfnReadFromBuffer15)(v15::XmlReader const *, unsigned char const *, unsigned int, wchar_t const *, class v15::XmlPieceReader *);
v15::XmlDoc *__fastcall ReadFromBuffer15_hook(
  v15::XmlReader const *thisptr,
#ifdef _M_IX86
  intptr_t unused__, // edx
#endif
  unsigned char const *data,
  unsigned int size,
  wchar_t const *file,
  class v15::XmlPieceReader *arg4)
{
  if ( !data || !size )
    return nullptr;

  auto queue = get_xml_patches(file);
  if ( queue.empty() )
    return g_pfnReadFromBuffer15(thisptr, data, size, file, arg4);

  pugi::xml_document doc;
  if ( size >= sizeof(int64_t)
    && *reinterpret_cast<int64_t const *>(data) == 0x424C534F42584D4Ci64 ) {
    auto xmlDoc = g_pfnReadFromBuffer15(thisptr, data, size, file, arg4);
    if ( !xmldoc_to_pugixml(xmlDoc, doc) )
      return xmlDoc;
    thisptr->Close(xmlDoc);
  } else {
    if ( !doc.load_buffer(data, size) )
      return nullptr;
  }

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer15(thisptr, writer.result.data(), SafeInt(writer.result.size()), file, arg4);
}
