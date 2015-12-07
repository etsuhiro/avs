#pragma once

#include "Windows/Component.h"
#include "DX11base.h"

class DX11Component : public Component {
	virtual void Init(HWND hWnd) override;
	virtual void Update() override;
	virtual void Final() override;

	DX11 dx11;
};
