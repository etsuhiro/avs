#pragma once

//#pragma warning(push)
//#pragma warning(disable : 4005)
#include <d3d11_1.h>
//#pragma warning(pop)

class DX11 {
public:
	DX11();
	~DX11();

	// DirectX11初期化 初期化の失敗を考慮してコンストラクタではなく、関数にした
	// hwnd : 表示するウインドウのハンドル
	HRESULT InitDX11(HWND hwnd);

	// DirectX11終了
	void ExitDX11();

	// レンダリング
	void Clear();
	void Flip();

	ID3D11Device* Device() const;
	ID3D11DeviceContext* DeviceContext() const;
};
