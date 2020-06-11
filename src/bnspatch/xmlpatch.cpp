#include "xmlpatch.h"
#include <phnt_windows.h>
#include <phnt.h>
#include <ShlObj.h>
#include <KnownFolders.h>

#include <algorithm>
#include <codecvt>
#include <filesystem>
#include <optional>
#include <unordered_set>

#include "versioninfo.h"
#include <fmt/format.h>
#include <fnv1a.h>
#include <magic_enum.hpp>
#include <pe/module.h>
#include <pugixml.hpp>
#include <wil/stl.h>
#include <wil/win32_helpers.h>
#include <xorstr.hpp>

pugi::xml_parse_result try_load_file(
  pugi::xml_document &document,
  const std::filesystem::path &path,
  unsigned int options = pugi::parse_default,
  pugi::xml_encoding encoding = pugi::encoding_auto)
{
  const auto result = document.load_file(path.c_str(), options, encoding);
try_again:
  if ( !result && result.status != pugi::xml_parse_status::status_file_not_found ) {
    switch ( MessageBoxA(
      nullptr,
      fmt::format(xorstr_("{}({}): {}"), path.string(), result.offset, result.description()).c_str(),
      xorstr_("bnspatch"),
      MB_ICONERROR | MB_CANCELTRYCONTINUE | MB_ICONERROR) ) {
      case IDTRYAGAIN: goto try_again;
      case IDCONTINUE: break;
      default: exit(0);
    }
  }
  return result;
}

void preprocess(pugi::xml_document &doc, const std::filesystem::path &current_directory, std::unordered_set<fnv1a::type> &previously_loaded)
{
  const auto &first_child = doc.document_element().first_child();
  for ( auto &child : doc.children() ) {
    if ( child.type() == pugi::node_pi && !_wcsicmp(child.name(), xorstr_(L"include")) ) {
      pugi::xml_node include = child;

      auto result = std::wstring();
      if ( SUCCEEDED(wil::ExpandEnvironmentStringsW(child.value(), result)) ) {
        auto include_filter = std::filesystem::path(result);
        if ( include_filter.is_relative() )
          include_filter = current_directory / include_filter;

        auto find_file_data = WIN32_FIND_DATAW();
        auto find_file_handle = FindFirstFileW(include_filter.c_str(), &find_file_data);
        if ( find_file_handle != INVALID_HANDLE_VALUE ) {
          const auto parent_path = include_filter.parent_path();
          do {
            const auto path = std::filesystem::canonical(parent_path / find_file_data.cFileName);
            auto document = pugi::xml_document();
            if ( previously_loaded.emplace(fnv1a::make_hash(path.c_str(), towupper)).second
              && try_load_file(document, path, pugi::parse_default | pugi::parse_pi)
              && !_wcsicmp(document.document_element().name(), xorstr_(L"patches")) ) {

              preprocess(document, parent_path, previously_loaded);
              doc.document_element().insert_child_before(pugi::node_comment, first_child).set_value(path.c_str());

              for ( const auto &ic : document.document_element().children(xorstr_(L"patch")) )
                doc.document_element().insert_copy_before(ic, first_child);
            }
          } while ( FindNextFileW(find_file_handle, &find_file_data) );
          FindClose(find_file_handle);
        }
      }
      doc.remove_child(include);
    }
  }
}

const std::filesystem::path &patches_file_path()
{
  static std::once_flag once_flag;
  static std::filesystem::path path;

  try {
    std::call_once(once_flag, [](std::filesystem::path &path) {
      auto result = wil::unique_cotaskmem_string();
      if ( SUCCEEDED(wil::TryGetEnvironmentVariableW(xorstr_(L"BNS_PROFILE_XML"), result)) && result ) {
        path = std::filesystem::canonical(result.get());
        return;
      }

      if ( SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Documents, KF_FLAG_DEFAULT, nullptr, &result)) ) {
        const wchar_t *fileName;
        if ( GetModuleVersionInfo(nullptr, xorstr_(L"\\StringFileInfo\\*\\OriginalFilename"), &(LPCVOID &)fileName) >= 0 ) {
          const auto documents = std::filesystem::path(result.get());
          switch ( fnv1a::make_hash(fileName, towupper) ) {
            case L"Client.exe"_fnv1au:
              path = std::filesystem::canonical(documents / xorstr_(L"BnS\\patches.xml"));
              return;
            case L"BNSR.exe"_fnv1au:
              path = std::filesystem::canonical(documents / xorstr_(L"BnS\\patches.xml"));
              return;
          }
        }
        throw std::exception();
      }
    }, path);
  } catch ( ... ) {}
  return path;
}

