#pragma once

#include "windows.h"

namespace pao
{
	class IDialogProc {
		HWND m_hwnd;

	public:
		HWND IDialogProc::Create(HINSTANCE hInst, UINT nIDResource, HWND hWndParent);
	private:
		static LRESULT CALLBACK DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

		virtual LRESULT OnInitDialog();
		virtual LRESULT OnDropFiles();
		virtual LRESULT OnNotify();
		virtual LRESULT OnMouseMove(int xPos, int yPos, DWORD flags);
		virtual LRESULT OnLButtonDown(int xPos, int yPos, DWORD flags);
		virtual LRESULT OnLButtonUp();
		virtual LRESULT OnContextMenu(int xPos, int yPos, HWND hwnd);
		virtual LRESULT OnRButtonUp();
		virtual LRESULT OnClose();
	};
}
