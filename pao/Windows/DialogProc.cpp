#include "DialogProc.h"

using namespace pao;

LRESULT CALLBACK IDialogProc::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IDialogProc *pThis = NULL;

	if (uMsg == WM_NCCREATE)
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
