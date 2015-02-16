#include "DxLib.h"

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
	SetDrawScreen(DX_SCREEN_BACK);

	// �a�l�o�摜�̕\��
	//	LoadGraphScreen( 0 , 0 , "test1.bmp" , TRUE ) ;
	int bg = LoadGraph("bg_18_1.png");
	int GHandle = LoadGraph("st_sor_s_916.png");

	int x = 0;
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
					}
				}
			}
		}

		DrawGraph(0, 0, bg, TRUE);
		x += 1;
		x %= 640;
		DrawGraph(x, -40, GHandle, TRUE);

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