const pugi::xml_document &patches_document()
{
  static std::once_flag once_flag;
  static pugi::xml_document document;

  try {
    std::call_once(once_flag, [](pugi::xml_document &document) {
      auto previously_loaded = std::unordered_set<fnv1a::type>();
      if ( previously_loaded.emplace(fnv1a::make_hash(patches_file_path().c_str(), towupper)).second
        && try_load_file(document, patches_file_path(), pugi::parse_default | pugi::parse_pi)
        && !_wcsicmp(document.document_element().name(), xorstr_(L"patches")) ) {
        preprocess(document, patches_file_path().parent_path(), previously_loaded);
#ifdef _DEBUG
        document.save_file(patches_file_path().parent_path().append(xorstr_(L"preprocessed.xml")).c_str(), L"  ");
#endif
      }
    }, document);
  } catch ( ... ) {}
  return document;
}

void process_patch(
  const pugi::xpath_node &ctx,
  const pugi::xml_object_range<pugi::xml_node_iterator> &children,
  std::unordered_map<fnv1a::type, pugi::xml_node> &saved_nodes)
{
  for ( const auto &current : children ) {
    if ( ctx.attribute() ) {
      switch ( fnv1a::make_hash(current.name(), towupper) ) {
        case L"parent"_fnv1au: // ok
          process_patch(ctx.parent(), current.children(), saved_nodes);
          break;

        case L"set-name"_fnv1au: // ok
          ctx.attribute().set_name(current.attribute(xorstr_(L"name")).value());
          break;

        case L"set-value"_fnv1au: // ok
          ctx.attribute().set_value(current.attribute(xorstr_(L"value")).value());
          break;

        case L"previous-attribute"_fnv1au: // ok
          process_patch(pugi::xpath_node(ctx.attribute().previous_attribute(), ctx.parent()), current.children(), saved_nodes);
          break;

        case L"next-attribute"_fnv1au: // ok
          process_patch(pugi::xpath_node(ctx.attribute().next_attribute(), ctx.parent()), current.children(), saved_nodes);
          break;

        case L"insert-attribute-before"_fnv1au: { // ok
          auto attribute = ctx.node().insert_attribute_before(current.attribute(xorstr_(L"name")).value(), ctx.attribute());
          if ( auto value = current.attribute(xorstr_(L"value")) )
            attribute.set_value(value.value());
          process_patch(pugi::xpath_node(attribute, ctx.node()), current.children(), saved_nodes);
          break;
        }
        case L"insert-attribute-after"_fnv1au: { // ok
          auto attribute = ctx.node().insert_attribute_after(current.attribute(xorstr_(L"name")).value(), ctx.attribute());
          if ( auto value = current.attribute(xorstr_(L"value")) )
            attribute.set_value(value.value());
          process_patch(pugi::xpath_node(attribute, ctx.node()), current.children(), saved_nodes);
          break;
        }
        case L"remove"_fnv1au: // ok
          ctx.parent().remove_attribute(ctx.attribute());
          return;
      }
    } else {
      switch ( fnv1a::make_hash(current.name(), towupper) ) {
        case L"parent"_fnv1au: // ok
          process_patch(ctx.parent(), current.children(), saved_nodes);
          break;

        case L"select-node"_fnv1au: // ok
          process_patch(ctx.node().select_node(current.attribute(xorstr_(L"query")).value()), current.children(), saved_nodes);
          break;

        case L"select-nodes"_fnv1au: // ok
          for ( auto &node : ctx.node().select_nodes(current.attribute(xorstr_(L"query")).value()) )
            process_patch(node, current.children(), saved_nodes);
          break;

        case L"prepend-attribute"_fnv1au: { // ok
          auto attribute = ctx.node().prepend_attribute(current.attribute(xorstr_(L"name")).value());
          if ( auto value = current.attribute(xorstr_(L"value")) )
            attribute.set_value(value.value());
          process_patch(pugi::xpath_node(attribute, ctx.node()), current.children(), saved_nodes);
          break;
        }
        case L"append-attribute"_fnv1au: { // ok
          auto attribute = ctx.node().append_attribute(current.attribute(xorstr_(L"name")).value());
          if ( auto value = current.attribute(xorstr_(L"value")) )
            attribute.set_value(value.value());
          process_patch(pugi::xpath_node(attribute, ctx.node()), current.children(), saved_nodes);
          break;
        }
        case L"prepend-child"_fnv1au: // ok
          process_patch(ctx.node().prepend_child(current.attribute(xorstr_(L"name")).value()), current.children(), saved_nodes);
          break;

        case L"append-buffer"_fnv1au: { // ok
          size_t size;
          const auto text = current.text();
          if ( SUCCEEDED(StringCbLengthW(text.get(), STRSAFE_MAX_CCH * sizeof(wchar_t), &size)) )
            ctx.node().append_buffer(text.get(), size, pugi::parse_default | pugi::parse_declaration | pugi::parse_fragment | pugi::parse_trim_pcdata, pugi::encoding_utf16);
          break;
        }
        case L"append-child"_fnv1au: // ok
          process_patch(ctx.node().append_child(current.attribute(xorstr_(L"name")).value()), current.children(), saved_nodes);
          break;

        case L"prepend-copy"_fnv1au: // ok
          if ( auto it = saved_nodes.find(fnv1a::make_hash(current.attribute(xorstr_(L"node-key")).value(), towupper)); it != saved_nodes.end() )
            process_patch(ctx.node().prepend_copy(it->second), current.children(), saved_nodes);
          break;

        case L"append-copy"_fnv1au: // ok
          if ( auto it = saved_nodes.find(fnv1a::make_hash(current.attribute(xorstr_(L"node-key")).value(), towupper)); it != saved_nodes.end() )
            process_patch(ctx.node().append_copy(it->second), current.children(), saved_nodes);
          break;

        case L"prepend-move"_fnv1au: // ok
          if ( auto it = saved_nodes.find(fnv1a::make_hash(current.attribute(xorstr_(L"node-key")).value(), towupper)); it != saved_nodes.end() )
            process_patch(ctx.node().prepend_move(it->second), current.children(), saved_nodes);
          break;

        case L"append-move"_fnv1au: // ok
          if ( auto it = saved_nodes.find(fnv1a::make_hash(current.attribute(xorstr_(L"node-key")).value(), towupper)); it != saved_nodes.end() )
            process_patch(ctx.node().append_move(it->second), current.children(), saved_nodes);
          break;

        case L"attribute"_fnv1au: // ok
          process_patch(pugi::xpath_node(ctx.node().attribute(current.attribute(xorstr_(L"name")).value()), ctx.node()), current.children(), saved_nodes);
          break;

        case L"attributes"_fnv1au: // ok
          for ( auto &attr : ctx.node().attributes() )
            process_patch(pugi::xpath_node(attr, ctx.node()), current.children(), saved_nodes);
          break;

        case L"child"_fnv1au: // ok
          process_patch(ctx.node().child(current.attribute(xorstr_(L"name")).value()), current.children(), saved_nodes);
          break;

        case L"children"_fnv1au: // ok
          if ( const auto name = current.attribute(xorstr_(L"name")) ) {
            for ( auto &child : ctx.node().children(name.value()) )
              process_patch(child, current.children(), saved_nodes);
          } else {
            for ( auto &child : ctx.node().children() )
              process_patch(child, current.children(), saved_nodes);
          }
          break;

        case L"find-child-by-attribute"_fnv1au: // ok
          if ( const auto name = current.attribute(xorstr_(L"name")) ) {
            process_patch(ctx.node().find_child_by_attribute(
              name.value(),
              current.attribute(xorstr_(L"attribute-name")).value(),
              current.attribute(xorstr_(L"attribute-value")).value()),
              current.children(), saved_nodes);
          } else {
            process_patch(ctx.node().find_child_by_attribute(
              current.attribute(xorstr_(L"attribute-name")).value(),
              current.attribute(xorstr_(L"attribute-value")).value()),
              current.children(), saved_nodes);
          }
          break;

        case L"first-attribute"_fnv1au: // ok
          process_patch(pugi::xpath_node(ctx.node().first_attribute(), ctx.node()), current.children(), saved_nodes);
          break;

        case L"last-attribute"_fnv1au: // ok
          process_patch(pugi::xpath_node(ctx.node().last_attribute(), ctx.node()), current.children(), saved_nodes);
          break;

        case L"first-child"_fnv1au: // ok
          process_patch(ctx.node().first_child(), current.children(), saved_nodes);
          break;

        case L"last-child"_fnv1au: // ok
          process_patch(ctx.node().last_child(), current.children(), saved_nodes);
          break;

        case L"first-element-by-path"_fnv1au: // ok
          process_patch(ctx.node().first_element_by_path(current.attribute(xorstr_(L"path")).value()), current.children(), saved_nodes);
          break;

        case L"insert-sibling-after"_fnv1au: // ok
          process_patch(ctx.node().parent().insert_child_after(current.attribute(xorstr_(L"name")).value(), ctx.node()), current.children(), saved_nodes);
          break;

        case L"insert-sibling-before"_fnv1au: // ok
          process_patch(ctx.node().parent().insert_child_before(current.attribute(xorstr_(L"name")).value(), ctx.node()), current.children(), saved_nodes);
          break;

        case L"insert-copy-after"_fnv1au: // ok
          if ( auto it = saved_nodes.find(fnv1a::make_hash(current.attribute(xorstr_(L"node-key")).value(), towupper)); it != saved_nodes.end() )
            process_patch(it->second.parent().insert_move_before(ctx.node(), it->second), current.children(), saved_nodes);
          break;

        case L"insert-copy-before"_fnv1au: // ok
          if ( auto it = saved_nodes.find(fnv1a::make_hash(current.attribute(xorstr_(L"node-key")).value(), towupper)); it != saved_nodes.end() )
            process_patch(it->second.parent().insert_copy_before(ctx.node(), it->second), current.children(), saved_nodes);
          break;

        case L"insert-move-after"_fnv1au: // ok
          if ( auto it = saved_nodes.find(fnv1a::make_hash(current.attribute(xorstr_(L"node-key")).value(), towupper)); it != saved_nodes.end() )
            process_patch(it->second.parent().insert_move_after(ctx.node(), it->second), current.children(), saved_nodes);
          break;

        case L"insert-move-before"_fnv1au: // ok
          if ( auto it = saved_nodes.find(fnv1a::make_hash(current.attribute(xorstr_(L"node-key")).value(), towupper)); it != saved_nodes.end() )
            process_patch(it->second.parent().insert_move_before(ctx.node(), it->second), current.children(), saved_nodes);
          break;

        case L"next-sibling"_fnv1au: // ok
          process_patch(ctx.node().next_sibling(), current.children(), saved_nodes);
          break;

        case L"remove-attribute"_fnv1au: // ok
          ctx.node().remove_attribute(current.attribute(xorstr_(L"name")).value());
          break;

        case L"remove-attributes"_fnv1au: // ok
          ctx.node().remove_attributes();
          break;

        case L"remove-child"_fnv1au: // ok
          ctx.node().remove_child(current.attribute(xorstr_(L"name")).value());
          break;

        case L"remove-children"_fnv1au: // ok
          ctx.node().remove_children();
          break;

        case L"root"_fnv1au: // ok
          process_patch(ctx.node().root(), current.children(), saved_nodes);
          break;

        case L"set-name"_fnv1au: // ok
          ctx.node().set_name(current.attribute(xorstr_(L"name")).value());
          break;

        case L"set-value"_fnv1au: // ok
          ctx.node().set_value(current.attribute(xorstr_(L"value")).value());
          break;

        case L"assign-node-key"_fnv1au: // ok
          saved_nodes.insert_or_assign(fnv1a::make_hash(current.attribute(xorstr_(L"node-key")).value(), towupper), ctx.node());
          break;

        case L"remove"_fnv1au: // ok
          ctx.node().parent().remove_child(ctx.node());
          return;
      }
    }
  }
}
