#pragma once

#include <vector>
#include <map>
#include <string>
#include "tinyxml/tinyxml.h"

class XmlEnum;

typedef std::map<unsigned int, std::string>	hashmap_t;

class XmlBin {
public:
	int Conv(std::vector<char>& outbuf, hashmap_t& hashmap, const TiXmlDocument& xml, XmlEnum* xenum);
};
