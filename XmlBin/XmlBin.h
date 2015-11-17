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

class IMikuPrint {
public:
	virtual void TagName(const char* name, int indent) {}
	virtual void TagClose(const char* name, int indent) {}
	virtual void EmptyElementTagClose(const char* name, int indent) {}
	virtual void EndTag(const char* name, int indent) {}
	virtual void AttrName(const char* name) {}
	virtual void Text(const char* text, int indent) {}
};

void mikuPrint(char *pBuf, XmlEnum& xmls, IMikuPrint& print);
