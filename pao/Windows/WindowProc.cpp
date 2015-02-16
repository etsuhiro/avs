#include "WindowProc.h"

using namespace pao;

LRESULT IWindowProc::operator()(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg){
	case WM_CREATE:
		return WmCreate(hWnd, wParam, lParam);

	case WM_COMMAND:
		return WmCommand(hWnd, wParam, lParam);

	case WM_PAINT:
		return WmPaint(hWnd, wParam, lParam);

	case WM_LBUTTONDOWN:
		return WmLButtondown(hWnd, wParam, lParam);

	case WM_MBUTTONDOWN:
		return WmMButtondown(hWnd, wParam, lParam);

	case WM_RBUTTONDOWN:
		return WmRButtondown(hWnd, wParam, lParam);

	case WM_MOUSEMOVE:
		return WmMouseMove(hWnd, wParam, lParam);

	case WM_DESTROY:
		return WmDestroy(hWnd, wParam, lParam);

	case WM_CLOSE:    // ウィンドウが閉じられようとしたときに送られてくる
		return WmClose(hWnd, wParam, lParam);

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT IWindowProc::WmCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return 0;
}

LRESULT IWindowProc::WmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
}

LRESULT IWindowProc::WmPaint(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	BeginPaint(hWnd, &ps);
	EndPaint(hWnd, &ps);
	return 0;
}

LRESULT IWindowProc::WmLButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_LBUTTONDOWN, wParam, lParam);
}

LRESULT IWindowProc::WmMButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_MBUTTONDOWN, wParam, lParam);
}

LRESULT IWindowProc::WmRButtondown(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_RBUTTONDOWN, wParam, lParam);
}

LRESULT IWindowProc::WmMouseMove(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_MOUSEMOVE, wParam, lParam);
}

LRESULT IWindowProc::WmDestroy(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	PostQuitMessage(0);
	return 0;
}

LRESULT IWindowProc::WmClose(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return DefWindowProc(hWnd, WM_CLOSE, wParam, lParam);
}
