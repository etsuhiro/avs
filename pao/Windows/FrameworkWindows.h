#pragma once

#include "WindowProc.h"

namespace pao
{
	class FrameworkWindows : public pao::IWindowProc {
		WNDCLASSEX wcex;
		BOOL m_bMenu = FALSE;	// メニューバー表示
	protected:
		int m_width = CW_USEDEFAULT;
		int m_height;
		DWORD m_style = WS_OVERLAPPEDWINDOW;
		DWORD m_exstyle = 0;

	public:
		FrameworkWindows(HINSTANCE hInstance);
		~FrameworkWindows();
		void Create(HINSTANCE hInstance);
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
		virtual int MainLoop();
	};
}
