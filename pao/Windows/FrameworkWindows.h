#pragma once

#include "WindowProc.h"

namespace pao
{
	class FrameworkWindows : public pao::IWindowProc {
		static pao::IWindowProc* s_pWindowProc;
		WNDCLASSEX wcex;
		BOOL m_bMenu = FALSE;	// メニューバー表示
		HWND m_hWnd;
	protected:
		//	static FrameworkWindows* s_pFrameworkWindows;	// インスタンスを保持
		int m_width = CW_USEDEFAULT;
		int m_height;
		DWORD m_style = WS_OVERLAPPEDWINDOW;
		DWORD m_exstyle = 0;

	public:
		FrameworkWindows(HINSTANCE hInstance);
		~FrameworkWindows();
		int Execute(LPCTSTR windowName, int nCmdShow);
		static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		void SetClassName(LPCTSTR name);
		void SetIcon(HICON icon, HICON iconSm);
		void SetCursor(HCURSOR cursor);
		void SetMenu(LPCTSTR menu);
		HINSTANCE GetAppInstanceHandle();

	private:
		BOOL InitInstance(LPCTSTR windowName, int nCmdShow);
		virtual BOOL Init(HWND hWnd);
		virtual int MainLoop();
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	};

	BOOL OpenFileName(HWND hWnd, LPCWSTR filter, LPCWSTR defExt);
	TCHAR* GetFullPath();
}
