#include "DialogProc.h"
#include <map>

using namespace pao;

std::map<HWND, IDialogProc *> s_wndmap;

LRESULT CALLBACK IDialogProc::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IDialogProc *pThis = NULL;

	if (uMsg == WM_INITDIALOG)
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (IDialogProc*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

		pThis->m_hwnd = hwnd;
	}
	else
	{
		pThis = (IDialogProc*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}
	if (pThis)
	{
//		return pThis->HandleMessage(uMsg, wParam, lParam);
	}
	return FALSE;
}
