#pragma once

#include "Windows/FrameworkWindows.h"
#include "DX11base.h"

namespace pao
{
	class FrameworkDX11 : public pao::FrameworkWindows {
	protected:
		DX11 dx11;
	public:
		FrameworkDX11(HINSTANCE hInstance);
		~FrameworkDX11();

	private:
		virtual int MainLoop() override;
		virtual LRESULT OnCreate(HWND hWnd, WPARAM wParam, LPARAM lParam) override;
		virtual void OnSize(HWND hwnd, UINT flag, int width, int height) override;

		virtual BOOL Setup(HWND hWnd);
		virtual void Render(ID3D11DeviceContext*);
	};

}