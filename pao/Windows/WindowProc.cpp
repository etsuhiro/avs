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
		int width = LOWORD(lParam);  // 下位ワードを取得するマクロ
		int height = HIWORD(lParam); // 上位ワードを取得するマクロ

		// メッセージに応答:
		OnSize(hWnd, (UINT)wParam, width, height);
	}
	break;

	case WM_DESTROY:
		OnDestroy(hWnd);

	case WM_CLOSE:    // ウィンドウが閉じられようとしたときに送られてくる
		return OnClose(hWnd, wParam, lParam);

	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return 0;
}

LRESULT IWindowProc::OnNCCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// WM_NCCREATEに限ってはTRUEを返す。01を返すとWindow生成処理が破棄され、ウィンドウハンドルがNULLとなる。
	return TRUE;
}

LRESULT IWindowProc::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// 通常は0を返す。-1を返すとWindow生成処理が破棄され、ウィンドウハンドルがNULLとなる。
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
	// サイズ変更を処理
}
