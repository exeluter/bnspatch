#include "xmlpatch.h"
#include <ntdll.h>
#include <ShlObj.h>
#include <KnownFolders.h>

#include <algorithm>
#include <codecvt>
#include <filesystem>
namespace fs = std::filesystem;

#include <fmt/format.h>
#include <magic_enum.hpp>
#include <pugixml/pugixml.hpp>
#include <xorstr.hpp>

#include "fnv1a.h"
#include "ic_char_traits.h"
#include "xmlreader.h"

pugi::xml_document const g_patches = []() {
  PWSTR pszPath;
  pugi::xml_document doc;

  if ( SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &pszPath) == S_OK ) {
    fs::path path(pszPath);
    CoTaskMemFree(pszPath);
#ifdef BNSR
    path /= xorstr_(L"BNSR\\patches.xml");
#else
    path /= xorstr_(L"BnS\\patches.xml");
#endif

load_patches:
    auto const result = doc.load_file(path.c_str());
    if ( !result ) {
      auto const text = fmt::format(xorstr_("{} at offset {}.\nDo you want to try again?"), result.description(), result.offset);
      switch ( MessageBoxA(nullptr, text.c_str(), xorstr_("patches.xml"), MB_CANCELTRYCONTINUE | MB_ICONERROR) ) {
        case IDTRYAGAIN: goto load_patches;
        case IDCONTINUE: break;
        default: exit(1);
      }
    }
  }
  return doc;
}();

