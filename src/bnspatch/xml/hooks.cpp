#include <phnt_windows.h>
#include <phnt.h>

#include <numeric>
#include <mutex>
#include <fstream>

#include <detours.h>
#include <fmt/format.h>
#include <SafeInt.hpp>
#include <xorstr.hpp>

#include "hooks.h"
#include "patch.h"
#include "xmlreader.h"
#include "common.h"
#include <util.hpp>

XmlDoc* __fastcall ReadMem(
  PFN_XMLREADER_READMEM pfnReadMem,
  PFN_XMLREADER_READFILE pfnReadFile,
  const XmlReader* thisptr,
  const unsigned char* mem,
  unsigned int size,
  const wchar_t* xmlFileNameForLogging,
  XmlPieceReader* xmlPieceReader) {

  if (!mem || !size)
    return nullptr;

  if (xmlFileNameForLogging && *xmlFileNameForLogging) {
#ifdef _DEBUG
    OutputDebugStringW(xmlFileNameForLogging);
#endif

    const auto patches = get_relevant_patches(xmlFileNameForLogging);
    const auto addons = get_relevant_addons(xmlFileNameForLogging);
    if (!patches.empty() || !addons.empty()) {
      pugi::xml_document doc;
      pugi::xml_parse_result res;

      if (size >= sizeof(std::int64_t) && *reinterpret_cast<const std::int64_t*>(mem) == 0x424C534F42584D4C) {
        const auto xmlDoc = pfnReadMem(thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);
        if (!xmlDoc) return nullptr;
        res = convert_document(doc, xmlDoc);
      }
      else {
        res = doc.load_buffer(mem, size);
      }

      if (res) {
        apply_patches(doc, res.encoding, patches);

        if (!addons.empty() && res.encoding == pugi::encoding_utf16_le) {
          xml_wstring_writer writer;
          doc.save(writer, xorstr_(L""), pugi::format_default | pugi::format_no_declaration, res.encoding);

          for (const auto& addon : addons)
            ReplaceStringInPlace(writer.result, addon.first, addon.second);

          return pfnReadMem(
            thisptr,
            reinterpret_cast<unsigned char*>(writer.result.data()),
            SafeInt(writer.result.size() * sizeof(wchar_t)),
            xmlFileNameForLogging,
            xmlPieceReader);
        }
        else {
          // don't apply addons
          xml_buffer_writer writer;
          doc.save(writer, nullptr, pugi::format_raw | pugi::format_no_declaration, res.encoding);
          return pfnReadMem(
            thisptr,
            writer.result.data(),
            SafeInt(writer.result.size()),
            xmlFileNameForLogging,
            xmlPieceReader);
        }
      }
    }
  }
  return pfnReadMem(thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);
}

XmlDoc* __fastcall ReadFile(
  PFN_XMLREADER_READMEM pfnReadMem,
  PFN_XMLREADER_READFILE pfnReadFile,
  const XmlReader* thisptr,
  const wchar_t* xml,
  XmlPieceReader* xmlPieceReader) {

  auto xmlDoc = pfnReadFile(thisptr, xml, xmlPieceReader);
  if (!xmlDoc) return nullptr;

  auto patches = get_relevant_patches(xml);
  if (!patches.empty()) {
    pugi::xml_document doc;
    if (const auto res = convert_document(doc, xmlDoc)) {
      apply_patches(doc, res.encoding, patches);
      xml_buffer_writer writer;
      doc.save(writer, nullptr, pugi::format_raw | pugi::format_no_declaration, res.encoding);
      return pfnReadMem(thisptr, writer.result.data(), SafeInt(writer.result.size()), xml, xmlPieceReader);
    }
  }
  return pfnReadFile(thisptr, xml, xmlPieceReader);
}

PFN_XMLREADER_READMEM g_pfnClientReadMem;
XmlDoc* thiscall_(ClientReadMem_hook, const XmlReader* thisptr, const unsigned char* mem, unsigned int size, const wchar_t* xmlFileNameForLogging, class XmlPieceReader* xmlPieceReader)
{
  return ReadMem(g_pfnClientReadMem, g_pfnClientReadFile, thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);
}

PFN_XMLREADER_READFILE g_pfnClientReadFile;
XmlDoc* thiscall_(ClientReadFile_hook, const XmlReader* thisptr, const wchar_t* xml, class XmlPieceReader* xmlPieceReader)
{
  return ReadFile(g_pfnClientReadMem, g_pfnClientReadFile, thisptr, xml, xmlPieceReader);
}

PFN_XMLREADER_READFILE g_pfnEngineReadFile;
XmlDoc* thiscall_(EngineReadFile_hook, const XmlReader* thisptr, const wchar_t* xml, class XmlPieceReader* xmlPieceReader)
{
  return ReadFile(g_pfnEngineReadMem, g_pfnEngineReadFile, thisptr, xml, xmlPieceReader);
}

PFN_XMLREADER_READMEM g_pfnEngineReadMem;
XmlDoc* thiscall_(EngineReadMem_hook, const XmlReader* thisptr, const unsigned char* mem, unsigned int size, const wchar_t* xmlFileNameForLogging, class XmlPieceReader* xmlPieceReader)
{
  return ReadMem(g_pfnEngineReadMem, g_pfnEngineReadFile, thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);
}
