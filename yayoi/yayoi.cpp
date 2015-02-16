// yayoi.cpp : ���C�� �v���W�F�N�g �t�@�C���ł��B

#include "XmlBin/XmlBin.h"
#include "XmlEnum/XmlEnum.h"
#include "Form1.h"

using namespace yayoi;

static XmlEnum	mikuEnum;


[STAThreadAttribute]
int main(array<System::String ^> ^args)
{
	// �R���g���[�����쐬�����O�ɁAWindows XP �r�W���A�����ʂ�L���ɂ��܂�
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false); 

	{
		TiXmlDocument schema;
		// �X�L�[�}��ǂ�
		if (!schema.LoadFile("sink.xsd")){
			MessageBox::Show("file read error (sink.xsd)");
			exit(0);
		}
		mikuEnum.ReadSchema(schema.RootElement());
	}

	// ���C�� �E�B���h�E���쐬���āA���s���܂�
	Application::Run(gcnew Form1(mikuEnum));
	return 0;
}
