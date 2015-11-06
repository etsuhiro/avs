#include "WindowProc.h"

using namespace pao;

LRESULT IWindowProc::operator()(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg){
	case WM_NCCREATE:
		return OnNCCreate(hWnd, wParam, lParam);

	case WM_CREATE:
		return OnCreate(hWnd, wParam, lParam);

	case WM_COMMAND:
		return OnCommand(hWnd, wParam, lParam);

	case WM_PAINT:
		return OnPaint(hWnd, wParam, lParam);

	case WM_LBUTTONDOWN:
		return OnLButtondown(hWnd, wParam, lParam);

	case WM_MBUTTONDOWN:
		return OnMButtondown(hWnd, wParam, lParam);

	case WM_RBUTTONDOWN:
		return OnRButtondown(hWnd, wParam, lParam);

	case WM_MOUSEMOVE:
		return OnMouseMove(hWnd, wParam, lParam);

	case WM_SIZE:
	{
		int width = LOWORD(lParam);  // ���ʃ��[�h���擾����}�N��
		int height = HIWORD(lParam); // ��ʃ��[�h���擾����}�N��

		// ���b�Z�[�W�ɉ���:
		OnSize(hWnd, (UINT)wParam, width, height);
	}
	break;

	case WM_DESTROY:
		OnDestroy(hWnd);

	case WM_CLOSE:    // �E�B���h�E�������悤�Ƃ����Ƃ��ɑ����Ă���
		return OnClose(hWnd, wParam, lParam);

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT IWindowProc::OnNCCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// WM_NCCREATE�Ɍ����Ă�TRUE��Ԃ��B01��Ԃ���Window�����������j������A�E�B���h�E�n���h����NULL�ƂȂ�B
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
