#include "stdafx.h"
#include "DX11base.h"

#include <directxmath.h>
#include <wrl.h>

#include <vector>


#include <memory>

#pragma comment( lib, "d3d11.lib" )

using namespace DirectX;
//Template Aliases��VS2012�ł͖��Ή�
template <typename TYPE>
using DXPtr = Microsoft::WRL::ComPtr<TYPE>;

// DX11�I�u�W�F�N�g�|�C���^�Z�k�`
typedef DXPtr<ID3D11Device>			IDevicePtr;
typedef DXPtr<ID3D11DeviceContext>	IDevCtxPtr;
typedef DXPtr<IDXGISwapChain>		ISChainPtr;

typedef DXPtr<ID3D11Buffer>			IBuffPtr;
typedef DXPtr<ID3D11Texture2D>		ITex2DPtr;

typedef DXPtr<ID3D11VertexShader>	IVShaderPtr;
typedef DXPtr<ID3D11GeometryShader>	IGShaderPtr;
typedef DXPtr<ID3D11PixelShader>	IPShaderPtr;
typedef DXPtr<ID3D11InputLayout>	IILayoutPtr;
typedef DXPtr<ID3D11SamplerState>	ISpStatePtr;

typedef DXPtr<ID3D11RasterizerState>	IRsStatePtr;
typedef DXPtr<ID3D11BlendState>			IBdStatePtr;
typedef DXPtr<ID3D11DepthStencilState>	IDsStatePtr;

typedef DXPtr<ID3D11ShaderResourceView>	ISRViewPtr;
typedef DXPtr<ID3D11RenderTargetView>	IRTViewPtr;
typedef DXPtr<ID3D11DepthStencilView>	IDSViewPtr;

typedef DXPtr<ID3DBlob>	IBlobPtr;

struct Context
{
	IDevicePtr pDevice;
	IDevCtxPtr pDevCtx;
	ISChainPtr pSwapChain;
	IRTViewPtr pRTView;
	IDSViewPtr pDSView;

	// C#�̂悤�ȏ������@VS2013����
	bool bInitDX11 = false;//2�d�������h�~
	D3D_DRIVER_TYPE DriverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL FeatureLevel = D3D_FEATURE_LEVEL_11_0;
};

namespace{

	std::unique_ptr<Context> pCtx;

}

DX11::DX11()
{
}

DX11::~DX11()
{
	ExitDX11();
}

ID3D11Device* DX11::Device() const
{
	return pCtx->pDevice.Get();
}

ID3D11DeviceContext* DX11::DeviceContext() const
{
	return pCtx->pDevCtx.Get();
}

//---------------------------------------------------------
HRESULT DX11::InitDX11(HWND hWnd)
{
	HRESULT hr = S_OK;

	RECT rc;
	GetClientRect(hWnd, &rc);
	UINT width = rc.right - rc.left;
	UINT height = rc.bottom - rc.top;

	UINT cdev_flags = 0;
#ifdef _DEBUG
	cdev_flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	pCtx.reset(new Context);

	// DirectX11&�n�[�h�E�F�A�Ή��̂�
	D3D_FEATURE_LEVEL feature_level = D3D_FEATURE_LEVEL_11_0;
	pCtx->DriverType = D3D_DRIVER_TYPE_HARDWARE;

	// �X���b�v�`�F�C���ݒ�
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = width;
	sd.BufferDesc.Height = height;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;	//1/60 = 60fps
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	// DirectX11�f�o�C�X�ƃX���b�v�`�F�C���쐬
	hr = D3D11CreateDeviceAndSwapChain(NULL, pCtx->DriverType, NULL,
		cdev_flags, &feature_level, 1, D3D11_SDK_VERSION, &sd,
		&pCtx->pSwapChain, &pCtx->pDevice, &pCtx->FeatureLevel, &pCtx->pDevCtx);
	if (FAILED(hr)){
		return hr;
	}

	// �X���b�v�`�F�C���ɗp�ӂ��ꂽ�o�b�t�@�i2D�e�N�X�`���j���擾
	ITex2DPtr back_buff;
	hr = pCtx->pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&back_buff);
	if (FAILED(hr)){
		return hr;
	}

	hr = pCtx->pDevice->CreateRenderTargetView(back_buff.Get(), NULL, &pCtx->pRTView);
	if (FAILED(hr)){
		return hr;
	}

	// render-target view��o�^
	ID3D11RenderTargetView* rtv[1] = { pCtx->pRTView.Get() };
	pCtx->pDevCtx->OMSetRenderTargets(1, rtv, NULL);

	// viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)width;
	vp.Height = (FLOAT)height;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pCtx->pDevCtx->RSSetViewports(1, &vp);

	pCtx->bInitDX11 = true;

	return S_OK;
}


//---------------------------------------------------------
void DX11::ExitDX11()
{
	pCtx.reset(nullptr);
}

//---------------------------------------------------------
void DX11::Clear()
{
	if (!pCtx || !pCtx->bInitDX11)return;
	// �w��F�ŉ�ʃN���A
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	pCtx->pDevCtx->ClearRenderTargetView(pCtx->pRTView.Get(), ClearColor);
}

void DX11::Flip()
{
	//���ʂ��E�C���h�E�ɔ��f
	pCtx->pSwapChain->Present(0, 0);
}

