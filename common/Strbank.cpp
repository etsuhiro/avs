#include "Strbank.h"

//#define SELF_DEBUG

void Strbank::Init()
{
	m_item.clear();
}

int Strbank::Add(const std::string& str)
{
	return m_item[str];
}

void Strbank::Calc(int ofs)
{
	for (strint::iterator it=m_item.begin(); it!=m_item.end(); it++){
		(*it).second = ofs;
		ofs += (*it).first.length()+1;
	}
}

int Strbank::Get(const std::string& str)
{
	return m_item[str];
}

void Strbank::Dump()
{
	for (strint::iterator it=m_item.begin(); it!=m_item.end(); it++){
		output( (*it).first.c_str(), (*it).first.length()+1 );
	}
}


#if defined(SELF_DEBUG)

#include <cstdio>

class myStrbank : public Strbank {
	void output(const char *str, int size)
	{
		printf("%2d:%-5s (%d)\n", Get(str), str, size);
	}
};

int main()
{
	myStrbank mystr;
	mystr.Add("One");
	mystr.Add("Two");
	mystr.Add("Three");
	mystr.Add("Four");
	mystr.Add("One");
	mystr.Add("Two");
	mystr.Calc();
	mystr.Dump();
}

#endif

