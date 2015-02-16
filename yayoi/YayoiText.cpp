#include "YayoiText.h"

using namespace yayoi;

void YayoiText::Clear()
{
	m_TextWindow->textBox1->Clear();
}

void YayoiText::Put(const char *msg)
{
	m_TextWindow->textBox1->Text += gcnew String(msg);
}

void YayoiText::Crlf()
{
	m_TextWindow->textBox1->Text += "\r\n";
}

void YayoiText::Option(const char *msg, int idx)
{
	m_SelectWindow->AddOption(gcnew String(msg), idx);
}
