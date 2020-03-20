#include "xmlpatch.h"
#include <ntdll.h>
#include <ShlObj.h>
#include <KnownFolders.h>

#include <algorithm>
#include <codecvt>
#include <filesystem>
namespace fs = std::filesystem;
#include <optional>

#include <fmt/format.h>
#include <fnv1a.h>
#include <magic_enum.hpp>
#include <pugixml/pugixml.hpp>
#ifdef NDEBUG
#include <xorstr.hpp>
#else
#define xorstr_(str) (str)
#endif

#include "pe/module.h"

pugi::xml_document const g_PatchesDocument = []() {
  PWSTR pszPath;
  pugi::xml_document doc;

  if ( SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &pszPath) == S_OK ) {
    fs::path path(pszPath);
    CoTaskMemFree(pszPath);

    const auto name = pe::get_module()->base_name();
    switch ( fnv1a::make_hash_lower(name.c_str()) ) {
      case L"Client.exe"_fnv1al: {
        path /= xorstr_(L"BnS\\patches.xml");

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
        break;
      } case L"BNSR.exe"_fnv1al: {
        path /= xorstr_(L"BNSR\\patches.xml");
        goto load_patches;
      }
    }
  }
  return doc;
}();

void ProcessPatch(
  pugi::xpath_node const &ctx,
  pugi::xml_object_range<pugi::xml_node_iterator> const &children)
{
  using pugi::xml_document;
  using pugi::xml_node;
  using pugi::xml_node_iterator;
  using pugi::xml_node_type;
  using pugi::xml_object_range;

  for ( auto const &current : children ) {
    if ( ctx.attribute() ) {
      switch ( fnv1a::make_hash_lower(current.name()) ) {
        case L"parent"_fnv1a: {
          ProcessPatch(ctx.parent(), current.children());
          break;
        } case L"set-name"_fnv1a: {
          ctx.attribute().set_name(current.attribute(xorstr_(L"name")).value());
          break;
        } case L"set-value"_fnv1a: {
          ctx.attribute().set_value(current.attribute(xorstr_(L"value")).value());
          break;
        } case L"previous-attribute"_fnv1a: {
          ProcessPatch({ ctx.attribute().previous_attribute(), ctx.parent() }, current.children());
          break;
        } case L"next-attribute"_fnv1a: {
          ProcessPatch({ ctx.attribute().next_attribute(), ctx.parent() }, current.children());
          break;
        } case L"insert-attribute-before"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ProcessPatch({ ctx.node().insert_attribute_before(name, ctx.attribute()), ctx.node() }, current.children());
          break;
        } case L"insert-attribute-after"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ProcessPatch({ ctx.node().insert_attribute_after(name, ctx.attribute()), ctx.node() }, current.children());
          break;
        } case L"remove"_fnv1a: {
          ctx.parent().remove_attribute(ctx.attribute());
          return; // context attribute is removed, nothing more to do.
        }
      }
    } else {
      switch ( fnv1a::make_hash_lower(current.name()) ) {
        case L"parent"_fnv1a: {
          ProcessPatch(ctx.parent(), current.children());
          break;
        } case L"select-node"_fnv1a: {
          auto query = current.attribute(xorstr_(L"query")).value();
          ProcessPatch(ctx.node().select_node(query), current.children());
          break;
        } case L"select-nodes"_fnv1a: {
          auto query = current.attribute(xorstr_(L"query")).value();
          for ( auto &node : ctx.node().select_nodes(query) )
            ProcessPatch(node, current.children());
          break;
        } case L"prepend-attribute"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name"));
          ProcessPatch({ ctx.node().prepend_attribute(name.value()), ctx.node() }, current.children());
          break;
        } case L"append-attribute"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name"));
          ProcessPatch({ ctx.node().append_attribute(name.value()), ctx.node() }, current.children());
          break;
        } case L"prepend-child"_fnv1a: {
          if ( auto name = current.attribute(xorstr_(L"name")) ) {
            ProcessPatch(ctx.node().prepend_child(name.value()), current.children());
          } else {
            std::optional<xml_node_type> t;
            if ( auto const type = current.attribute(xorstr_(L"type")) ) {
              std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
              auto s = converter.to_bytes(type.value());
              t = magic_enum::enum_cast<xml_node_type>(s);
            }
            ProcessPatch(ctx.node().append_child(t.value_or(xml_node_type::node_element)), current.children());
          }
          break;
        } case L"append-buffer"_fnv1a: {
          auto const s = std::wstring_view(current.text().get());
          ctx.node().append_buffer(s.data(), s.size(), pugi::parse_default | pugi::parse_fragment);
          break;
        } case L"append-child"_fnv1a: {
          if ( auto const name = current.attribute(xorstr_(L"name")) ) {
            ProcessPatch(ctx.node().append_child(name.value()), current.children());
          } else {
            std::optional<xml_node_type> t;
            if ( auto const type = current.attribute(xorstr_(L"type")) ) {
              std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
              auto s = converter.to_bytes(type.value());
              t = magic_enum::enum_cast<xml_node_type>(s);
            }
            ProcessPatch(ctx.node().append_child(t.value_or(xml_node_type::node_element)), current.children());
          }
          break;
        } case L"prepend-copy"_fnv1a: {
          auto proto = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value());
          if ( proto.attribute() ) {
            ProcessPatch({ ctx.node().prepend_copy(proto.attribute()), ctx.node() }, current.children());
          } else {
            ProcessPatch(ctx.node().prepend_copy(proto.node()), current.children());
          }
          break;
        } case L"append-copy"_fnv1a: {
          auto proto = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value());
          if ( proto.attribute() ) {
            ProcessPatch({ ctx.node().append_copy(proto.attribute()), ctx.node() }, current.children());
          } else {
            ProcessPatch(ctx.node().append_copy(proto.node()), current.children());
          }
          break;
        } case L"prepend-move"_fnv1a: {
          auto moved = ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node();
          ProcessPatch(ctx.node().prepend_move(moved), current.children());
          break;
        } case L"append-move"_fnv1a: {
          auto moved = ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node();
          ProcessPatch(ctx.node().append_move(moved), current.children());
          break;
        } case L"attribute"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ProcessPatch({ ctx.node().attribute(name), ctx.node() }, current.children());
          break;
        } case L"attributes"_fnv1a: {
          for ( auto &attr : ctx.node().attributes() )
            ProcessPatch({ attr, ctx.node() }, current.children());
          break;
        } case L"child"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ProcessPatch(ctx.node().child(name), current.children());
          break;
        } case L"children"_fnv1a: {
          for ( auto &child : ctx.node().children() )
            ProcessPatch(child, current.children());
          break;
        } case L"find-child-by-attribute"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          auto attr_name = current.attribute(xorstr_(L"attr-name")).value();
          auto attr_value = current.attribute(xorstr_(L"attr-value")).value();
          ProcessPatch(ctx.node().find_child_by_attribute(name, attr_name, attr_value), current.children());
          break;
        } case L"first-attribute"_fnv1a: {
          ProcessPatch({ ctx.node().first_attribute(), ctx.node() }, current.children());
          break;
        } case L"last-attribute"_fnv1a: {
          ProcessPatch({ ctx.node().last_attribute(), ctx.node() }, current.children());
          break;
        } case L"first-child"_fnv1a: {
          ProcessPatch(ctx.node().first_child(), current.children());
          break;
        } case L"last-child"_fnv1a: {
          ProcessPatch(ctx.node().last_child(), current.children());
          break;
        } case L"first-element-by-path"_fnv1a: {
          auto path = current.attribute(xorstr_(L"path")).value();
          ProcessPatch(ctx.node().first_element_by_path(path), current.children());
          break;
        } case L"insert-child-after"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          ProcessPatch(ctx.node().insert_child_after(name, node), current.children());
          break;
        } case L"insert-child-before"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          ProcessPatch(ctx.node().insert_child_before(name, node), current.children());
          break;
        } case L"insert-copy-after"_fnv1a: {
          auto proto = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value()).node();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          ProcessPatch(ctx.node().insert_copy_after(proto, node), current.children());
          break;
        } case L"insert-copy-before"_fnv1a: {
          auto proto = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value()).node();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          ProcessPatch(ctx.node().insert_copy_before(proto, node), current.children());
          break;
        } case L"insert-move-after"_fnv1a: {
          auto moved = ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          ProcessPatch(ctx.node().insert_move_after(moved, node), current.children());
          break;
        } case L"insert-move-before"_fnv1a: {
          auto moved = ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          ProcessPatch(ctx.node().insert_move_before(moved, node), current.children());
          break;
        } case L"next-sibling"_fnv1a: {
          ProcessPatch(ctx.node().next_sibling(), current.children());
          break;
        } case L"remove-attribute"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ctx.node().remove_attribute(name);
          break;
        } case L"remove-attributes"_fnv1a: {
          ctx.node().remove_attributes();
          break;
        } case L"remove-child"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ctx.node().remove_child(name);
          break;
        } case L"remove-children"_fnv1a: {
          ctx.node().remove_children();
          break;
        } case L"root"_fnv1a: {
          ProcessPatch(ctx.node().root(), current.children());
          break;
        } case L"set-name"_fnv1a: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ctx.node().set_name(name);
          break;
        } case L"set-value"_fnv1a: {
          auto value = current.attribute(xorstr_(L"value")).value();
          ctx.node().set_value(value);
          break;
        } case L"remove"_fnv1a: {
          ctx.node().parent().remove_child(ctx.node());
          return; // context node is removed, nothing more to do.
        }
      }
    }
  }
}
