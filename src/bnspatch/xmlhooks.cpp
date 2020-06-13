#include "xmlhooks.h"
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

#include <fastwildcompare.hpp>
#include <fmt/format.h>
#include <pugixml.hpp>
#include <safeint.hpp>
#include <xorstr.hpp>

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

template <class T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline T read_integer_aligned_unchecked(const void *&mem, size_t &size)
{
  const T result = *reinterpret_cast<const T *>(mem);
  mem = reinterpret_cast<const uint8_t *>(mem) + ((sizeof(T) + 3) & ~3);
  size -= ((sizeof(T) + 3) & ~3);
  return result;
}

template <class T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline std::optional<T> read_integer_aligned(const void *&mem, size_t &size)
{
  if ( size < ((sizeof(T) + 3) & ~3) )
    return std::nullopt;

  return read_integer_aligned_unchecked<T>(mem, size);
}

template <class T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline T read_integer_unaligned_unchecked(const void *&mem, size_t &size)
{
  const T result = *reinterpret_cast<const T *>(mem);
  mem = reinterpret_cast<const uint8_t *>(mem) + sizeof(T);
  size -= sizeof(T);
  return result;
}

template <class T, typename = std::enable_if_t<std::is_integral_v<T>>>
inline std::optional<T> read_integer_unaligned(const void *&mem, size_t &size)
{
  if ( size < sizeof(T) )
    return std::nullopt;

  return read_integer_unaligned_unchecked<T>(mem, size);
}

inline void discard_bytes_unchecked(size_t discarded_bytes_count, const void *&mem, size_t &size)
{
  mem = reinterpret_cast<const uint8_t *>(mem) + discarded_bytes_count;
  size -= discarded_bytes_count;
}

inline bool discard_bytes(size_t discarded_bytes_count, const void *&mem, size_t &size)
{
  if ( size < discarded_bytes_count )
    return false;

  discard_bytes_unchecked(discarded_bytes_count, mem, size);
  return true;
}

inline bool discard_string(const void *&buf, size_t &size)
{
  auto length = read_integer_aligned<int>(buf, size);
  if ( length.value_or(-1) < 0 )
    return false;

  return discard_bytes(*length * sizeof(wchar_t), buf, size);
}

std::optional<std::wstring> read_string(const void *&buf, size_t &size)
{
  static const auto cipherKeys = std::array {
    0x9fa4ui16,
    0xb3d8ui16,
    0x8ef6ui16,
    0xc239ui16,
    0xe02dui16,
    0x7561ui16,
    0x4b5cui16,
    0x071aui16
  };

  auto length = read_integer_aligned<int>(buf, size).value_or(-1);
  if ( length < 0 || size < length * sizeof(wchar_t) )
    return std::nullopt;

  // don't need to size-check after this

  std::wstring result;
  result.reserve(length);

  auto data = reinterpret_cast<const uint16_t *>(buf);
  for ( size_t i = 0; i < length; ++i )
    result.push_back(data[i] ^ cipherKeys[i & (cipherKeys.size() - 1)]);
  buf = data + length;
  return result;
}

pugi::xml_node deserialize_element(pugi::xml_node &parent, const void *&mem, size_t &size);
bool deserialize_text(pugi::xml_node &parent, const void *&mem, size_t &size);
bool deserialize_node(pugi::xml_node &parent, const void *&mem, size_t &size);

pugi::xml_node deserialize_element(pugi::xml_node &parent, const void *&mem, size_t &size)
{
  auto node = parent.append_child();

  const auto attrcount = read_integer_aligned<int>(mem, size);
  if ( !attrcount ) {
remove_node:
    parent.remove_child(node);
    return pugi::xml_node();
  }
  for ( int i = 0; i < *attrcount; ++i ) {
    const auto attrname = read_string(mem, size);
    if ( !attrname ) goto remove_node;

    const auto attrvalue = read_string(mem, size);
    if ( !attrvalue ) goto remove_node;
    node.append_attribute(attrname->c_str()).set_value(attrvalue->c_str());
  }

  const auto valid = read_integer_unaligned<bool>(mem, size);
  if ( !valid ) goto remove_node;

  const auto name = read_string(mem, size);
  if ( !name ) goto remove_node;

  node.set_name(name->c_str());

  const auto children_count = read_integer_unaligned<int>(mem, size);
  if ( !children_count ) goto remove_node;

  const auto id = read_integer_unaligned<int>(mem, size);
  if ( !id ) goto remove_node;

  for ( int i = 0; i < *children_count; ++i ) {
    if ( !deserialize_node(node, mem, size) )
      goto remove_node;
  }
  return node;
}

