#pragma once

#include <string>
#include <map>

class Strbank {
	typedef std::map<std::string, int> strint;
	strint m_item;
	virtual void output(const char *str, int size) {};
public:
	virtual ~Strbank(){}
	void Init();
	int Add(const std::string& str);
	int Get(const std::string& str);
	void Calc(int ofs=0);
	void Dump();
};

