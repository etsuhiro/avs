#pragma once

#include "WindowProc.h"
#include "Component.h"
#include <list>

namespace pao
{
	class FrameworkWindows : public pao::IWindowProc {
		WNDCLASSEX wcex;
		BOOL m_bMenu = FALSE;	// メニューバー表示
		std::list<Component*> m_components;
	protected:
		int m_width = CW_USEDEFAULT;
		int m_height;
		DWORD m_style = WS_OVERLAPPEDWINDOW;
		DWORD m_exstyle = 0;

	public:
		FrameworkWindows(HINSTANCE hInstance);
		~FrameworkWindows();
		HWND Create(HINSTANCE hInstance, LPCTSTR className, LPCTSTR windowName);
		int Run();
		static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

	protected:
		void SetClassName(LPCTSTR name);
		void SetIcon(HICON icon, HICON iconSm);
		void SetCursor(HCURSOR cursor);
		void SetMenu(LPCTSTR menu);
		HINSTANCE GetAppInstanceHandle();

	private:
		HWND InitInstance(LPCTSTR windowName);
		virtual int MainLoop();
		virtual void MakeWindow(WNDCLASSEX& wcex);
	};
}
