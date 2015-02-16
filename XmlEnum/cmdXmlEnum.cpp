#include <cstdio>

#include "XmlEnum.h"

int main(int argc, char *argv[])
{
	if (argc==2){
		TiXmlDocument xml;
		if (xml.LoadFile(argv[1])){
			XmlEnum xmls;
			xmls.ReadSchema(xml.RootElement());

			std::string filename(argv[1]);
			std::string::size_type pos = filename.find_last_of('.');
			if (pos!=std::string::npos){
				filename.erase(pos);	// ägí£éqÇè¡Ç∑
			}
			xmls.PrintEnum(filename.c_str());
		}
	}
}
