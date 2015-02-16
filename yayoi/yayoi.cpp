// yayoi.cpp : メイン プロジェクト ファイルです。

#include "XmlBin/XmlBin.h"
#include "XmlEnum/XmlEnum.h"
#include "Form1.h"

using namespace yayoi;

static XmlEnum	mikuEnum;


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// コントロールが作成される前に、Windows XP ビジュアル効果を有効にします
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	{
		TiXmlDocument schema;
		// スキーマを読む
		if (!schema.LoadFile("sink.xsd")){
			MessageBox::Show("file read error (sink.xsd)");
			exit(0);
		}
		mikuEnum.ReadSchema(schema.RootElement());
	}

	// メイン ウィンドウを作成して、実行します
	Application::Run(gcnew Form1(mikuEnum));
	return 0;
}
