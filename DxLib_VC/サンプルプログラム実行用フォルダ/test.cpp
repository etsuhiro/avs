// �����ɃT���v���v���O�������y�[�X�g���Ă�������
// ������Ԃł� LoadGraphScreen �̃T���v���v���O���������͂���Ă��܂��B
#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
//	SetGraphMode(800, 600, 32);	// �E�B���h�E�̃T�C�Y
	ChangeWindowMode(TRUE);	// �E�B���h�E���[�h�ŋN��������
	if( DxLib_Init() == -1 )	// �c�w���C�u��������������
	{
		 return -1;				// �G���[���N�����璼���ɏI��
	}

	// �a�l�o�摜�̕\��
//	LoadGraphScreen( 0 , 0 , "test1.bmp" , TRUE ) ;
	LoadGraphScreen(0, 0, "bg_18_1.png", TRUE);
	LoadGraphScreen(0, -40, "st_sor_s_916.png", TRUE);

	WaitKey() ;					// �L�[�̓��͑҂�((7-3)�wWaitKey�x���g�p)

	DxLib_End() ;				// �c�w���C�u�����g�p�̏I������

	return 0 ;					// �\�t�g�̏I��
}
