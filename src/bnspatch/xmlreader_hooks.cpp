#include "xmlreader_hooks.h"
#include <phnt_windows.h>
#include <phnt.h>
#include <Shlwapi.h>

#include <array>
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <optional>
#include <queue>
#include <string>

#include <fmt/format.h>
#include <pugixml.hpp>
#include <safeint.hpp>
#include <xorstr.hpp>

#include "fastwildcompare.h"
#include "xmlpatch.h"
#include "xmlreader.h"

struct xml_memory_buffer_writer : pugi::xml_writer
{
  fmt::basic_memory_buffer<unsigned char> result;

  virtual void write(const void *data, size_t size)
  {
    result.append(
      reinterpret_cast<const unsigned char *>(data),
      reinterpret_cast<const unsigned char *>(data) + size);
  }
};

v13::XmlDoc *(__thiscall *g_pfnReadFromFile13)(const v13::XmlReader *a, const wchar_t *);
v13::XmlDoc *thiscall_(ReadFromFile13_hook, const v13::XmlReader *thisptr, const wchar_t *xml)
{
  if ( !xml )
    return nullptr;

  auto queue = get_xml_patches(xml);
  if ( !queue.empty() ) {
    pugi::xml_document document;
    if ( const auto res = document.load_file(xml, pugi::parse_full) ) {
      patch_xml(document, queue);
      xml_memory_buffer_writer writer;
      document.save(writer, nullptr, pugi::format_raw | pugi::format_no_declaration, res.encoding);
      return g_pfnReadFromBuffer13(thisptr, writer.result.data(), SafeInt(writer.result.size()), xml);
    }
  }
  return g_pfnReadFromFile13(thisptr, xml);
}

v14::XmlDoc *(__thiscall *g_pfnReadFromFile14)(const v14::XmlReader *, const wchar_t *, class v14::XmlPieceReader *);
v14::XmlDoc *thiscall_(ReadFromFile14_hook, const v14::XmlReader *thisptr, const wchar_t *xml, class v14::XmlPieceReader *xmlPieceReader)
{
  if ( !xml )
    return nullptr;

  auto queue = get_xml_patches(xml);
  if ( !queue.empty() ) {
    pugi::xml_document document;
    if ( const auto res = document.load_file(xml, pugi::parse_full) ) {
      patch_xml(document, queue);
      xml_memory_buffer_writer writer;
      document.save(writer, nullptr, pugi::format_raw | pugi::format_no_declaration, res.encoding);
      return g_pfnReadFromBuffer14(thisptr, writer.result.data(), SafeInt(writer.result.size()), xml, xmlPieceReader);
    }
  }
  return g_pfnReadFromFile14(thisptr, xml, xmlPieceReader);
}

v13::XmlDoc *(__thiscall *g_pfnReadFromBuffer13)(const v13::XmlReader *, const unsigned char *, unsigned int, const wchar_t *);
v13::XmlDoc *thiscall_(ReadFromBuffer13_hook, const v13::XmlReader *thisptr, const unsigned char *mem, unsigned int size, const wchar_t *xmlFileNameForLogging)
{
  if ( !mem || !size )
    return nullptr;

  auto queue = get_xml_patches(xmlFileNameForLogging);
  if ( !queue.empty() ) {
    pugi::xml_document document;
    if ( const auto res = deserialize_document(mem, size, document) ) {
      patch_xml(document, queue);
      xml_memory_buffer_writer writer;
      document.save(writer, nullptr, pugi::format_raw | pugi::format_no_declaration, res.encoding);
      return g_pfnReadFromBuffer13(thisptr, writer.result.data(), SafeInt(writer.result.size()), xmlFileNameForLogging);
    }
  }
  return g_pfnReadFromBuffer13(thisptr, mem, size, xmlFileNameForLogging);
}

v14::XmlDoc *(__thiscall *g_pfnReadFromBuffer14)(const v14::XmlReader *, const unsigned char *, unsigned int, const wchar_t *, class v14::XmlPieceReader *);
v14::XmlDoc *thiscall_(ReadFromBuffer14_hook, const v14::XmlReader *thisptr, const unsigned char *mem, unsigned int size, const wchar_t *xmlFileNameForLogging, class v14::XmlPieceReader *xmlPieceReader)
{
  if ( !mem || !size )
    return nullptr;

  auto queue = get_xml_patches(xmlFileNameForLogging);
  if ( !queue.empty() ) {
    pugi::xml_document document;
    if ( const auto res = deserialize_document(mem, size, document) ) {

#ifdef _DEBUG
      if ( xmlFileNameForLogging && *xmlFileNameForLogging ) {
        const auto temp = patches_file_path().parent_path().append(xorstr_(L"temp")).append(xmlFileNameForLogging);
        document.save_file(temp.c_str(), L"  ", pugi::format_default | pugi::format_no_declaration, res.encoding);
      }
#endif

      patch_xml(document, queue);

#ifdef _DEBUG
      if ( xmlFileNameForLogging && *xmlFileNameForLogging ) {
        const auto temp_patched = patches_file_path().parent_path().append(xorstr_(L"temp_patched")).append(xmlFileNameForLogging);
        document.save_file(temp_patched.c_str(), L"  ", pugi::format_default | pugi::format_no_declaration, res.encoding);
      }
#endif

      xml_memory_buffer_writer writer;
      document.save(writer, nullptr, pugi::format_raw | pugi::format_no_declaration, res.encoding);
      return g_pfnReadFromBuffer14(thisptr, writer.result.data(), SafeInt(writer.result.size()), xmlFileNameForLogging, xmlPieceReader);
    }
  }
  return g_pfnReadFromBuffer14(thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);
}
