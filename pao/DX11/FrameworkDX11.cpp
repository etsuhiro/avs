#include "DX11/FrameworkDX11.h"

using namespace pao;

FrameworkDX11::FrameworkDX11(HINSTANCE hInstance)
	: FrameworkWindows(hInstance)
{
}

FrameworkDX11::~FrameworkDX11()
{
}

BOOL FrameworkDX11::Setup(HWND hWnd)
{
	return TRUE;
}

LRESULT FrameworkDX11::OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// WM_NCCREATEでは早すぎてdx11の初期化ができませんのでWM_CREATEで行います。
	if (dx11.InitDX11(hWnd) == S_OK)
		Setup(hWnd);
	return 0;
}

int FrameworkDX11::MainLoop()
{
	MSG msg = { 0 };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
//				Update();
			dx11.Clear();
			Render(dx11.DeviceContext());
			dx11.Flip();
		}
	}

	dx11.ExitDX11();

	return (int)msg.wParam;
}

// ウィンドウリサイズ中はレンダーに処理が移行しないので、中で描画してみた
void FrameworkDX11::OnSize(HWND hwnd, UINT flag, int width, int height)
{
	dx11.Clear();
	Render(dx11.DeviceContext());
	dx11.Flip();
}

void FrameworkDX11::Render(ID3D11DeviceContext*)
{
}
