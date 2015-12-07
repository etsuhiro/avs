#include "DX11Component.h"
#include "DX11base.h"

void DX11Component::Init(HWND hWnd)
{
	dx11.InitDX11(hWnd);
}

void DX11Component::Update()
{
}

void DX11Component::Final()
{
	dx11.ExitDX11();
}
