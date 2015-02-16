#include "stdafx.h"
#include "YayoiScriptEngine.h"
#include "TextWindow.h"

using namespace System;
using namespace System::Windows::Forms;

using namespace yayoi;

YayoiScriptEngine::YayoiScriptEngine(XmlEnum& mikuEnum, LogWindow^ hLog)
	: ScriptEngine()
	, m_mikuEnum(mikuEnum)
	, m_LogWindow(hLog)
	, m_MsgIndex(0)
{
}

void YayoiScriptEngine::ErrorInvalidElement(int cmd, int element)
{
	char buf[256];
	sprintf_s(buf, 256, "<%s>タグ内では<%s>タグを子要素に出来ません。\r\n", m_mikuEnum.ElementName(cmd), m_mikuEnum.ElementName(element));
//	MessageBox::Show( gcnew String(	buf ), "warning" );
	m_LogWindow->textBox1->Text += gcnew String( buf );
}

void YayoiScriptEngine::ErrorInvalidAttr(int cmd, int attr)
{
	char buf[256];
	sprintf_s(buf, 256, "<%s>タグに\"%s\"属性は無効です。\r\n", m_mikuEnum.ElementName(cmd), m_mikuEnum.AttributeName(attr));
//	MessageBox::Show( gcnew String(	buf ), "warning" );
	m_LogWindow->textBox1->Text += gcnew String( buf );
}

void YayoiScriptEngine::ErrorrequiredAttr(int cmd, int attr)
{
	char buf[256];
	sprintf_s(buf, 256, "<%s>タグには\"s\"属性が必須です。\r\n", m_mikuEnum.ElementName(cmd), m_mikuEnum.AttributeName(attr));
//	MessageBox::Show( gcnew String(	buf ), "warning" );
	m_LogWindow->textBox1->Text += gcnew String( buf );
}

void YayoiScriptEngine::ErrorInvalidText(const char *tag, const char* str)
{
	char buf[256];
	sprintf_s(buf, 256, "%s内で文字データは無効です。\r\n「%s」\r\n", tag, str);
//	MessageBox::Show( gcnew String(	buf ), "warning" );
	m_LogWindow->textBox1->Text += gcnew String( buf );
}

void YayoiScriptEngine::ErrorUnfoundLabel(unsigned int label)
{
	char buf[256];
	sprintf_s(buf, 256, "ラベルL%xが見つかりませんでした。\r\n", label);
	MessageBox::Show( gcnew String(	buf ), "warning" );
}

void YayoiScriptEngine::MsgIdx(int idx)
{
	m_MsgIndex = idx;
}

void YayoiScriptEngine::ScriptLoad(const char *filename)
{
	m_ReqLoadFileName = filename;

	char buf[256];
	sprintf_s(buf, 256, "load request %s\r\n", filename);
	m_LogWindow->textBox1->Text += gcnew String( buf );
}
