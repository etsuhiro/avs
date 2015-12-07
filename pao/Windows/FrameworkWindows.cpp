#include "FrameworkWindows.h"

using namespace pao;

namespace {
	HINSTANCE s_hInstance;
}

FrameworkWindows::FrameworkWindows(HINSTANCE hInstance)
{
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

HWND FrameworkWindows::Create(HINSTANCE hInstance, LPCTSTR className, LPCTSTR windowName)
{
//	WNDCLASSEX wcex{};

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
	wcex.lpszClassName = className;
	wcex.hIconSm = NULL;

	MakeWindow(wcex);

	// ウィンドウクラス登録
	RegisterClassEx(&wcex);

	// アプリケーションの初期化を実行します:
	HWND hwnd = InitInstance(windowName);

	for (auto& x : m_components)
	{
		x->Init(hwnd);
	}

	return hwnd;
}

void FrameworkWindows::MakeWindow(WNDCLASSEX& wcex)
{
}

int FrameworkWindows::Run()
{
	return MainLoop();
}

HWND FrameworkWindows::InitInstance(LPCTSTR windowName)
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

	return hWnd;
}

int FrameworkWindows::MainLoop()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			for (auto& x : m_components)
			{
				x->Update();
			}
		}
	}

	for (auto& x : m_components)
	{
		x->Final();
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