bool deserialize_text(pugi::xml_node &parent, const void *&mem, size_t &size)
{
  auto pcdata = read_string(mem, size);
  if ( !pcdata ) return false;

  if ( !std::all_of(pcdata->begin(), pcdata->end(), ::iswspace) ) {
    auto node = parent.append_child(pugi::node_pcdata);
    node.set_value(pcdata->data());
  }

  return read_integer_unaligned<bool>(mem, size) // valid
    && discard_string(mem, size)                 // name
    && read_integer_unaligned<int>(mem, size)    // children count
    && read_integer_unaligned<int>(mem, size);   // id
}

inline bool deserialize_node(pugi::xml_node &parent, const void *&mem, size_t &size)
{
  switch ( read_integer_unaligned<int>(mem, size).value_or(0) ) {
    case 1: return deserialize_element(parent, mem, size);
    case 2: return deserialize_text(parent, mem, size);
  }
  return false;
}

pugi::xml_document deserialize_document(const void *mem, const size_t size)
{
  auto document = pugi::xml_document();
  auto size_remaining = size;

  if ( mem
    && size > 0x50 // rest of the header doesn't need to be size-checked
    && read_integer_aligned_unchecked<int64_t>(mem, size_remaining) == 0x424C534F42584D4Ci64
    && read_integer_aligned_unchecked<char>(mem, size_remaining) == 3 ) {

    const auto document_size = read_integer_aligned_unchecked<int>(mem, size_remaining);
    if ( document_size > size )
      return pugi::xml_document();

    discard_bytes_unchecked(0x40, mem, size_remaining);

    // need to size-check after this
    const auto valid = read_integer_unaligned<bool>(mem, size_remaining);
    if ( !valid ) return pugi::xml_document();

    const auto document_name = read_string(mem, size_remaining);
    if ( !document_name ) return pugi::xml_document();

    auto decl = document.prepend_child(pugi::node_declaration);
    decl.append_attribute(xorstr_(L"version")) = xorstr_(L"1.0");
    decl.append_attribute(xorstr_(L"encoding")) = xorstr_(L"utf-16");
    document.append_child(pugi::node_comment).set_value(document_name->c_str());
    if ( deserialize_element(document, mem, size_remaining) )
      return document;
  } else {
    if ( document.load_buffer(mem, size, pugi::parse_default, pugi::encoding_utf16) )
      return document;
  }
  return pugi::xml_document();
}

std::queue<pugi::xml_node> get_xml_patches(const wchar_t *xml)
{
  auto queue = std::queue<pugi::xml_node>();

  auto xml_path = std::filesystem::path(xml ? xml : L"");
  auto name = xorstr(L"patch");
  name.crypt();
  for ( const auto &patch : patches_document().document_element().children(name.get()) ) {
    auto attribute = patch.attribute(xorstr_(L"file"));
    if ( !attribute )
      attribute = patch.attribute(xorstr_(L"filename"));

    auto str = std::unique_ptr<wchar_t, decltype(&::free)>(::_wcsdup(attribute.value()), ::free);
    wchar_t *next_token;
    wchar_t *token = wcstok_s(str.get(), xorstr_(L";"), &next_token);
    while ( token ) {
      auto file = std::filesystem::path(token);
      if ( FastWildCompare(file.c_str(), xml_path.c_str())
        || FastWildCompare(file.c_str(), xml_path.filename().c_str()) )
        queue.push(patch);
      token = wcstok_s(nullptr, xorstr_(L";"), &next_token);
    }
  }
  return queue;
}

void patch_xml(pugi::xml_document &src, std::queue<pugi::xml_node> queue, pugi::xml_writer &writer)
{
  while ( !queue.empty() ) {
    auto const &patch = queue.front();
    std::unordered_map<fnv1a::type, pugi::xml_node> saved_nodes;
    process_patch(src, patch.children(), saved_nodes);
    queue.pop();
  }
  src.save(writer, nullptr, pugi::format_raw, pugi::xml_encoding::encoding_utf16);
}

