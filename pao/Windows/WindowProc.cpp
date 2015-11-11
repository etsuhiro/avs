#include "WindowProc.h"

using namespace pao;

// WindowProcのインターフェイス
// MFC使った方がいいんじゃないのとも思ったり思わなかったり

// static関数でエントリ
LRESULT CALLBACK IWindowProc::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	IWindowProc *pThis = (IWindowProc*)GetWindowLongPtr(hWnd, GWLP_USERDATA);

	switch (uMsg){
	case WM_NCCREATE:
	{
		// WM_NCCREATE と WM_CREATE での lParam パラメーターは CREATESTRUCT 構造体へのポインターとなります。
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		// CreateWindowEx で指定したポインターを取り出します。
		pThis = (IWindowProc*)pCreate->lpCreateParams;
		// ウィンドウのインスタンスデータに保存しておきます。
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
		int width = LOWORD(lParam);  // 下位ワードを取得するマクロ
		int height = HIWORD(lParam); // 上位ワードを取得するマクロ

		// メッセージに応答:
		pThis->OnSize(hWnd, (UINT)wParam, width, height);
	}
	break;

	case WM_DESTROY:
		pThis->OnDestroy(hWnd);
		break;

	case WM_CLOSE:    // ウィンドウが閉じられようとしたときに送られてくる
		return pThis->OnClose(hWnd, wParam, lParam);

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}

LRESULT IWindowProc::OnNCCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// WM_NCCREATEに限ってはTRUEを返す。0を返すとWindow生成処理が破棄され、ウィンドウハンドルがNULLとなる。
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
	DestroyWindow(hWnd);
	return 0;
//	return DefWindowProc(hWnd, WM_CLOSE, wParam, lParam);
}

void IWindowProc::OnSize(HWND hwnd, UINT flag, int width, int height)
{
	// サイズ変更を処理
}