void process_patch(
  pugi::xpath_node const &ctx,
  pugi::xml_object_range<pugi::xml_node_iterator> const &children)
{
  using pugi::xml_document;
  using pugi::xml_node;
  using pugi::xml_node_iterator;
  using pugi::xml_node_type;
  using pugi::xml_object_range;
  const auto &facet = std::use_facet<std::ctype<pugi::char_t>>(std::locale());

  for ( auto const &current : children ) {
    if ( ctx.attribute() ) {
      switch ( fnv1a::hash_lower<pugi::char_t>(current.name()) ) {
        case L"parent"_fnv1a: {
          process_patch(ctx.parent(), current.children());
          break;
        } case L"set-name"_fnv1a: {
          ctx.attribute().set_name(current.attribute(xorstr_(L"name")).value());
          break;
        } case L"set-value"_fnv1a: {
          ctx.attribute().set_value(current.attribute(xorstr_(L"value")).value());
          break;
        } case L"previous-attribute"_fnv1a: {
          process_patch({ ctx.attribute().previous_attribute(), ctx.parent() }, current.children());
          break;
        } case L"next-attribute"_fnv1a: {
          process_patch({ ctx.attribute().next_attribute(), ctx.parent() }, current.children());
          break;
        } case L"remove"_fnv1a: {
          ctx.parent().remove_attribute(ctx.attribute());
          break;
        }
      }
    } else {
      switch ( fnv1a::hash_lower<pugi::char_t>(current.name()) ) {
        case L"parent"_fnv1a: {
          process_patch(ctx.parent(), current.children());
          break;
        } case L"select-node"_fnv1a: {
          process_patch(ctx.node().select_node(current.attribute(xorstr_(L"query")).value()), current.children());
          break;
        } case L"select-nodes"_fnv1a: {
          for ( auto &xn : ctx.node().select_nodes(current.attribute(xorstr_(L"query")).value()) ) {
            process_patch(xn, current.children());
          }
          break;
        } case L"prepend-attribute"_fnv1a: {
          auto const name = current.attribute(xorstr_(L"name"));
          process_patch({ ctx.node().prepend_attribute(name.value()), ctx.node() }, current.children());
          break;
        } case L"append-attribute"_fnv1a: {
          auto const name = current.attribute(xorstr_(L"name"));
          process_patch({ ctx.node().append_attribute(name.value()), ctx.node() }, current.children());
          break;
        } case L"prepend-child"_fnv1a: {
          if ( auto const name = current.attribute(xorstr_(L"name")) ) {
            process_patch(ctx.node().prepend_child(name.value()), current.children());
          } else {
            std::optional<xml_node_type> t;
            if ( auto const type = current.attribute(xorstr_(L"type")) ) {
              std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
              auto s = converter.to_bytes(type.value());
              t = magic_enum::enum_cast<xml_node_type>(s);
            }
            process_patch(ctx.node().append_child(t.value_or(xml_node_type::node_element)), current.children());
          }
          break;
        } case L"append-buffer"_fnv1a: {
          auto const s = std::wstring_view(current.text().get());
          ctx.node().append_buffer(s.data(), s.size(), pugi::parse_default | pugi::parse_fragment);
          break;
        } case L"append-child"_fnv1a: {
          if ( auto const name = current.attribute(xorstr_(L"name")) ) {
            process_patch(ctx.node().append_child(name.value()), current.children());
          } else {
            std::optional<xml_node_type> t;
            if ( auto const type = current.attribute(xorstr_(L"type")) ) {
              std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
              auto s = converter.to_bytes(type.value());
              t = magic_enum::enum_cast<xml_node_type>(s);
            }
            process_patch(ctx.node().append_child(t.value_or(xml_node_type::node_element)), current.children());
          }
          break;
        } case L"prepend-copy"_fnv1a: {
          auto xn = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value());
          if ( auto attr = xn.attribute() ) {
            process_patch({ ctx.node().prepend_copy(attr), xn.parent() }, current.children());
          } else if ( auto node = xn.node() ) {
            process_patch(ctx.node().prepend_copy(node), current.children());
          }
          break;
        } case L"append-copy"_fnv1a: {
          auto xn = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value());
          if ( auto attr = xn.attribute() ) {
            process_patch({ ctx.node().append_copy(attr), xn.parent() }, current.children());
          } else if ( auto node = xn.node() ) {
            process_patch(ctx.node().append_copy(node), current.children());
          }
          break;
        } case L"prepend-move"_fnv1a: {
          process_patch(
            ctx.node().prepend_move(ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node()),
            current.children());
          break;
        } case L"append-move"_fnv1a: {
          process_patch(ctx.node().append_move(ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node()), current.children());
          break;
        } case L"attribute"_fnv1a: {
          process_patch({ ctx.node().attribute(current.attribute(xorstr_(L"name")).value()), ctx.node() }, current.children());
          break;
        } case L"attributes"_fnv1a: {
          for ( auto &attr : ctx.node().attributes() ) {
            process_patch({ attr, ctx.node() }, current.children());
          }
          break;
        } case L"child"_fnv1a: {
          process_patch(ctx.node().child(current.attribute(xorstr_(L"name")).value()), current.children());
          break;
        } case L"children"_fnv1a: {
          for ( auto &child : ctx.node().children() ) {
            process_patch(child, current.children());
          }
          break;
        } case L"find-child-by-attribute"_fnv1a: {
          process_patch(
            ctx.node().find_child_by_attribute(
              current.attribute(xorstr_(L"name")).value(),
              current.attribute(xorstr_(L"attr-name")).value(),
              current.attribute(xorstr_(L"attr-value")).value()),
            current.children());
          break;
        } case L"first-attribute"_fnv1a: {
          process_patch({ ctx.node().first_attribute(), ctx.node() }, current.children());
          break;
        } case L"last-attribute"_fnv1a: {
          process_patch({ ctx.node().last_attribute(), ctx.node() }, current.children());
          break;
        } case L"first-child"_fnv1a: {
          process_patch(ctx.node().first_child(), current.children());
          break;
        } case L"last-child"_fnv1a: {
          process_patch(ctx.node().last_child(), current.children());
          break;
        } case L"first-element-by-path"_fnv1a: {
          process_patch(ctx.node().first_element_by_path(current.attribute(xorstr_(L"path")).value()), current.children());
          break;
        } case L"insert-attribute-before"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          auto attr_query = current.attribute(xorstr_(L"attr-query")).value();
          auto xn = ctx.node().select_node(attr_query);
          process_patch({ ctx.node().insert_attribute_before(name, xn.attribute()), xn.parent() }, current.children());
          break;
        } case L"insert-attribute-after"_fnv1a: {
          // not implemented
          break;
        } case L"insert-child-after"_fnv1a: {
          process_patch(
            ctx.node().insert_child_after(
              current.attribute(xorstr_(L"name")).value(),
              ctx.node().select_node(current.attribute(xorstr_(L"query")).value()).node()),
            current.children());
          break;
        } case L"insert-child-before"_fnv1a: {
          process_patch(
            ctx.node().insert_child_before(
              current.attribute(xorstr_(L"name")).value(),
              ctx.node().select_node(current.attribute(xorstr_(L"query")).value()).node()),
            current.children());
          break;
        } case L"insert-copy-after"_fnv1a: {
          // not implemented
          break;
        } case L"insert-copy-before"_fnv1a: {
          // not implemented
          break;
        } case L"insert-move-after"_fnv1a: {
          // not implemented
          break;
        } case L"insert-move-before"_fnv1a: {
          // not implemented
          break;
        } case L"next-sibling"_fnv1a: {
          process_patch(ctx.node().next_sibling(), current.children());
          break;
        } case L"remove-attribute"_fnv1a: {
          ctx.node().remove_attribute(current.attribute(xorstr_(L"name")).value());
          break;
        } case L"remove-attributes"_fnv1a: {
          ctx.node().remove_attributes();
          break;
        } case L"remove-child"_fnv1a: {
          ctx.node().remove_child(current.attribute(xorstr_(L"name")).value());
          break;
        } case L"remove-children"_fnv1a: {
          ctx.node().remove_children();
          break;
        } case L"root"_fnv1a: {
          process_patch(ctx.node().root(), current.children());
          break;
        } case L"set-name"_fnv1a: {
          ctx.node().set_name(current.attribute(xorstr_(L"value")).value());
          break;
        } case L"set-value"_fnv1a: {
          ctx.node().set_value(current.attribute(xorstr_(L"value")).value());
          break;
        } case L"remove"_fnv1a: {
          ctx.node().parent().remove_child(ctx.node());
          break;
        }
      }
    }
  }
}

void process_xmldoc(XmlElement const *src, pugi::xml_node &dst)
{
  auto node = dst.append_child(src->Name());
  for ( int i = 0; i < src->AttributeCount(); ++i )
    node.append_attribute(src->AttributeName(i)) = src->Attribute(i);
  for ( auto child = src->FirstChildElement(); child; child = child->NextElement() )
    process_xmldoc(child, node);
}
