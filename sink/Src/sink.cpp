/*! @file sink.cpp */

#if	0
/* Warnig �����p ��{�I�ɂ� if 0 �� */
#pragma warning( disable : 4995 ) // disable deprecated warning 
#endif

#include <d3d9.h>
#include <d3dx9tex.h>

#if 0
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 
#endif

#include "Framework.h"
#include "resource.h"
#include "tinyxml.h"

#include "avs/ImageCanvas.h"
#include "image/ImageManager.h"

#include "avs/TextCanvas.h"
#include "WinTextCanvas.h"

#include "Setup.h"

#include "TextWrite.h"
#include "ScriptTool.h"
#include "LayerTool.h"
#include "SoundTool.h"
#include "sound/mysound.h"
#include "movie/mymovie.h"

WindowsSetupData	g_SetupData;
ScriptTool			g_ScriptTool;
SoundTool			g_SoundTool;
HWND	hScpWnd,hSndWnd;

class myFramework : public pao::Framework
{
	bool Setup( HWND hWnd );
	void Update();
	void Draw();
	virtual LRESULT WmCommand( HWND hWnd, WPARAM wParam, LPARAM lParam );
	virtual LRESULT WmRButtondown( HWND hWnd, WPARAM wParam, LPARAM lParam );
	virtual LRESULT WmMouseMove( HWND hWnd, WPARAM wParam, LPARAM lParam );

	IDirect3DTexture9*	pSrcTexture;
	int					prevTime;

//	avs::ImageTag*			pTag;
//	avs::ImageCanvas*		pCanvas;
	image::ImageManager*	pManager;

//	avs::TextCanvas*		pTextCanvas;
//	WinTextCanvas*			pWinTextCanvas;

	bool	MovieIsPlay(){ return movie::MovieIsPlay(); }
	TCHAR buffer[4096];

public:
	myFramework(HINSTANCE hInst)
	 : Framework(hInst)
	 , pSrcTexture(NULL)
//	 , pTag(NULL)
	{
		SetMenu( MAKEINTRESOURCE(IDC_SINK) );	// ���j���[���\�������悤�ɂ��܂�

		prevTime = (int)timeGetTime();
		memset(buffer, 0x0, sizeof(buffer));
	}

	void	NewCanvas( const char* path )
	{
//		if	( pTag){ delete[]	pTag; }
//		pTag = new avs::ImageTag[g_SetupData.nLayer];

//		pCanvas = new avs::ImageCanvas( NULL, g_SetupData.nLayer );
		pManager = new image::ImageManager( pd3dDevice, g_SetupData.nLayer, path );
	}
};


void CALLBACK TimerProc( HWND hWnd, UINT Msg, UINT nIDEvent, DWORD dwTime )
{
	sound::SoundUpdate();
	movie::MovieUpdate();
}

//-----------------------------------------------------------------------------
// Name: wWinMain()
// Desc: The application's entry point
//-----------------------------------------------------------------------------
INT WINAPI wWinMain( HINSTANCE hInst, HINSTANCE, LPWSTR, INT )
{
	g_SetupData.Init();
	g_SetupData.Load();
	myFramework framework(hInst);

	framework.MainLoop(L"�^�g", g_SetupData.width, g_SetupData.height);
	return 0;
}

bool myFramework::Setup( HWND  hWnd )
{
	NewCanvas( g_SetupData.imagePath );
	if (!LoadFont(g_SetupData.fontData, -g_SetupData.fontSize))
		return false;
	if	( !sound::SoundInit( hWnd )){ return	false; }		// DirectSound
	if	( !movie::MovieInit( hWnd )){ return	false; }		// DirectShow

	SetTimer( hWnd, 16, 32, TimerProc );

	return true;
}

