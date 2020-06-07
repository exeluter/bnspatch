#include "xmlhooks.h"
#include <phnt_windows.h>
#include <phnt.h>
#include <cstring>
#include <queue>
#include <Shlwapi.h>

#include <sstream>

#include <fmt/format.h>
#include <fastwildcompare.hpp>
#include <pugixml.hpp>
#include <safeint.hpp>
#ifdef NDEBUG
#include <xorstr.hpp>
#else
#define xorstr_(str) (str)
#endif

#include "xmlpatch.h"
#include "xmlreader.h"
#include <filesystem>

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
  // TODO: text
  for ( auto child = src->FirstChildElement(); child; child = child->NextElement() )
    convert_impl(child, node);
}

template <class XmlDoc>
bool xmldoc_to_pugixml(XmlDoc *src, pugi::xml_document &dst)
{
  if ( src && src->IsValid() ) {
    auto decl = dst.prepend_child(pugi::node_declaration);
    decl.append_attribute(xorstr_(L"version")) = xorstr_(L"1.0");
    decl.append_attribute(xorstr_(L"encoding")) = xorstr_(L"utf-16");

    if ( src->Name() )
      dst.append_child(pugi::node_comment).set_value(src->Name());

    convert_impl(src->Root(), dst);
    return true;
  }
  return false;
}

std::queue<pugi::xml_node> get_xml_patches(const wchar_t *xmlFileNameForLogging)
{
  auto queue = std::queue<pugi::xml_node>();

  for ( auto const &patch : patches_document().document_element().children(xorstr_(L"patch")) ) {
    auto attribute = patch.attribute(xorstr_(L"file"));
    if ( !attribute )
      attribute = patch.attribute(xorstr_(L"filename"));

    auto file = std::filesystem::path(attribute.value());
    auto xml_file = std::filesystem::path(xmlFileNameForLogging ? xmlFileNameForLogging : L"");
    attribute.value();

    if ( (!file.has_parent_path() && FastWildCompare(file.filename().c_str(), xml_file.filename().c_str()))
      || FastWildCompare(file.c_str(), xml_file.c_str()) )
      queue.push(patch);
  }
  return queue;
}

void patch_xml(pugi::xml_document &src, std::queue<pugi::xml_node> queue, pugi::xml_writer &writer)
{
  while ( !queue.empty() ) {
    auto const &patch = queue.front();
    std::unordered_map<fnv1a::type, pugi::xml_node> saved_nodes;
    process_patch(src.document_element(), patch.children(), saved_nodes);
    queue.pop();
  }
  src.save(writer, L"", pugi::format_raw, pugi::xml_encoding::encoding_utf16);
}

v13::XmlDoc *(__thiscall *g_pfnReadFromFile13)(const v13::XmlReader *a, const wchar_t *);
v13::XmlDoc *THISCALL ReadFromFile13_hook(THISPTR(const v13::XmlReader *), const wchar_t *xmlFileNameForLogging)
{
  auto queue = get_xml_patches(xmlFileNameForLogging);
  if ( queue.empty() )
    return g_pfnReadFromFile13(thisptr, xmlFileNameForLogging);

  pugi::xml_document doc;
  if ( !doc.load_file(xmlFileNameForLogging) )
    return nullptr;

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer13(thisptr, writer.result.data(), SafeInt(writer.result.size()), xmlFileNameForLogging);
}

v14::XmlDoc *(__thiscall *g_pfnReadFromFile14)(const v14::XmlReader *, const wchar_t *, class v14::XmlPieceReader *);
v14::XmlDoc *THISCALL ReadFromFile14_hook(THISPTR(const v14::XmlReader *), const wchar_t *xmlFileNameForLogging, class v14::XmlPieceReader *xmlPieceReader)
{
  auto queue = get_xml_patches(xmlFileNameForLogging);
  if ( queue.empty() )
    return g_pfnReadFromFile14(thisptr, xmlFileNameForLogging, xmlPieceReader);

  pugi::xml_document doc;
  if ( !doc.load_file(xmlFileNameForLogging) )
    return nullptr;

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer14(thisptr, writer.result.data(), SafeInt(writer.result.size()), xmlFileNameForLogging, xmlPieceReader);
}

v15::XmlDoc *(__thiscall *g_pfnReadFromFile15)(const v15::XmlReader *, const wchar_t *, class v15::XmlPieceReader *);
v15::XmlDoc *THISCALL ReadFromFile15_hook(THISPTR(const v15::XmlReader *), const wchar_t *xmlFileNameForLogging, class v15::XmlPieceReader *xmlPieceReader)
{
  auto queue = get_xml_patches(xmlFileNameForLogging);
  if ( queue.empty() )
    return g_pfnReadFromFile15(thisptr, xmlFileNameForLogging, xmlPieceReader);

  pugi::xml_document doc;
  if ( !doc.load_file(xmlFileNameForLogging) )
    return nullptr;

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer15(thisptr, writer.result.data(), SafeInt(writer.result.size()), xmlFileNameForLogging, xmlPieceReader);
}

