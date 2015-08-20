#include "FrameworkWindows.h"

using namespace pao;

//FrameworkWindows* FrameworkWindows::s_pFrameworkWindows;	// インスタンスを保持
pao::IWindowProc* FrameworkWindows::s_pWindowProc;	// インスタンスを保持

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

	// メイン メッセージ ループ:
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

TCHAR fname_full[MAX_PATH] = L"";   // ファイル名(フルパス)を受け取る領域

BOOL pao::OpenFileName(HWND hWnd, LPCWSTR filter, LPCWSTR defExt)
{
	OPENFILENAME ofn;
	// 構造体に情報をセット
	ZeroMemory(&ofn, sizeof(ofn));				// 最初にゼロクリアしておく
	ofn.lStructSize = sizeof(ofn);				// 構造体のサイズ
	ofn.hwndOwner = hWnd;						// コモンダイアログの親ウィンドウハンドル
	ofn.lpstrFilter = L"png(*.png)\0*.png\0\0";	// ファイルの種類
	ofn.lpstrFile = fname_full;				// 選択されたファイル名(フルパス)を受け取る変数のアドレス
	ofn.nMaxFile = MAX_PATH;		// lpstrFileに指定した変数のサイズ
	ofn.Flags = OFN_FILEMUSTEXIST;		// フラグ指定
	ofn.lpstrTitle = L"ファイルを開く";		// コモンダイアログのキャプション
	ofn.lpstrDefExt = L"png";					// デフォルトのファイルの種類
	// 初期フォルダの指定
	//			ofn.lpstrInitialDir = g_SetupData.dataPath;
	// ファイルを開くコモンダイアログを作成
	return GetOpenFileName(&ofn);
}

TCHAR* pao::GetFullPath()
{
	return fname_full;
}
