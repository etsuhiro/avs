#pragma once

#include "AvsCore/VariableListener.h"
#include <map>

namespace avs {

	class AvsVariable : public avs::VariableListener
	{
		std::map<unsigned int, int> m_var;

	public:
		virtual void Set(const char* name, int val);
		virtual int Get(const char* name);

		void Init(char *pBuf);
	};
}