v13::XmlDoc *(__thiscall *g_pfnReadFromFile13)(const v13::XmlReader *a, const wchar_t *);
v13::XmlDoc *THISCALL ReadFromFile13_hook(THISPTR(const v13::XmlReader *), const wchar_t *xml)
{
  auto queue = get_xml_patches(xml);
  if ( queue.empty() )
    return g_pfnReadFromFile13(thisptr, xml);

  pugi::xml_document doc;
  if ( !doc.load_file(xml) )
    return nullptr;

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer13(thisptr, writer.result.data(), SafeInt(writer.result.size()), xml);
}

v14::XmlDoc *(__thiscall *g_pfnReadFromFile14)(const v14::XmlReader *, const wchar_t *, class v14::XmlPieceReader *);
v14::XmlDoc *THISCALL ReadFromFile14_hook(THISPTR(const v14::XmlReader *), const wchar_t *xml, class v14::XmlPieceReader *xmlPieceReader)
{
  auto queue = get_xml_patches(xml);
  if ( queue.empty() )
    return g_pfnReadFromFile14(thisptr, xml, xmlPieceReader);

  pugi::xml_document doc;
  if ( !doc.load_file(xml) )
    return nullptr;

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer14(thisptr, writer.result.data(), SafeInt(writer.result.size()), xml, xmlPieceReader);
}

v15::XmlDoc *(__thiscall *g_pfnReadFromFile15)(const v15::XmlReader *, const wchar_t *, class v15::XmlPieceReader *);
v15::XmlDoc *THISCALL ReadFromFile15_hook(THISPTR(const v15::XmlReader *), const wchar_t *xml, class v15::XmlPieceReader *xmlPieceReader)
{
  auto queue = get_xml_patches(xml);
  if ( queue.empty() )
    return g_pfnReadFromFile15(thisptr, xml, xmlPieceReader);

  pugi::xml_document doc;
  if ( !doc.load_file(xml) )
    return nullptr;

  memory_buffer_writer<unsigned char> writer;
  patch_xml(doc, queue, writer);
  return g_pfnReadFromBuffer15(thisptr, writer.result.data(), SafeInt(writer.result.size()), xml, xmlPieceReader);
}

v13::XmlDoc *(__thiscall *g_pfnReadFromBuffer13)(const v13::XmlReader *, const unsigned char *, unsigned int, const wchar_t *);
v13::XmlDoc *THISCALL ReadFromBuffer13_hook(THISPTR(const v13::XmlReader *), const unsigned char *mem, unsigned int size, const wchar_t *xmlFileNameForLogging)
{
  if ( !mem || !size )
    return nullptr;

  auto queue = get_xml_patches(xmlFileNameForLogging);
  if ( queue.empty() )
    return g_pfnReadFromBuffer13(thisptr, mem, size, xmlFileNameForLogging);

  auto doc = deserialize_document(mem, size);

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

  auto doc = deserialize_document(mem, size);

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
  if ( !queue.empty() ) {
    if ( auto doc = deserialize_document(mem, size) ) {
#ifdef _DEBUG
      auto tmp = patches_file_path().parent_path().append(xorstr_(L"tmp")).append(xmlFileNameForLogging);
      std::filesystem::create_directories(tmp.parent_path());
      doc.save_file(tmp.c_str(), L"  ", pugi::format_default, pugi::encoding_utf16);
#endif
      memory_buffer_writer<unsigned char> writer;
      patch_xml(doc, queue, writer);
#ifdef _DEBUG
      auto tmp_patched = patches_file_path().parent_path().append(xorstr_(L"tmp_patched")).append(xmlFileNameForLogging);
      std::filesystem::create_directories(tmp_patched.parent_path());
      doc.save_file(tmp_patched.c_str(), L"  ", pugi::format_default, pugi::encoding_utf16);
#endif
      return g_pfnReadFromBuffer15(thisptr, writer.result.data(), SafeInt(writer.result.size()), xmlFileNameForLogging, xmlPieceReader);
    }
  }
  return g_pfnReadFromBuffer15(thisptr, mem, size, xmlFileNameForLogging, xmlPieceReader);
}
