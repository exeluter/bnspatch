#pragma once

#include <filesystem>
#include <queue>
#include <unordered_map>

#include <fnv1a.h>
#include <pugixml.hpp>

void process_patch(
  const pugi::xpath_node &ctx,
  const pugi::xml_object_range<pugi::xml_node_iterator> &children,
  std::unordered_map<fnv1a::type, pugi::xml_node> &saved_nodes);

const pugi::xml_document &patches_document();
const std::filesystem::path &patches_file_path();
std::queue<pugi::xml_node> get_xml_patches(const wchar_t *xml);
void patch_xml(pugi::xml_document &src, std::queue<pugi::xml_node> queue);
pugi::xml_parse_result deserialize_document(const void *mem, const uint32_t size, pugi::xml_document &document);
