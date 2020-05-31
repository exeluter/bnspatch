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
#include <wil/stl.h>
#include <wil/win32_helpers.h>
#ifdef NDEBUG
#include <xorstr.hpp>
#else
#define xorstr_(str) (str)
#endif

#include <pe/module.h>

pugi::xml_document load_profile(const fs::path &path) {
  pugi::xml_document doc;

load_patches:
  const auto result = doc.load_file(path.c_str());
  if ( !result && result.status != pugi::xml_parse_status::status_file_not_found ) {
    const auto text = fmt::format(xorstr_("{} at offset {}.\nDo you want to try again?"), result.description(), result.offset);
    switch ( MessageBoxA(nullptr, text.c_str(), xorstr_("bnspatch"), MB_CANCELTRYCONTINUE | MB_ICONERROR) ) {
      case IDTRYAGAIN: goto load_patches;
      case IDCONTINUE: break;
      default: exit(1);
    }
  }
  return pugi::xml_document();
}

pugi::xml_document const g_doc = []() {
  std::wstring path;
  wil::unique_cotaskmem_string documents;

  if ( SUCCEEDED(wil::TryGetEnvironmentVariableW(xorstr_(L"BNS_PROFILE_XML"), path)) )
    return load_profile(path);
  if ( SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &documents) == S_OK ) {
    const auto module = pe::get_module();
    switch ( fnv1a::make_hash(module->base_name(), false) ) {
      case L"Client.exe"_fnv1ai:
        return load_profile(fs::path(documents.get()).append(xorstr_(L"BnS\\patches.xml")));
      case L"BNSR.exe"_fnv1ai:
        return load_profile(fs::path(documents.get()).append(xorstr_(L"BNSR\\patches.xml")));
      default:
        return load_profile(fs::path(module->full_name()).remove_filename().append(xorstr_(L"patches.xml")));
    }
  }
  return pugi::xml_document();
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

  for ( const auto &current : children ) {
    if ( ctx.attribute() ) {
      switch ( fnv1a::make_hash(current.name(), false) ) {
        case L"parent"_fnv1ai: {
          process_patch(ctx.parent(), current.children());
          break;
        } case L"set-name"_fnv1ai: {
          ctx.attribute().set_name(current.attribute(xorstr_(L"name")).value());
          break;
        } case L"set-value"_fnv1ai: {
          ctx.attribute().set_value(current.attribute(xorstr_(L"value")).value());
          break;
        } case L"previous-attribute"_fnv1ai: {
          process_patch({ ctx.attribute().previous_attribute(), ctx.parent() }, current.children());
          break;
        } case L"next-attribute"_fnv1ai: {
          process_patch({ ctx.attribute().next_attribute(), ctx.parent() }, current.children());
          break;
        } case L"insert-attribute-before"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          process_patch({ ctx.node().insert_attribute_before(name, ctx.attribute()), ctx.node() }, current.children());
          break;
        } case L"insert-attribute-after"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          process_patch({ ctx.node().insert_attribute_after(name, ctx.attribute()), ctx.node() }, current.children());
          break;
        } case L"remove"_fnv1ai: {
          ctx.parent().remove_attribute(ctx.attribute());
          return; // context attribute is removed, nothing more to do.
        }
      }
    }
    else {
      switch ( fnv1a::make_hash(current.name(), false) ) {
        case L"parent"_fnv1ai: {
          process_patch(ctx.parent(), current.children());
          break;
        } case L"select-node"_fnv1ai: {
          auto query = current.attribute(xorstr_(L"query")).value();
          process_patch(ctx.node().select_node(query), current.children());
          break;
        } case L"select-nodes"_fnv1ai: {
          auto query = current.attribute(xorstr_(L"query")).value();
          for ( auto &node : ctx.node().select_nodes(query) )
            process_patch(node, current.children());
          break;
        } case L"prepend-attribute"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name"));
          process_patch({ ctx.node().prepend_attribute(name.value()), ctx.node() }, current.children());
          break;
        } case L"append-attribute"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name"));
          process_patch({ ctx.node().append_attribute(name.value()), ctx.node() }, current.children());
          break;
        } case L"prepend-child"_fnv1ai: {
          if ( auto name = current.attribute(xorstr_(L"name")) ) {
            process_patch(ctx.node().prepend_child(name.value()), current.children());
          }
          else {
            std::optional<xml_node_type> t;
            if ( const auto type = current.attribute(xorstr_(L"type")) ) {
              std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
              auto s = converter.to_bytes(type.value());
              t = magic_enum::enum_cast<xml_node_type>(s);
            }
            process_patch(ctx.node().append_child(t.value_or(xml_node_type::node_element)), current.children());
          }
          break;
        } case L"append-buffer"_fnv1ai: {
          const auto s = std::wstring_view(current.text().get());
          ctx.node().append_buffer(s.data(), s.size(), pugi::parse_default | pugi::parse_fragment);
          break;
        } case L"append-child"_fnv1ai: {
          if ( const auto name = current.attribute(xorstr_(L"name")) ) {
            process_patch(ctx.node().append_child(name.value()), current.children());
          }
          else {
            std::optional<xml_node_type> t;
            if ( const auto type = current.attribute(xorstr_(L"type")) ) {
              std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
              auto s = converter.to_bytes(type.value());
              t = magic_enum::enum_cast<xml_node_type>(s);
            }
            process_patch(ctx.node().append_child(t.value_or(xml_node_type::node_element)), current.children());
          }
          break;
        } case L"prepend-copy"_fnv1ai: {
          auto proto = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value());
          if ( proto.attribute() ) {
            process_patch({ ctx.node().prepend_copy(proto.attribute()), ctx.node() }, current.children());
          }
          else {
            process_patch(ctx.node().prepend_copy(proto.node()), current.children());
          }
          break;
        } case L"append-copy"_fnv1ai: {
          auto proto = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value());
          if ( proto.attribute() ) {
            process_patch({ ctx.node().append_copy(proto.attribute()), ctx.node() }, current.children());
          }
          else {
            process_patch(ctx.node().append_copy(proto.node()), current.children());
          }
          break;
        } case L"prepend-move"_fnv1ai: {
          auto moved = ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node();
          process_patch(ctx.node().prepend_move(moved), current.children());
          break;
        } case L"append-move"_fnv1ai: {
          auto moved = ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node();
          process_patch(ctx.node().append_move(moved), current.children());
          break;
        } case L"attribute"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          process_patch({ ctx.node().attribute(name), ctx.node() }, current.children());
          break;
        } case L"attributes"_fnv1ai: {
          for ( auto &attr : ctx.node().attributes() )
            process_patch({ attr, ctx.node() }, current.children());
          break;
        } case L"child"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          process_patch(ctx.node().child(name), current.children());
          break;
        } case L"children"_fnv1ai: {
          for ( auto &child : ctx.node().children() )
            process_patch(child, current.children());
          break;
        } case L"find-child-by-attribute"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          auto attr_name = current.attribute(xorstr_(L"attr-name")).value();
          auto attr_value = current.attribute(xorstr_(L"attr-value")).value();
          process_patch(ctx.node().find_child_by_attribute(name, attr_name, attr_value), current.children());
          break;
        } case L"first-attribute"_fnv1ai: {
          process_patch({ ctx.node().first_attribute(), ctx.node() }, current.children());
          break;
        } case L"last-attribute"_fnv1ai: {
          process_patch({ ctx.node().last_attribute(), ctx.node() }, current.children());
          break;
        } case L"first-child"_fnv1ai: {
          process_patch(ctx.node().first_child(), current.children());
          break;
        } case L"last-child"_fnv1ai: {
          process_patch(ctx.node().last_child(), current.children());
          break;
        } case L"first-element-by-path"_fnv1ai: {
          auto path = current.attribute(xorstr_(L"path")).value();
          process_patch(ctx.node().first_element_by_path(path), current.children());
          break;
        } case L"insert-child-after"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          process_patch(ctx.node().insert_child_after(name, node), current.children());
          break;
        } case L"insert-child-before"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          process_patch(ctx.node().insert_child_before(name, node), current.children());
          break;
        } case L"insert-copy-after"_fnv1ai: {
          auto proto = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value()).node();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          process_patch(ctx.node().insert_copy_after(proto, node), current.children());
          break;
        } case L"insert-copy-before"_fnv1ai: {
          auto proto = ctx.node().select_node(current.attribute(xorstr_(L"proto-query")).value()).node();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          process_patch(ctx.node().insert_copy_before(proto, node), current.children());
          break;
        } case L"insert-move-after"_fnv1ai: {
          auto moved = ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          process_patch(ctx.node().insert_move_after(moved, node), current.children());
          break;
        } case L"insert-move-before"_fnv1ai: {
          auto moved = ctx.node().select_node(current.attribute(xorstr_(L"moved-query")).value()).node();
          auto node = ctx.node().select_node(current.attribute(xorstr_(L"node-query")).value()).node();
          process_patch(ctx.node().insert_move_before(moved, node), current.children());
          break;
        } case L"next-sibling"_fnv1ai: {
          process_patch(ctx.node().next_sibling(), current.children());
          break;
        } case L"remove-attribute"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ctx.node().remove_attribute(name);
          break;
        } case L"remove-attributes"_fnv1ai: {
          ctx.node().remove_attributes();
          break;
        } case L"remove-child"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ctx.node().remove_child(name);
          break;
        } case L"remove-children"_fnv1ai: {
          ctx.node().remove_children();
          break;
        } case L"root"_fnv1ai: {
          process_patch(ctx.node().root(), current.children());
          break;
        } case L"set-name"_fnv1ai: {
          auto name = current.attribute(xorstr_(L"name")).value();
          ctx.node().set_name(name);
          break;
        } case L"set-value"_fnv1ai: {
          auto value = current.attribute(xorstr_(L"value")).value();
          ctx.node().set_value(value);
          break;
        } case L"remove"_fnv1ai: {
          ctx.node().parent().remove_child(ctx.node());
          return; // context node is removed, nothing more to do.
        }
      }
    }
  }
}
