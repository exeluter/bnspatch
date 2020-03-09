#include "xmlpatch.h"
#include <ntdll.h>
#include <ShlObj.h>
#include <KnownFolders.h>

#include <codecvt>
#include <filesystem>
namespace fs = std::filesystem;

#include <fmt/format.h>
#include <pugixml/pugixml.hpp>
#include <magic_enum.hpp>
#include <xorstr.hpp>

#include "xmlreader.h"

pugi::xml_document const g_XML_PATCHES = []() {
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

// HACK: Using xorstr in process_patch makes compilation of the x86 build take
// a very long time for some reason, so make the macro a no-op for x86.

#ifdef _M_IX86
#pragma push_macro( "xorstr_" )
#undef xorstr_
#define xorstr_(x) (x)
#endif

void process_patch(
  pugi::xpath_node const &context,
  pugi::xml_object_range<pugi::xml_node_iterator> const &children)
{
  using pugi::xml_document;
  using pugi::xml_node;
  using pugi::xml_node_iterator;
  using pugi::xml_node_type;
  using pugi::xml_object_range;


  for ( auto const &current : children ) {
    if ( !_wcsicmp(current.name(), xorstr_(L"parent")) ) {
      process_patch(context.parent(), current.children());
    } else if ( auto context_attr = context.attribute() ) {
      if ( !_wcsicmp(current.name(), xorstr_(L"set-name")) ) {
        context_attr.set_name(current.attribute(xorstr_(L"name")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"set-value")) ) {
        context_attr.set_value(current.attribute(xorstr_(L"value")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"previous-attribute")) ) {
        process_patch({ context_attr.previous_attribute(), context.parent() }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"next-attribute")) ) {
        process_patch({ context_attr.next_attribute(), context.parent() }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-node")) ) {
        context.parent().remove_attribute(context_attr);
      }
    } else if ( auto context_node = context.node() ) {
      if ( !_wcsicmp(current.name(), xorstr_(L"select-node")) ) {
        process_patch(context_node.select_node(current.attribute(xorstr_(L"query")).value()), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"select-nodes")) ) {
        for ( auto &xn : context_node.select_nodes(current.attribute(xorstr_(L"query")).value()) ) {
          process_patch(xn, current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-attribute")) ) {
        auto const name = current.attribute(xorstr_(L"name"));
        process_patch({ context_node.prepend_attribute(name.value()), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-attribute")) ) {
        auto const name = current.attribute(xorstr_(L"name"));
        process_patch({ context_node.append_attribute(name.value()), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-child")) ) {
        if ( auto const name = current.attribute(xorstr_(L"name")) ) {
          process_patch(context_node.prepend_child(name.value()), current.children());
        } else {
          std::optional<xml_node_type> t;
          if ( auto const type = current.attribute(xorstr_(L"type")) ) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            auto s = converter.to_bytes(type.value());
            t = magic_enum::enum_cast<xml_node_type>(s);
          }
          process_patch(context_node.append_child(t.value_or(xml_node_type::node_element)), current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-buffer")) ) {
        auto const s = std::basic_string_view<pugi::char_t>(current.text().get());
        context_node.append_buffer(s.data(), s.size(), pugi::parse_default | pugi::parse_fragment);
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-child")) ) {
        if ( auto const name = current.attribute(xorstr_(L"name")) ) {
          process_patch(context_node.append_child(name.value()), current.children());
        } else {
          std::optional<xml_node_type> t;
          if ( auto const type = current.attribute(xorstr_(L"type")) ) {
            std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
            auto s = converter.to_bytes(type.value());
            t = magic_enum::enum_cast<xml_node_type>(s);
          }
          process_patch(context_node.append_child(t.value_or(xml_node_type::node_element)), current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-children")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-children")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-copy")) ) {
        auto xn = context_node.select_node(current.attribute(xorstr_(L"proto-query")).value());
        if ( auto attr = xn.attribute() ) {
          process_patch({ context_node.prepend_copy(attr), xn.parent() }, current.children());
        } else if ( auto node = xn.node() ) {
          process_patch(context_node.prepend_copy(node), current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-copy")) ) {
        auto xn = context_node.select_node(current.attribute(xorstr_(L"proto-query")).value());
        if ( auto attr = xn.attribute() ) {
          process_patch({ context_node.append_copy(attr), xn.parent() }, current.children());
        } else if ( auto node = xn.node() ) {
          process_patch(context_node.append_copy(node), current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"prepend-move")) ) {
        process_patch(
          context_node.prepend_move(context_node.select_node(current.attribute(xorstr_(L"moved-query")).value()).node()),
          current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"append-move")) ) {
        process_patch(context_node.append_move(context_node.select_node(current.attribute(xorstr_(L"moved-query")).value()).node()), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"attribute")) ) {
        process_patch({ context_node.attribute(current.attribute(xorstr_(L"name")).value()), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"attributes")) ) {
        for ( auto &attr : context_node.attributes() ) {
          process_patch({ attr, context_node }, current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"child")) ) {
        process_patch(context_node.child(current.attribute(xorstr_(L"name")).value()), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"children")) ) {
        for ( auto &child : context_node.children() ) {
          process_patch(child, current.children());
        }
      } else if ( !_wcsicmp(current.name(), xorstr_(L"find-child-by-attribute")) ) {
        process_patch(
          context_node.find_child_by_attribute(
            current.attribute(xorstr_(L"name")).value(),
            current.attribute(xorstr_(L"attr-name")).value(),
            current.attribute(xorstr_(L"attr-value")).value()),
          current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"first-attribute")) ) {
        process_patch({ context_node.first_attribute(), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"last-attribute")) ) {
        process_patch({ context_node.last_attribute(), context_node }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"first-child")) ) {
        process_patch(context_node.first_child(), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"last-child")) ) {
        process_patch(context_node.last_child(), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"first-element-by-path")) ) {
        process_patch(context_node.first_element_by_path(current.attribute(xorstr_(L"path")).value()), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-attribute-before")) ) {
        auto name = current.attribute(xorstr_(L"name")).value();
        auto attr_query = current.attribute(xorstr_(L"attr-query")).value();
        auto xn = context_node.select_node(attr_query);
        process_patch({ context_node.insert_attribute_before(name, xn.attribute()), xn.parent() }, current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-attribute-after")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-child-after")) ) {
        process_patch(
          context_node.insert_child_after(
            current.attribute(xorstr_(L"name")).value(),
            context_node.select_node(current.attribute(xorstr_(L"query")).value()).node()),
          current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-child-before")) ) {
        process_patch(
          context_node.insert_child_before(
            current.attribute(xorstr_(L"name")).value(),
            context_node.select_node(current.attribute(xorstr_(L"query")).value()).node()),
          current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-copy-after")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-copy-before")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-move-after")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"insert-move-before")) ) {
        // not implemented
      } else if ( !_wcsicmp(current.name(), xorstr_(L"next-sibling")) ) {
        process_patch(context_node.next_sibling(), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-attribute")) ) {
        context_node.remove_attribute(current.attribute(xorstr_(L"name")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-attributes")) ) {
        context_node.remove_attributes();
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-child")) ) {
        context_node.remove_child(current.attribute(xorstr_(L"name")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-children")) ) {
        context_node.remove_children();
      } else if ( !_wcsicmp(current.name(), xorstr_(L"root")) ) {
        process_patch(context_node.root(), current.children());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"set-name")) ) {
        context_node.set_name(current.attribute(xorstr_(L"value")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"set-value")) ) {
        context_node.set_value(current.attribute(xorstr_(L"value")).value());
      } else if ( !_wcsicmp(current.name(), xorstr_(L"remove-node")) ) {
        context_node.parent().remove_child(context_node);
      }
    }
  }
}

#ifdef _M_IX86
#pragma pop_macro( "xorstr_" )
#endif

void process_xmldoc(XmlElement const *src, pugi::xml_node &dst)
{
  auto node = dst.append_child(src->Name());
  for ( int i = 0; i < src->AttributeCount(); ++i )
    node.append_attribute(src->AttributeName(i)) = src->Attribute(i);
  for ( auto child = src->FirstChildElement(); child; child = child->NextElement() )
    process_xmldoc(child, node);
}
