// sprite.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "sprite.h"
#include "DX11/FrameworkDX11.h"
#include "Windows/FileDialog.h"

#include <list>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#pragma comment(lib, "DirectXTK.lib")

void InitializeMenuItem(HMENU hmenu, LPTSTR lpszItemName, int nId, HMENU hmenuSub);

class MyFramework : public pao::FrameworkDX11 {
	static const int MAX_LOADSTRING = 100;
	HACCEL hAccelTable;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::list<ID3D11ShaderResourceView*> textures;

public:
	MyFramework(HINSTANCE hInstance) : pao::FrameworkDX11(hInstance)
	{
		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPRITE));
	}
	~MyFramework()
	{
	}

	void Init(HINSTANCE hInstance, int nCmdShow)
	{
		TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
		LoadString(GetAppInstanceHandle(), IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
		LoadString(hInstance, IDC_SPRITE, szWindowClass, MAX_LOADSTRING);
		HWND hWnd = Create(hInstance, szWindowClass, szTitle);
		ShowWindow(hWnd, nCmdShow);
//		UpdateWindow(hWnd);
	}

private:
	virtual void MakeWindow(WNDCLASSEX& wcex) override
	{
		HINSTANCE hInstance = GetAppInstanceHandle();

		SetIcon(LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPRITE)), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		SetMenu(MAKEINTRESOURCE(IDC_SPRITE));
	}

	virtual BOOL Setup(HWND hWnd) override
	{
		ID3D11DeviceContext* hpDeviceContext = dx11.DeviceContext();
		spriteBatch = std::unique_ptr<DirectX::SpriteBatch>(new DirectX::SpriteBatch(hpDeviceContext));
		return TRUE;
	}

	virtual void Render(ID3D11DeviceContext* pDeviceContext) override
	{
#if 0
		ID3D11BlendState* pBlendState = NULL;
		D3D11_BLEND_DESC BlendDesc;
		ZeroMemory(&BlendDesc, sizeof(BlendDesc));
		BlendDesc.AlphaToCoverageEnable = FALSE;
		BlendDesc.IndependentBlendEnable = FALSE;
		BlendDesc.RenderTarget[0].BlendEnable = TRUE;
		BlendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		BlendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_COLOR;
		BlendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		BlendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		BlendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		BlendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
		dx11.Device()->CreateBlendState(&BlendDesc, &pBlendState);
		pDeviceContext->OMSetBlendState(pBlendState, blendFactor, 0xffffffff);
#endif
		if (spriteBatch){
			spriteBatch->Begin();
			for (auto& x : textures){
				spriteBatch->Draw(x, DirectX::XMFLOAT2(0, 0), nullptr, DirectX::Colors::White);
			}
			spriteBatch->End();
		}
	}

	// アプリケーション メニューの処理
	virtual LRESULT OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) override
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(GetAppInstanceHandle(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_FILE:
		{
			pao::FileDialog fileDialog(hWnd);

			if (fileDialog.DialogBoxOpen() == TRUE){
				ID3D11ShaderResourceView* texture;

				if (FAILED(DirectX::CreateWICTextureFromFile(dx11.Device(), fileDialog.GetFullPath(), NULL, &texture))){
					MessageBox(hWnd, _T("CreateWICTextureFromFile"), _T("Err"), MB_ICONSTOP);
					return FALSE;
				}
				textures.push_back(texture);

				// ドロップダウンメニューにスプライトを追加します。
				// まずはメニューハンドルを得ます
				HMENU hMenu = GetMenu(hWnd);
				// サブメニューは位置を指定して取得します。左から0,1,2で、今回は2
				HMENU hSubMenu = GetSubMenu(hMenu, 2);
				if (hSubMenu == NULL){
					// 空の場合の追加の仕方がわからないので、現時点ではサブメニューにダミーを仕込んでNULLにならないようにしてます
//					hSubMenu = CreatePopupMenu();
//					AppendMenu(hMenu, );
//					InsertMenu(hMenu, 2, MF_BYPOSITION, 0, NULL);
//					InitializeMenuItem(hMenu, fileDialog.GetFileName(), 0, NULL);
				} else
					InitializeMenuItem(hSubMenu, fileDialog.GetFileName(), textures.size(), NULL);
			}
		}
		break;
		default:
			return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
		}
		return 0;
	}
};

int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyFramework myFrameWork(hInstance);
	myFrameWork.Init(hInstance, nCmdShow);
	return myFrameWork.Run();
}

void InitializeMenuItem(HMENU hmenu, LPTSTR lpszItemName, int nId, HMENU hmenuSub)
{
	MENUITEMINFO mii;

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_ID | MIIM_TYPE;
	mii.wID = nId;

	if (lpszItemName != NULL) {
		mii.fType = MFT_STRING;
//		mii.fType = MFT_MENUBREAK;
		mii.dwTypeData = lpszItemName;
	}
	else
		mii.fType = MFT_SEPARATOR;

	if (hmenuSub != NULL) {
		mii.fMask |= MIIM_SUBMENU;
		mii.hSubMenu = hmenuSub;
	}

	InsertMenuItem(hmenu, -1, TRUE, &mii); // -1,TRUEで最下段への追加の意味になる
}
