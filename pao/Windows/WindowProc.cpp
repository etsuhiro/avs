#include "WindowProc.h"

using namespace pao;

LRESULT CALLBACK IWindowProc::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IWindowProc *pThis = (IWindowProc*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (uMsg){
	case WM_NCCREATE:
	{
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (IWindowProc*)pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

		return pThis->OnNCCreate(hWnd, wParam, lParam);
	}

	case WM_CREATE:
		return pThis->OnCreate(hWnd, wParam, lParam);

	case WM_COMMAND:
		return pThis->OnCommand(hWnd, wParam, lParam);

	case WM_PAINT:
		return pThis->OnPaint(hWnd, wParam, lParam);

	case WM_LBUTTONDOWN:
		return pThis->OnLButtondown(hWnd, wParam, lParam);

	case WM_MBUTTONDOWN:
		return pThis->OnMButtondown(hWnd, wParam, lParam);

	case WM_RBUTTONDOWN:
		return pThis->OnRButtondown(hWnd, wParam, lParam);

	case WM_MOUSEMOVE:
		return pThis->OnMouseMove(hWnd, wParam, lParam);

	case WM_SIZE:
	{
		int width = LOWORD(lParam);  // ���ʃ��[�h���擾����}�N��
		int height = HIWORD(lParam); // ��ʃ��[�h���擾����}�N��

		// ���b�Z�[�W�ɉ���:
		pThis->OnSize(hWnd, (UINT)wParam, width, height);
	}
	break;

	case WM_DESTROY:
		pThis->OnDestroy(hWnd);
		break;

	case WM_CLOSE:    // �E�B���h�E�������悤�Ƃ����Ƃ��ɑ����Ă���
		return pThis->OnClose(hWnd, wParam, lParam);

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

LRESULT IWindowProc::OnNCCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// WM_NCCREATE�Ɍ����Ă�TRUE��Ԃ��B0��Ԃ���Window�����������j������A�E�B���h�E�n���h����NULL�ƂȂ�B
	return TRUE;
}

LRESULT IWindowProc::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// �ʏ��0��Ԃ��B-1��Ԃ���Window�����������j������A�E�B���h�E�n���h����NULL�ƂȂ�B
	return 0;
}

LRESULT IWindowProc::OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
}

LRESULT IWindowProc::OnPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT IWindowProc::OnLButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_LBUTTONDOWN, wParam, lParam);
}

LRESULT IWindowProc::OnMButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_MBUTTONDOWN, wParam, lParam);
}

LRESULT IWindowProc::OnRButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_RBUTTONDOWN, wParam, lParam);
}

LRESULT IWindowProc::OnMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_MOUSEMOVE, wParam, lParam);
}

void IWindowProc::OnDestroy(HWND)
{
	PostQuitMessage(0);
}

LRESULT IWindowProc::OnClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_CLOSE, wParam, lParam);
}

void IWindowProc::OnSize(HWND hwnd, UINT flag, int width, int height)
{
	// �T�C�Y�ύX������
}
