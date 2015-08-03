//
// DxLib���g�p����avs�̎��s��
//
#include "DxLib.h"
#include "avs.h"
#include "DxLibAvsImage.h"
#include <list>
#include <string>

XmlEnum sinkxsd;

void mikuPrint(char *pBuf, XmlEnum& xmls)
{
	int idx = 0;
	int indent = 0;
	do {
		miku::Node *pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
		if (pNode->isElementType()){
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);

			for (int i = 0; i<indent; i++)
				printfDx("  ");
			printfDx("<%s", xmls.ElementName(pElement->name));
			miku::Attr *pAttr = reinterpret_cast<miku::Attr *>(pElement + 1);
			for (int i = 0; i<pElement->getAttrNum(); i++, pAttr++){
				printfDx(" %s=???", xmls.AttributeName(pAttr->name));
			}
			if (pElement->getChildNode()){
				printfDx(">\n");
				idx = pElement->getChildNode();
				indent++;
				continue;
			}
			else {
				printfDx("/>\n");
			}
		}
		else {
			miku::TextNode *pText = static_cast<miku::TextNode *>(pNode);

			for (int i = 0; i<indent; i++)
				printfDx("  ");
			printfDx("%s\n", &pBuf[pText->entity]);
		}
		while (pNode->isTerminate()){
			// �����Ȃ���΂P�K�w�̃G�������g�ɖ߂�
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
			indent--;
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);
			for (int i = 0; i<indent; i++)
				printfDx("  ");
			printfDx("</%s>\n", xmls.ElementName(pElement->name));

			//			if (indent==0)	break;	// root�܂ŏオ���Ă����̂ŏI���Breturn�ł��ǂ�
			if (idx == 0)	break;	// root�܂ŏオ���Ă����̂ŏI���Breturn�ł��ǂ�
		}
		idx = pNode->getBrotherNode();
	} while (indent);	// �K�w���Ȃ��ꍇ�݂̂����Ŕ�����B
	ScreenFlip();
}

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
	mikuPrint(&scriptbuf[0], xmls);
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
