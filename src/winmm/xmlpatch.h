#pragma once
#include <pugixml/pugixml.hpp>
#include "xmlreader.h"

extern pugi::xml_document const g_XML_PATCHES;

void process_patch(
  pugi::xpath_node const &context,
  pugi::xml_object_range<pugi::xml_node_iterator> const &children);

void process_xmldoc(XmlElement const *src, pugi::xml_node &dst);
