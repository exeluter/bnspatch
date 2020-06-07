#pragma once
#include <filesystem>
#include <unordered_map>

#include <fnv1a.h>
#include <pugixml.hpp>

void process_patch(
  pugi::xpath_node const &ctx,
  pugi::xml_object_range<pugi::xml_node_iterator> const &children,
  std::unordered_map<fnv1a::type, pugi::xml_node> &saved_nodes);

const pugi::xml_document &patches_document();
const std::filesystem::path &patches_file_path();