LRESULT myFramework::WmCommand( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	int wmId    = LOWORD(wParam);
	int wmEvent = HIWORD(wParam);

	switch( wmId ){
	case IDM_LAYERTOOL: // ���C���[�c�[��
		LayerTool::Create(wcex.hInstance, hWnd);
		break;

	case IDM_LOADSCRIPT: // �X�N���v�g�ǂݍ���
		g_ScriptTool.Create(wcex.hInstance, hWnd);
		break;

	case IDM_LOADSOUND: // ���y�t�@�C���ǂݍ���
		g_SoundTool.Create(wcex.hInstance, hWnd);
		break;

	case IDM_LOADMOVIE: // ����t�@�C���ǂݍ���
#if 1
		{	
			OPENFILENAME ofn;
			TCHAR fname_full[MAX_PATH] = L"";   // �t�@�C����(�t���p�X)���󂯎��̈�
			// �\���̂ɏ����Z�b�g
			ZeroMemory( &ofn, sizeof(ofn) );				// �ŏ��Ƀ[���N���A���Ă���
			ofn.lStructSize		= sizeof(ofn);				// �\���̂̃T�C�Y
			ofn.hwndOwner		= hWnd;						// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
			ofn.lpstrFilter		= L"mpg(*.mpg)\0*.mpg\0\0";	// �t�@�C���̎��
			ofn.lpstrFile		= fname_full;				// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
			ofn.nMaxFile		= MAX_PATH;		// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
			ofn.Flags			= OFN_FILEMUSTEXIST;		// �t���O�w��
			ofn.lpstrTitle		= L"�t�@�C�����J��";		// �R�����_�C�A���O�̃L���v�V����
			ofn.lpstrDefExt		= L"mpg";					// �f�t�H���g�̃t�@�C���̎��
			// �����t�H���_�̎w��
			ofn.lpstrInitialDir = g_SetupData.dataPath;
			// �t�@�C�����J���R�����_�C�A���O���쐬
			if(!GetOpenFileName(&ofn)) return false;

			if (!movie::MovieInit(hWnd)){
				MessageBox( NULL, L"�������Ɏ��s���܂����B", L"LoadMovie", MB_OK|MB_ICONSTOP);
				return	false;
			}
			movie::MoviePlay(fname_full);
		}
#endif
		break;

	case IDM_ABOUT:
		DialogBox(wcex.hInstance, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;

	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;

	default:
	    return DefWindowProc( hWnd, WM_COMMAND, wParam, lParam );
	}
	return 0;
}

void myFramework::Update()
{
	if ( NULL == pd3dDevice )
		return;

	RECT SrcRect;
	SetRect( &SrcRect, 0, 0, g_SetupData.width, g_SetupData.height );
	if	( pBG){
		pd3dDevice->StretchRect( pBG, &SrcRect, pBack,
								 NULL, D3DTEXF_NONE );
	} else
	{
		pd3dDevice->Clear( 0, NULL, D3DCLEAR_TARGET, 0xff000000, 0, 0 );
	}
	int time = prevTime;
	prevTime = (int)timeGetTime();
	time = prevTime - time;


	/* ���C���[�P���� */
	pManager->Update();

#if 0
	pTextCanvas->Update();
#endif

	// LayerTool�X�V
	LayerTool::Update();
	// ScriptTool�X�V
	if (hScpWnd != 0)
		g_ScriptTool.Update(hScpWnd);
}

void myFramework::Draw()
{
//	if (pSprite->Begin( D3DXSPRITE_ALPHABLEND ) == D3D_OK){
	if (pSprite->Begin( D3DXSPRITE_ALPHABLEND|D3DXSPRITE_OBJECTSPACE ) == D3D_OK){
		D3DXMATRIX	mtx;
//		D3DXVECTOR3 pos(x-340,320,0);
//		D3DXVECTOR3 center(340,280,0);
		float		camlen = 500.f;

		D3DXMatrixIdentity( &mtx );		// �P�ʍs��
		pSprite->SetTransform( &mtx );
//		pSprite->Draw( pSrcTexture, NULL, &center, &pos, 0xffffffff);

		{	/* �ˉe�}�g���N�X */
			D3DXMATRIX		proj_mtx;
			D3DVIEWPORT9	view_port;

			D3DXMatrixIdentity(&proj_mtx);  // �P�ʍs��

			if	( g_SetupData.fovy <= 0 || g_SetupData.fovy > 179 || FAILED( pd3dDevice->GetViewport( &view_port ))){
				/* ���ˉe */
				D3DXMatrixOrthoLH( &proj_mtx, (float)g_SetupData.width, (float)g_SetupData.height, 1, 65535 );
			} else
			{
				/* �����ˉe */
				float	aspect = (float)view_port.Width/(float)view_port.Height;

				// �ˉe�}�g���b�N�X���쐬

				D3DXMatrixPerspectiveFovLH(&proj_mtx,
											D3DXToRadian((float)g_SetupData.fovy),	// �J�����̉�p(����p)
											aspect,				// �A�X�y�N�g��
											1.0f,				// near�v���[��
											65535.0f);			// far�v���[��

				camlen = (float)view_port.Height/(tan(D3DXToRadian((float)g_SetupData.fovy/2.f))*2);
			}
			pd3dDevice->SetTransform( D3DTS_PROJECTION, &proj_mtx );
		}
		{	/* �r���[�}�g���N�X */
			D3DXMATRIX		view_mtx;

			D3DXVECTOR3		vec_from = D3DXVECTOR3(0, 0, camlen);	// �J�����̈ʒu
			D3DXVECTOR3		vec_lookat = D3DXVECTOR3(0, 0, 0);	// �J�����̒����_
			D3DXVECTOR3		vec_up = D3DXVECTOR3(0, -1, 0);		// �J�����̃x�N�g��

			D3DXMatrixIdentity( &view_mtx );  // �P�ʍs����쐬
			D3DXMatrixLookAtLH( &view_mtx, &vec_from, &vec_lookat, &vec_up );

			pd3dDevice->SetTransform( D3DTS_VIEW, &view_mtx );
		}
//		pd3dDevice->SetRenderState( D3DRS_MULTISAMPLEANTIALIAS, TRUE );
		pd3dDevice->SetRenderState( D3DRS_ANTIALIASEDLINEENABLE, TRUE );

		pd3dDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
		pd3dDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
//		pd3dDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		pManager->Draw( pSprite );

		pSprite->End();
	}
#if 0
	TextWrite(pFont,
			  g_SetupData.window.pos.x+g_SetupData.window.text.x,
			  g_SetupData.window.pos.y+g_SetupData.window.text.y,
			  L"�t�H���g�̕\�����ł����");
	RECT rc;
	POINT po;
	GetCursorPos(&po);
	wchar_t buf[128];
//	wsprintf(buf, L"%4d, %4d", po.x, po.y);
	StringCbPrintf(buf, sizeof(buf), L"%4d, %4d", po.x, po.y);
	SetRect( &rc, 0, 576, 0, 0 );
	pFont->DrawText( NULL, buf, -1, &rc, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 0.0f, 1.0f ));
#endif
#if 0
	pTextCanvas->Draw();
#endif
}

// ���C���[���}�E�X�œ�����
LRESULT myFramework::WmRButtondown( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	LayerTool::ImageMove(WM_RBUTTONDOWN, wParam, lParam);
	return 0;
}
LRESULT myFramework::WmMouseMove( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	LayerTool::ImageMove(WM_MOUSEMOVE, wParam, lParam);
	return 0;
}

