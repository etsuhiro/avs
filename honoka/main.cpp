//
// DxLib���g�p����avs�̎��s��
//
#include "DxLib.h"
#include "avs.h"
#include "DxLibAvsImage.h"
#include <list>
#include <string>

XmlEnum sinkxsd;

class DxLibMikuPrint : public IMikuPrint {
	virtual void TagName(const char* name, int indent) override
	{
		for (int i = 0; i<indent; i++)
			printfDx("  ");
		printfDx("<%s", name);
	}

	virtual void TagClose(const char* name, int indent) override
	{
		printfDx(">\n");
	}

	virtual void EmptyElementTagClose(const char* name, int indent) override
	{
		printfDx("/>\n");
	}

	virtual void EndTag(const char* name, int indent) override
	{
		for (int i = 0; i<indent; i++)
			printfDx("  ");
		printfDx("</%s>\n", name);
	}

	virtual void AttrName(const char* name) override
	{
		printfDx(" %s=???", name);
	}

	virtual void Text(const char* text, int indent) override
	{
		for (int i = 0; i<indent; i++)
			printfDx("  ");
		printfDx("%s\n", text);
	}
};

void LoadScript(std::vector<char>& scriptbuf, const char* path, XmlEnum& xmls)
{
	TiXmlDocument xml;
	// �X�N���v�g��ǂ�
	if (!xml.LoadFile(path)){
		printf("file read error %s\n", path);
		exit(0);
	}

	// �o�C�i���ɕϊ�
	hashmap_t hashmap;
	XmlBin().Conv(scriptbuf, hashmap, xml, &xmls);

	mikuPrint(&scriptbuf[0], xmls, DxLibMikuPrint());
	ScreenFlip();
}

class MyScriptEngine : public avs::ScriptEngine
{
};

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("��T��");
	SetOutApplicationLogValidFlag(FALSE);	// ���O�o�͂��s��Ȃ�
	//	SetGraphMode(800, 600, 32);	// �E�B���h�E�̃T�C�Y
	ChangeWindowMode(TRUE);	// �E�B���h�E���[�h�ŋN��������
	if (DxLib_Init() == -1)	// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}
	SetDrawScreen(DX_SCREEN_BACK);	// ����ʕ`�惂�[�h

	{
		TiXmlDocument schema;
		// �X�L�[�}��ǂ�
		if (!schema.LoadFile("sink.xsd")){
			printf("file read error (sink.xsd)\n");
			exit(0);
		}
		//	XmlEnum xmls;
		sinkxsd.ReadSchema(schema.RootElement());
	}

	MyScriptEngine script;
	DxLibAvsImage avsImage;
	script.AddImageListener(&avsImage);

	std::vector<char> scriptBuf;
	{
		OPENFILENAME ofn;
		TCHAR fname_full[MAX_PATH] = "";   // �t�@�C����(�t���p�X)���󂯎��̈�
		// �\���̂ɏ����Z�b�g
		ZeroMemory(&ofn, sizeof(ofn));				// �ŏ��Ƀ[���N���A���Ă���
		ofn.lStructSize = sizeof(ofn);				// �\���̂̃T�C�Y
		ofn.hwndOwner = NULL;						// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
		ofn.lpstrFilter = "script(*.xml)\0*.xml\0\0";	// �t�@�C���̎��
		ofn.lpstrFile = fname_full;				// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
		ofn.nMaxFile = MAX_PATH;		// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
		ofn.Flags = OFN_FILEMUSTEXIST;		// �t���O�w��
		ofn.lpstrTitle = "�t�@�C�����J��";		// �R�����_�C�A���O�̃L���v�V����
		ofn.lpstrDefExt = "xml";					// �f�t�H���g�̃t�@�C���̎��
		// �t�@�C�����J���R�����_�C�A���O���쐬
		if (GetOpenFileName(&ofn)){
			LoadScript(scriptBuf, fname_full, sinkxsd);
			script.SetScript(&scriptBuf[0]);
		}
	}

	int gametime = GetNowCount();
	while (1){
		// �L�[�̏�Ԃ��`�F�b�N
		{
			char Buf[256];
			GetHitKeyStateAll(Buf);
			if (Buf[KEY_INPUT_LCONTROL] || Buf[KEY_INPUT_RCONTROL])
			{
			}
		}

		int nowtime = GetNowCount();
		int elapsedtime = nowtime - gametime;
		gametime = nowtime;
		avs::RunningStatus stat = script.Run(elapsedtime);
		if (stat == avs::FINISH)
			break;

		avsImage.Update(elapsedtime);
		avsImage.Draw();

		// ���b�Z�[�W���[�v�ɑ��鏈��������
		if (ProcessMessage() == -1)
		{
			break;        // �G���[���N�����烋�[�v�𔲂���

		}
		ScreenFlip();
		ClearDrawScreen();
	}

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
