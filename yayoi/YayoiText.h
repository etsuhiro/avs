#pragma once

#include "AvsCore/TextListener.h"
#include <vcclr.h>
#include "TextWindow.h"
#include "SelectWindow.h"

namespace yayoi
{
	class YayoiText : public avs::avsTextListener {
		virtual void Clear();
		virtual void Put(const char *msg);
		virtual void Crlf();
		virtual void Option(const char *msg, int num);

	public:
		YayoiText(TextWindow^ hText, SelectWindow^ hSelect) : m_TextWindow(hText), m_SelectWindow(hSelect) {}

	private:
		gcroot<TextWindow^>		m_TextWindow;
		gcroot<SelectWindow^>	m_SelectWindow;
	};
}
