#include "FrameworkWindows.h"

using namespace pao;

FrameworkWindows::FrameworkWindows(HINSTANCE hInstance)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	//		wcex.style = CS_CLASSDC;	// DirectXアプリはこれ
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("pao_Framework");
	wcex.hIconSm = NULL;
}

FrameworkWindows::~FrameworkWindows()
{
}

void FrameworkWindows::Create(HINSTANCE hInstance)
{
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	//		wcex.style = CS_CLASSDC;	// DirectXアプリはこれ
	wcex.lpfnWndProc = WindowProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = NULL;
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = TEXT("pao_Framework");
	wcex.hIconSm = NULL;
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
	// ウィンドウクラス登録
	RegisterClassEx(&wcex);

	// アプリケーションの初期化を実行します:
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

	pao::IWindowProc* pWindowProc = this;
	HWND hWnd = CreateWindowEx(
		m_exstyle,
		wcex.lpszClassName,
		windowName,
		m_style,
		CW_USEDEFAULT, 0,
		m_width, m_height,
		NULL, NULL,
		wcex.hInstance, pWindowProc
		);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

int FrameworkWindows::MainLoop()
{
	MSG msg;

	// メイン メッセージ ループ:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

// バージョン情報ボックスのメッセージ ハンドラーです。
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
