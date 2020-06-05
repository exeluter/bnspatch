#pragma once
#include <pugixml.hpp>

extern pugi::xml_document const g_doc;

void process_patch(
  pugi::xpath_node const &ctx,
  pugi::xml_object_range<pugi::xml_node_iterator> const &children);
