#pragma once
#include <pugixml/pugixml.hpp>

extern pugi::xml_document const g_PatchesDocument;

void ProcessPatch(
  pugi::xpath_node const &ctx,
  pugi::xml_object_range<pugi::xml_node_iterator> const &children);
