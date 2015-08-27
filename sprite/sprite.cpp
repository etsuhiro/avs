// sprite.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "sprite.h"
#include "DX11/FrameworkDX11.h"
#include "Windows/FileDialog.h"

#include <list>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#pragma comment(lib, "DirectXTK.lib")

class MyFramework : public pao::FrameworkDX11 {
	static const int MAX_LOADSTRING = 100;
	TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
	TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
	HACCEL hAccelTable;
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;
	std::list<ID3D11ShaderResourceView*> textures;

public:
	MyFramework(HINSTANCE hInstance) : pao::FrameworkDX11(hInstance)
	{
		LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadString(hInstance, IDC_SPRITE, szWindowClass, MAX_LOADSTRING);

		SetIcon(LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPRITE)), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		SetMenu(MAKEINTRESOURCE(IDC_SPRITE));
		SetClassName(szWindowClass);

		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPRITE));
	}
	~MyFramework()
	{
	}

	int Execute(int nCmdShow)
	{
		return FrameworkWindows::Execute(szTitle, nCmdShow);
	}

	virtual BOOL Setup(HWND hWnd) override
	{
		ID3D11Device* hpDevice = dx11.Device();
		ID3D11DeviceContext* hpDeviceContext = dx11.DeviceContext();

		spriteBatch = std::unique_ptr<DirectX::SpriteBatch>(new DirectX::SpriteBatch(hpDeviceContext));

		return TRUE;
	}

	virtual void Render(ID3D11DeviceContext* pDeviceContext) override
	{
#if 1
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
		spriteBatch->Begin();
		for (auto& x : textures){
			spriteBatch->Draw(x, DirectX::XMFLOAT2(0, 0), nullptr, DirectX::Colors::White);
		}
		spriteBatch->End();
	}

	// �A�v���P�[�V���� ���j���[�̏���
	virtual LRESULT WmCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) override
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
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
#if 1
			pao::FileDialog fileDialog(hWnd);

			if (fileDialog.DialogBoxOpen() == TRUE){
				ID3D11ShaderResourceView* texture;

				if (FAILED(DirectX::CreateWICTextureFromFile(dx11.Device(), fileDialog.GetFullPath(), NULL, &texture))){
					MessageBox(hWnd, _T("CreateWICTextureFromFile"), _T("Err"), MB_ICONSTOP);
					return FALSE;
				}
				textures.push_back(texture);
			}
#else
			OPENFILENAME ofn;
			TCHAR fname_full[MAX_PATH] = L"";   // �t�@�C����(�t���p�X)���󂯎��̈�
			// �\���̂ɏ����Z�b�g
			ZeroMemory(&ofn, sizeof(ofn));				// �ŏ��Ƀ[���N���A���Ă���
			ofn.lStructSize = sizeof(ofn);				// �\���̂̃T�C�Y
			ofn.hwndOwner = hWnd;						// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
			ofn.lpstrFilter = L"png(*.png)\0*.png\0\0";	// �t�@�C���̎��
			ofn.lpstrFile = fname_full;				// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
			ofn.nMaxFile = MAX_PATH;		// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
			ofn.Flags = OFN_FILEMUSTEXIST;		// �t���O�w��
			ofn.lpstrTitle = L"�t�@�C�����J��";		// �R�����_�C�A���O�̃L���v�V����
			ofn.lpstrDefExt = L"png";					// �f�t�H���g�̃t�@�C���̎��
			// �����t�H���_�̎w��
			//			ofn.lpstrInitialDir = g_SetupData.dataPath;
			// �t�@�C�����J���R�����_�C�A���O���쐬
			if (!GetOpenFileName(&ofn)) return false;
			if (FAILED(DirectX::CreateWICTextureFromFile(dx11.Device(), fname_full, NULL, &hpShaderResourceView))){
				MessageBox(hWnd, _T("CreateWICTextureFromFile"), _T("Err"), MB_ICONSTOP);
				return FALSE;
			}
#endif
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
	return myFrameWork.Execute(nCmdShow);
}
