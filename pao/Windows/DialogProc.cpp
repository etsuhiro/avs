#include "DialogProc.h"
#include <map>
#include <windowsx.h>

using namespace pao;

std::map<HWND, IDialogProc *> s_dialogMap;

HWND IDialogProc::Create(HINSTANCE hInst, UINT nIDResource, HWND hWndParent)
{
	// モードレスダイアログボックスを作成します
	m_hwnd = CreateDialog(hInst, MAKEINTRESOURCE(nIDResource), hWndParent, (DLGPROC)DialogProc);
	s_dialogMap[m_hwnd] = this;
	return m_hwnd;
}

LRESULT CALLBACK IDialogProc::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IDialogProc *pThis = s_dialogMap[hwnd];

	switch (uMsg) {
	case WM_INITDIALOG:
		return pThis->OnInitDialog();

	case WM_DROPFILES:
		return pThis->OnDropFiles();

	case WM_NOTIFY:
		return pThis->OnNotify();

	case WM_MOUSEMOVE:
		return pThis->OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);

	case WM_LBUTTONDOWN:
		return pThis->OnLButtonDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (DWORD)wParam);

	case WM_LBUTTONUP:
		return pThis->OnLButtonUp();

	case WM_CONTEXTMENU:
		return pThis->OnContextMenu(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), (HWND)wParam);

	case WM_RBUTTONUP:
		return pThis->OnRButtonUp();

	case WM_CLOSE:
		return pThis->OnClose();
	}

	return FALSE;
}

LRESULT IDialogProc::OnInitDialog()
{
	return FALSE;
}

LRESULT IDialogProc::OnDropFiles()
{
	return FALSE;
}

LRESULT IDialogProc::OnNotify()
{
	return FALSE;
}

LRESULT IDialogProc::OnMouseMove(int xPos, int yPos, DWORD flags)
{
	return FALSE;
}

LRESULT IDialogProc::OnLButtonDown(int xPos, int yPos, DWORD flags)
{
	return FALSE;
}

LRESULT IDialogProc::OnLButtonUp()
{
	return FALSE;
}

LRESULT IDialogProc::OnContextMenu(int xPos, int yPos, HWND hwnd)
{
	return FALSE;
}

LRESULT IDialogProc::OnRButtonUp()
{
	return FALSE;
}

LRESULT IDialogProc::OnClose()
{
	return FALSE;
}