v13::XmlDoc *(__thiscall *g_pfnReadFromBuffer13)(const v13::XmlReader *, const unsigned char *, unsigned int, const wchar_t *);
v13::XmlDoc *THISCALL ReadFromBuffer13_hook(THISPTR(const v13::XmlReader *), const unsigned char *mem, unsigned int size, const wchar_t *xmlFileNameForLogging)
{
  if ( !mem || !size )
    return nullptr;

  auto queue = get_xml_patches(xmlFileNameForLogging);
  if ( queue.empty() )
    return g_pfnReadFromBuffer13(thisptr, mem, size, xmlFileNameForLogging);

  pugi::xml_document doc;
  if ( size >= sizeof(int64_t)
    && *reinterpret_cast<int64_t const *>(mem) == 0x424C534F42584D4Ci64 ) {
    auto xmlDoc = g_pfnReadFromBuffer13(thisptr, mem, size, xmlFileNameForLogging);
    if ( !xmldoc_to_pugixml(xmlDoc, doc) )
      return xmlDoc;
    thisptr->Close(xmlDoc);
  } else {
    if ( !doc.load_buffer(mem, size) )
      return nullptr;
  }

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer13(thisptr, writer.result.data(), SafeInt(writer.result.size()), xmlFileNameForLogging);
}

v14::XmlDoc *(__thiscall *g_pfnReadFromBuffer14)(const v14::XmlReader *, const unsigned char *, unsigned int, const wchar_t *, class v14::XmlPieceReader *);
v14::XmlDoc *THISCALL ReadFromBuffer14_hook(THISPTR(const v14::XmlReader *), const unsigned char *mem, unsigned int size, const wchar_t *xmlFileNameForLogging, class v14::XmlPieceReader *xmlPieceReader)
{
  if ( !mem || !size )
    return nullptr;

  auto queue = get_xml_patches(xmlFileNameForLogging);
  if ( queue.empty() )
    return g_pfnReadFromBuffer14(thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);

  pugi::xml_document doc;
  if ( size >= sizeof(int64_t)
    && *reinterpret_cast<int64_t const *>(mem) == 0x424C534F42584D4Ci64 ) {
    auto xmlDoc = g_pfnReadFromBuffer14(thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);
    if ( !xmldoc_to_pugixml(xmlDoc, doc) )
      return xmlDoc;
    thisptr->Close(xmlDoc);
  } else {
    if ( !doc.load_buffer(mem, size) )
      return nullptr;
  }

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer14(thisptr, writer.result.data(), SafeInt(writer.result.size()), xmlFileNameForLogging, xmlPieceReader);
}

v15::XmlDoc *(__thiscall *g_pfnReadFromBuffer15)(const v15::XmlReader *, const unsigned char *, unsigned int, const wchar_t *, class v15::XmlPieceReader *);
v15::XmlDoc *THISCALL ReadFromBuffer15_hook(THISPTR(const v15::XmlReader *), const unsigned char *mem, unsigned int size, const wchar_t *xmlFileNameForLogging, class v15::XmlPieceReader *xmlPieceReader)
{
  if ( !mem || !size )
    return nullptr;

  auto queue = get_xml_patches(xmlFileNameForLogging);
  if ( queue.empty() )
    return g_pfnReadFromBuffer15(thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);

  pugi::xml_document doc;
  if ( size >= sizeof(int64_t)
    && *reinterpret_cast<int64_t const *>(mem) == 0x424C534F42584D4Ci64 ) {
    auto xmlDoc = g_pfnReadFromBuffer15(thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);
    if ( !xmldoc_to_pugixml(xmlDoc, doc) )
      return xmlDoc;
    thisptr->Close(xmlDoc);
  } else {
    if ( !doc.load_buffer(mem, size) )
      return nullptr;
  }

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
#ifdef _DEBUG
  auto p = patches_file_path().parent_path().append(xorstr_("tmp")).append(xmlFileNameForLogging);
  std::filesystem::create_directories(p.parent_path());
  doc.save_file(p.c_str(), L"  ");
#endif
  return g_pfnReadFromBuffer15(thisptr, writer.result.data(), SafeInt(writer.result.size()), xmlFileNameForLogging, xmlPieceReader);
}
