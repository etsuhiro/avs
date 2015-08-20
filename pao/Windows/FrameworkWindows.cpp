#include "FrameworkWindows.h"

using namespace pao;

//FrameworkWindows* FrameworkWindows::s_pFrameworkWindows;	// �C���X�^���X��ێ�
pao::IWindowProc* FrameworkWindows::s_pWindowProc;	// �C���X�^���X��ێ�

FrameworkWindows::FrameworkWindows(HINSTANCE hInstance)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	//		wcex.style = CS_CLASSDC;	// DirectX�A�v���͂���
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = L"pao_Framework";
	wcex.hIconSm = NULL;

	//		s_pFrameworkWindows = this;
	s_pWindowProc = this;
}

FrameworkWindows::~FrameworkWindows()
{
}

void FrameworkWindows::SetClassName(LPCTSTR name)
{
	wcex.lpszClassName = name;
}

void FrameworkWindows::SetIcon(HICON icon, HICON iconSm)
{
	wcex.hIcon = icon;
	wcex.hIconSm = iconSm;
}

void FrameworkWindows::SetCursor(HCURSOR cursor)
{
	wcex.hCursor = cursor;
}

void FrameworkWindows::SetMenu(LPCTSTR menu)
{
	wcex.lpszMenuName = menu;
	m_bMenu = TRUE;
}

HINSTANCE FrameworkWindows::GetAppInstanceHandle()
{
	return wcex.hInstance;
}

int FrameworkWindows::Execute(LPCTSTR windowName, int nCmdShow)
{
	// �E�B���h�E�N���X�o�^
	RegisterClassEx(&wcex);

	// �A�v���P�[�V�����̏����������s���܂�:
	if (!InitInstance(windowName, nCmdShow))
	{
		return FALSE;
	}

	return MainLoop();
}

BOOL FrameworkWindows::InitInstance(LPCTSTR windowName, int nCmdShow)
{
	if (m_width != CW_USEDEFAULT){
		RECT rc;
		SetRect(&rc, 0, 0, m_width, m_height);
		AdjustWindowRect(&rc, m_style, m_bMenu);
		m_width = rc.right - rc.left;
		m_height = rc.bottom - rc.top;
	}

	m_hWnd = CreateWindowEx(
		m_exstyle,
		wcex.lpszClassName,
		windowName,
		m_style,
		CW_USEDEFAULT, 0,
		m_width, m_height,
		NULL, NULL,
		wcex.hInstance, NULL
		);

	if (!m_hWnd)
	{
		return FALSE;
	}

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);

	Init(m_hWnd);

	return TRUE;
}

BOOL FrameworkWindows::Init(HWND hWnd)
{
	return TRUE;
}

int FrameworkWindows::MainLoop()
{
	MSG msg;

	// ���C�� ���b�Z�[�W ���[�v:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

LRESULT CALLBACK FrameworkWindows::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//		return s_pFrameworkWindows->WndProc(hWnd, message, wParam, lParam);
	return (*s_pWindowProc)(hWnd, message, wParam, lParam);
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���[�ł��B
INT_PTR CALLBACK FrameworkWindows::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

TCHAR fname_full[MAX_PATH] = L"";   // �t�@�C����(�t���p�X)���󂯎��̈�

BOOL pao::OpenFileName(HWND hWnd, LPCWSTR filter, LPCWSTR defExt)
{
	OPENFILENAME ofn;
	// �\���̂ɏ����Z�b�g
	ZeroMemory(&ofn, sizeof(ofn));				// �ŏ��Ƀ[���N���A���Ă���
	ofn.lStructSize = sizeof(ofn);				// �\���̂̃T�C�Y
	ofn.hwndOwner = hWnd;						// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
	ofn.lpstrFilter = L"png(*.png)\0*.png\0\0";	// �t�@�C���̎��
	ofn.lpstrFile = fname_full;				// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
	ofn.nMaxFile = MAX_PATH;		// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
	ofn.Flags = OFN_FILEMUSTEXIST;		// �t���O�w��
	ofn.lpstrTitle = L"�t�@�C�����J��";		// �R�����_�C�A���O�̃L���v�V����
	ofn.lpstrDefExt = L"png";					// �f�t�H���g�̃t�@�C���̎��
	// �����t�H���_�̎w��
	//			ofn.lpstrInitialDir = g_SetupData.dataPath;
	// �t�@�C�����J���R�����_�C�A���O���쐬
	return GetOpenFileName(&ofn);
}

TCHAR* pao::GetFullPath()
{
	return fname_full;
}
