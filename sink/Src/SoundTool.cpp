/*! @file SoundTool.cpp */
#include <d3d9.h>
#include <d3dx9tex.h>

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

#include "SoundTool.h"
#include "resource.h"
#include "sound/mysound.h"
#include "Setup.h"

extern HWND				hSndWnd;
extern WindowsSetupData	g_SetupData;

SoundTool::SoundTool()
{
}

SoundTool::~SoundTool()
{
}

void SoundTool::Create(HINSTANCE hInst, HWND hWnd)
{
	OPENFILENAME ofn;
	TCHAR fname_full[MAX_PATH] = L"";   // �t�@�C����(�t���p�X)���󂯎��̈�
	TCHAR fname[MAX_PATH];        // �t�@�C�������󂯎��̈�

	// �\���̂ɏ����Z�b�g
	ZeroMemory( &ofn, sizeof(ofn) );				// �ŏ��Ƀ[���N���A���Ă���
	ofn.lStructSize		= sizeof(ofn);				// �\���̂̃T�C�Y
	ofn.hwndOwner		= hWnd;						// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
	ofn.lpstrFilter		= L"ogg(*.ogg)\0*.ogg\0\0";	// �t�@�C���̎��
	ofn.lpstrFile		= fname_full;				// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
	ofn.lpstrFileTitle	= fname;					// �I�����ꂽ�t�@�C�������󂯎��ϐ��̃A�h���X
	ofn.nMaxFile		= MAX_PATH;					// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
	ofn.nMaxFileTitle	= MAX_PATH;					// lpstrFileTitle�Ɏw�肵���ϐ��̃T�C�Y
	ofn.Flags			= OFN_FILEMUSTEXIST;		// �t���O�w��
	ofn.lpstrTitle		= L"�t�@�C�����J��";		// �R�����_�C�A���O�̃L���v�V����
	ofn.lpstrDefExt		= L"ogg";					// �f�t�H���g�̃t�@�C���̎��
	// �����t�H���_�̎w��
	ofn.lpstrInitialDir = g_SetupData.dataPath;
	// �t�@�C�����J���R�����_�C�A���O���쐬
	if(!GetOpenFileName(&ofn)) return;
//	sound::SoundPlay(fname_full);

	char path[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, fname_full, -1, path, MAX_PATH, NULL, NULL);
	sound::Bgm.Req(path);
#if 0
	// ���ɕ\������Ă��邩�H
	if (IsWindow(hSndWnd) == 0){
		// ����Ă��Ȃ��Ȃ�_�C�A���O�쐬
		hSndWnd = CreateDialog(hInst, L"SoundTool", hWnd, (DLGPROC)Proc);
	}
#endif
}

// �v���V�[�W��
LRESULT CALLBACK SoundTool::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		// Pan�l�X���C�_�[������
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(-100, 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			hSndWnd = NULL;
			EndDialog(hDlg, IDCANCEL);
			DestroyWindow(hDlg);
			hDlg = NULL;
			return FALSE;
		case IDC_BUTTON1: // �u�Đ��v���u�ꎞ��~�v�{�^��
			return FALSE;
		case IDC_BUTTON2: // �u��~�v�{�^��
			return FALSE;
		}
		return FALSE;
	}

	return FALSE;
}

// ScriptTool�X�V
void SoundTool::Update(HWND hDlg)
{
#if 0
	if (g_ScriptData.CheckLoad()){
		int		buf_size = strlen(g_ScriptData.MultiBuffer()) + 1;
		char*	tmp_buff = new char[buf_size];
		char*	pb = tmp_buff, *pl = tmp_buff;
		TIXML_STRING	xml_str;

		memset( tmp_buff, 0x0, sizeof(char)*buf_size );
		xml_str.reserve( buf_size*2 );
		/* ���s�R�[�h�� LF �� CR+LF */
		strcpy_s( tmp_buff, buf_size*sizeof(char), g_ScriptData.MultiBuffer() );
		for	( int i = 0; i < (int)buf_size; i++, pb++){
			if	( !(*pb)){ break; }
#if	0
			if	( *pb == 0x0a){
				xml_str.append( pl, pb - pl );
				xml_str += 0x0d;	// '\r';	// CR
				xml_str += 0x0a;	// '\n';	// LF
				pl = pb;
			}
#endif
		}
		xml_str.append( pl, pb - pl );

		delete[]	tmp_buff;

		// �G�f�B�b�g�R���g���[���ɓǂݍ��񂾃e�L�X�g�̕\��
		size_t	buf_size2 = xml_str.size() + 1;
		TCHAR*	str_t = new TCHAR [buf_size2];
		memset(str_t, 0x0, sizeof(TCHAR)*(buf_size2));
		MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, xml_str.c_str(), -1, str_t, buf_size2);
		SetDlgItemText(hDlg, IDC_EDIT1, str_t);
		g_ScriptData.BreakLoadFlg();
		delete [] str_t;

		// ScriptTool�̕\�����X�V
		InvalidateRect(hDlg, NULL, TRUE);
	}
#endif
}
