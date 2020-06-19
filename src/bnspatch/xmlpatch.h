#pragma once

#include <filesystem>
#include <unordered_map>
#include <unordered_set>
#include <map>

#include <fnv1a.h>
#include <pugixml.hpp>

#include "binary_reader.h"

const std::multimap<std::filesystem::path, std::vector<std::pair<std::wstring, std::wstring>>> addon_files();
const std::filesystem::path &addons_path();
pugi::xml_parse_result deserialize_document(const void *mem, const uint32_t size, pugi::xml_document &document);
void deserialize_element(pugi::xml_node &parent, binary_reader &reader);
void deserialize_node(pugi::xml_node &parent, binary_reader &reader);
void deserialize_text(pugi::xml_node &parent, binary_reader &reader);
const std::filesystem::path &documents_path();
std::vector<std::pair<std::wstring, std::wstring>> get_relevant_addons(const wchar_t *xml);
std::vector<pugi::xml_node> get_relevant_patches(const wchar_t *xml);
void patch_xml(pugi::xml_document &src, const std::vector<pugi::xml_node> &patches);
const pugi::xml_document &patches_document();
void preprocess(pugi::xml_document &patches_doc, const std::filesystem::path &path, std::unordered_set<fnv1a::type> &include_guard);
const std::filesystem::path &patches_path();
void process_patch(
  const pugi::xpath_node &ctx,
  const pugi::xml_object_range<pugi::xml_node_iterator> &children,
  std::unordered_map<fnv1a::type, pugi::xml_node> &saved_nodes);
pugi::xml_parse_result try_load_file(
  pugi::xml_document &document,
  const std::filesystem::path &path,
  unsigned int options = pugi::parse_default,
  pugi::xml_encoding encoding = pugi::encoding_auto);
