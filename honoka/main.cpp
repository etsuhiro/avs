#include "DxLib.h"
#include "avs.h"
#include <list>
#include <string>
#include <algorithm>

struct Asset {
	std::string name;
	int handle;
};

class AssetManager {
	std::list<Asset> m_assets;

public:
	~AssetManager(){
		for (auto& asset : m_assets){
			DeleteGraph(asset.handle);
		}
		m_assets.clear();
	}
	int Load(const char* path){
		auto it = std::find_if(m_assets.begin(), m_assets.end(), [&path](Asset& asset){return asset.name == path; });
		if (it != m_assets.end())
			return it->handle;

		int handle = LoadGraph(path);
		if (handle >= 0){
			Asset asset{ path, handle };
			m_assets.push_back(asset);
		}
		return handle;
	}
	void Remove(int handle){
		auto it = std::find_if(m_assets.begin(), m_assets.end(), [&handle](Asset& asset){return asset.handle == handle; });
		if (it != m_assets.end()){
			DeleteGraph(it->handle);
			m_assets.erase(it);
		}
	}
};

struct Sprite {
	int ghandle;
	int x;
	int y;
};

std::list<Sprite> sprites;
XmlEnum sinkxsd;

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

	AssetManager asset_manager;

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
		}
	}

	while(1){
		// �L�[�̏�Ԃ��`�F�b�N
		{
			char Buf[256];
			GetHitKeyStateAll(Buf);
			if (Buf[KEY_INPUT_LCONTROL] || Buf[KEY_INPUT_RCONTROL])
			{
				if (Buf[KEY_INPUT_O])
				{
					OPENFILENAME ofn;
					TCHAR fname_full[MAX_PATH] = "";   // �t�@�C����(�t���p�X)���󂯎��̈�
					// �\���̂ɏ����Z�b�g
					ZeroMemory(&ofn, sizeof(ofn));				// �ŏ��Ƀ[���N���A���Ă���
					ofn.lStructSize = sizeof(ofn);				// �\���̂̃T�C�Y
					ofn.hwndOwner = NULL;						// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
					ofn.lpstrFilter = "png(*.png)\0*.png\0\0";	// �t�@�C���̎��
					ofn.lpstrFile = fname_full;				// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
					ofn.nMaxFile = MAX_PATH;		// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
					ofn.Flags = OFN_FILEMUSTEXIST;		// �t���O�w��
					ofn.lpstrTitle = "�t�@�C�����J��";		// �R�����_�C�A���O�̃L���v�V����
					ofn.lpstrDefExt = "png";					// �f�t�H���g�̃t�@�C���̎��
					// �t�@�C�����J���R�����_�C�A���O���쐬
					if (GetOpenFileName(&ofn)){
						int handle = asset_manager.Load(fname_full);
						if (handle >= 0){
							Sprite sprite{ handle, 0, 0 };
							sprites.push_back(sprite);
						}
					}
				}
			}
		}
		for (auto& sprite: sprites)
		{
			DrawGraph(sprite.x, sprite.y, sprite.ghandle, TRUE);
		}

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
