#include <cstdio>

#include "XmlEnum.h"

int XmlEnum::Element(const char *p)
{
	int i;
	for (i=0; i<m_element.size(); i++){
		if (m_element[i]==p)
			return i;
	}
	m_element.push_back(p);
	return i;
}

int XmlEnum::Attribute(const char *p)
{
	int i;
	for (i=0; i<m_attribute.size(); i++){
		if (m_attribute[i].name==p)
			return i;
	}
	m_attribute.push_back(AttrSet());
	m_attribute.back().name = p;
	return i;
}

void XmlEnum::ReadSchema(const TiXmlElement* e)
{
	if (std::string("xsd:element") == e->Value()){
		const char* p;
		if ((p = e->Attribute("name"))){
			Element(p);
		}
	}
	if (std::string("xsd:attribute") == e->Value()){
		const char* p;
		if ((p = e->Attribute("name"))){
			int idx = Attribute(p);
			m_attribute[idx].type = XmlEnum::STRING;
			if ((p = e->Attribute("type"))){
				std::string s(p);
				if (s=="xsd:int"){
					m_attribute[idx].type = XmlEnum::INT;
				} else
				if (s=="xsd:short"){
					m_attribute[idx].type = XmlEnum::SHORT;
				} else
				if (s=="xsd:byte"){
					m_attribute[idx].type = XmlEnum::BYTE;
				} else
				if (s=="xsd:float"){
					m_attribute[idx].type = XmlEnum::FLOAT;
				}
			}
		}
	}

	for (const TiXmlNode* child = e->FirstChild(); child; child = child->NextSibling())
	{
		const TiXmlElement* childElement = child->ToElement();
		if (childElement){
			ReadSchema(childElement);	// çƒãAì«Ç›
		}
	}
}

void XmlEnum::PrintEnum(const char *fname)
{
	printf("namespace %s {\n", fname);

	printf("\tnamespace element {\n");
	printf("\t\tenum {\n");
	for (std::vector<std::string>::iterator it=m_element.begin(); it!=m_element.end(); it++){
		printf("\t\t\t%s,\n", it->c_str());
	}
	printf("\t\t};\n");
	printf("\t}\n");

	printf("\tnamespace attribute {\n");
	printf("\t\tenum {\n");
	for (std::vector<AttrSet>::iterator it=m_attribute.begin(); it!=m_attribute.end(); it++){
		printf("\t\t\t%s,\n", it->name.c_str());
	}
	printf("\t\t};\n");
	printf("\t}\n");

	printf("}\n");
}

