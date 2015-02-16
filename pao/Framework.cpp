/*! @file Framework.cpp */
#include "Framework.h"
#include <algorithm>
#include <functional>

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

using namespace pao;


namespace
{
}

Framework* Framework::instance;	// �C���X�^���X��ێ�

bool Framework::Init( HWND hWnd )
{
	// Create the D3D object, which is needed to create the D3DDevice.
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		MessageBox(NULL, L"Dirext3D�̏������Ɏ��s���܂����B", L"InitD3D", MB_OK|MB_ICONSTOP);
		return false;
	}
	d3di.push_back(pD3D);

	// Set up the structure used to create the D3DDevice. Most parameters are
	// zeroed out. We set Windowed to TRUE, since we want to do D3D in a
	// window, and then set the SwapEffect to "discard", which is the most
	// efficient method of presenting the back buffer to the display.  And 
	// we request a back buffer format that matches the current desktop display 
	// format.
	D3DPRESENT_PARAMETERS d3dpp; 
	ZeroMemory( &d3dpp, sizeof(d3dpp) );	// �����l�ł悢���̂̐ݒ���Ȃ�����
	d3dpp.Windowed = TRUE;	// �E�B���h�E���[�h�BFALSE�Ńt���X�N���[�����[�h
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// �f�B�X�v���C�h���C�o�ɂ��C��
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_DONOTWAIT;	// �傫�ȃX�v���C�g�̈ړ����`�����Ȃ�

	// Create the Direct3D device. Here we are using the default adapter (most
	// systems only have one, unless they have multiple graphics hardware cards
	// installed) and requesting the HAL (which is saying we want the hardware
	// device rather than a software one). Software vertex processing is 
	// specified since we know it will work on all cards. On cards that support 
	// hardware vertex processing, though, we would see a big performance gain 
	// by specifying hardware vertex processing.
	if (FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp,
									&pd3dDevice ) ) )
	if (FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT,
									D3DDEVTYPE_HAL,
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&pd3dDevice ) ) )
	if (FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT,
									D3DDEVTYPE_SW,
									hWnd,
									D3DCREATE_HARDWARE_VERTEXPROCESSING,
									&d3dpp,
									&pd3dDevice ) ) )
	if (FAILED( pD3D->CreateDevice( D3DADAPTER_DEFAULT,
									D3DDEVTYPE_SW,
									hWnd,
									D3DCREATE_SOFTWARE_VERTEXPROCESSING,
									&d3dpp,
									&pd3dDevice ) ) )
	{
		MessageBox(NULL, L"CreateDevice�Ɏ��s���܂����B", L"InitD3D", MB_OK|MB_ICONSTOP);
		return false;
	}
	d3di.push_back(pd3dDevice);

    // Device state would normally be set here

	// �o�b�N�o�b�t�@���擾
	if (pd3dDevice->GetBackBuffer(
		0,	// �X���b�v �`�F�[�����w�肷�镄���Ȃ������B
		0,	// �Ԃ��o�b�N �o�b�t�@ �I�u�W�F�N�g�̃C���f�b�N�X�B
			// �o�b�N �o�b�t�@�� 0 ����o�b�N �o�b�t�@�̑��� - 1 �܂Ńi���o�����O�����B
			// 0 �̖߂�l�͍ŏ��̃o�b�N �o�b�t�@�������A�t�����g �o�b�t�@�ł͂Ȃ��B
			// ���̃��\�b�h���g���ăt�����g �o�b�t�@�ɃA�N�Z�X���邱�Ƃ͂ł��Ȃ��B 
		D3DBACKBUFFER_TYPE_MONO,	// MicrosoftR DirectXR 9.0 �ł̓X�e���I �r���[�̓T�|�[�g����Ă��Ȃ��̂ŁA
									// ���̃p�����[�^�̗L���Ȓl�� D3DBACKBUFFER_TYPE_MONO �����ł���B 
		&pBack		// �Ԃ����o�b�N �o�b�t�@ �T�[�t�F�C�X��\�� IDirect3DSurface9 �C���^�[�t�F�C�X�ւ̃|�C���^�̃A�h���X�B
		) != D3D_OK)
	{
		MessageBox(NULL, L"GetBackBuffer�Ɏ��s���܂����B", L"InitD3D", MB_OK|MB_ICONSTOP);
		return false;
	}
	d3di.push_back(pBack);

	// �X�v���C�g
	if (D3DXCreateSprite( pd3dDevice, &pSprite ) != D3D_OK)
	{
		MessageBox(NULL, L"CreateSprite�Ɏ��s���܂����B", L"InitD3D", MB_OK|MB_ICONSTOP);
		return false;
	}
	d3di.push_back(pSprite);

	return true;
}

bool Framework::LoadBG(LPCTSTR pSrcFile, int w, int h)
{
	if (pd3dDevice==NULL)	return false;
	pd3dDevice->CreateOffscreenPlainSurface( w, h, D3DFMT_X8R8G8B8,
											 D3DPOOL_DEFAULT, &pBG, NULL );
	d3di.push_back(pBG);
	return (D3DXLoadSurfaceFromFile( pBG, NULL, NULL, pSrcFile,
									 NULL, D3DX_FILTER_NONE, 0, NULL) == D3D_OK);
}

bool Framework::LoadFont(LPCTSTR pFacename, int height)
{
	if( D3D_OK != D3DXCreateFont( pd3dDevice, height, 0, FW_DONTCARE, 1, FALSE, DEFAULT_CHARSET, 
								OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, 
								pFacename, &pFont ) )
	{
		MessageBox(NULL, L"CreateFont�Ɏ��s���܂����B", L"InitD3D", MB_OK|MB_ICONSTOP);
		return false;
	}
	d3di.push_back(pFont);
    return true;
}

bool Framework::Setup( HWND /* hWnd */)
{
	return true;
}

Framework::Framework(HINSTANCE hInst)
	: DeviceLost(false)
	, pD3D(NULL)
	, pd3dDevice(NULL)
	, pBack(NULL)
	, pBG(NULL)
	, pSprite(NULL)
	, pFont(NULL)
	, bMenu(FALSE)
{
	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_CLASSDC;	// DirectX�A�v���͂���
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInst;
	wcex.hIcon			= NULL;
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= L"paoD3D_Framework";
	wcex.hIconSm		= NULL;

	style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
	exstyle = 0;

	instance = this;
}

Framework::~Framework()
{
}

//	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MYTEST));
//	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
//	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MYTEST);
//	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

void Framework::SetMenu(LPCTSTR menu)
{
	wcex.lpszMenuName	= menu;
	bMenu = TRUE;
}

void Framework::MainLoop(LPCTSTR windowName, int width, int height)
{
	RegisterClassEx( &wcex );

	if (width != CW_USEDEFAULT){
		RECT rc;
		SetRect( &rc, 0, 0, width, height );
		AdjustWindowRect( &rc, style, bMenu );
		width = rc.right - rc.left;
		height = rc.bottom - rc.top;
	}

	// Create the application's window
	HWND hWnd = CreateWindowEx(
					exstyle,
					wcex.lpszClassName,
					windowName, 
					style,
					CW_USEDEFAULT, 0,
					width, height,
					NULL, NULL,
					wcex.hInstance, NULL
				);

	if ( Init( hWnd ) )
	if ( Setup( hWnd ) )
	{ 
		// Show the window
		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );

		// Enter the message loop
		MSG msg; 
		ZeroMemory(&msg, sizeof(msg));
		while(msg.message != WM_QUIT)
		{
			if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)){
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			} else {
				Update();
				Render();
			}
		}
	}

	UnregisterClass( wcex.lpszClassName, wcex.hInstance );
}


LRESULT WINAPI Framework::WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg ){
	case WM_ACTIVATEAPP:
		instance->Render();
		ValidateRect( hWnd, NULL );
		break;

	case WM_CREATE:
		break;

	case WM_COMMAND:
		// �I�����ꂽ���j���[�̉��:
		return instance->WmCommand( hWnd, wParam, lParam );

	case WM_PAINT:
		instance->Render();
		ValidateRect( hWnd, NULL );
		break;

	case WM_LBUTTONDOWN:
		return instance->WmLButtondown( hWnd, wParam, lParam );

	case WM_MBUTTONDOWN:
		return instance->WmMButtondown( hWnd, wParam, lParam );

	case WM_RBUTTONDOWN:
		return instance->WmRButtondown( hWnd, wParam, lParam );

	case WM_MOUSEMOVE:
		return instance->WmMouseMove( hWnd, wParam, lParam );

	case WM_DESTROY:
		instance->Cleanup();
		PostQuitMessage( 0 );
		break;

#if 0
	case WM_CLOSE:    // �E�B���h�E�������悤�Ƃ����Ƃ��ɑ����Ă���
		if( MessageBox( hWnd, L"�{���ɏI�����Ă������ł����H", L"�m�F", MB_YESNO ) == IDNO )
		{
			return 0; // ���Ȃ����߂ɂ�DefWindowProc()�ɏ��������Ă͂Ȃ�Ȃ�
		}
		// ���Ă����̂Ȃ�DefWindowProc()�ɂ܂�����΂���
#endif
	default:
	    return DefWindowProc( hWnd, msg, wParam, lParam );
	}
	return 0;
}

LRESULT Framework::WmCommand( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
    return DefWindowProc( hWnd, WM_COMMAND, wParam, lParam );
}

LRESULT Framework::WmLButtondown( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
    return DefWindowProc( hWnd, WM_LBUTTONDOWN, wParam, lParam );
}

LRESULT Framework::WmMButtondown( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
    return DefWindowProc( hWnd, WM_MBUTTONDOWN, wParam, lParam );
}

LRESULT Framework::WmRButtondown( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
    return DefWindowProc( hWnd, WM_RBUTTONDOWN, wParam, lParam );
}

LRESULT Framework::WmMouseMove( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
    return DefWindowProc( hWnd, WM_MOUSEMOVE, wParam, lParam );
}

// �o�[�W�������{�b�N�X�̃��b�Z�[�W �n���h���ł��B
INT_PTR CALLBACK Framework::About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void Framework::Render()
{
	if( NULL == pd3dDevice )
		return;
#if 0
	static int prevTime;
	int time = prevTime;
	prevTime = (int)timeGetTime();
	time = prevTime - time;
#endif
    // Begin the scene
	if( SUCCEEDED( pd3dDevice->BeginScene() ) )
	{
        // Rendering of scene objects can happen here
		Draw();
        // End the scene
		pd3dDevice->EndScene();
	}

	if (MovieIsPlay()) return;	// �ǂ����Ă������ɕK�v

    // Present the backbuffer contents to the display
	HRESULT hr = pd3dDevice->Present( NULL, NULL, NULL, NULL );

	if ( D3DERR_DEVICELOST == hr )
	{
		DeviceLost = true;
	}
	else if( D3DERR_DRIVERINTERNALERROR == hr )
	{
		// When D3DERR_DRIVERINTERNALERROR is returned from Present(),
		// the application can do one of the following:
		// 
		// - End, with the pop-up window saying that the application cannot continue 
		//   because of problems in the display adapter and that the user should 
		//   contact the adapter manufacturer.
		//
		// - Attempt to restart by calling IDirect3DDevice9::Reset, which is essentially the same 
		//   path as recovering from a lost device. If IDirect3DDevice9::Reset fails with 
		//   D3DERR_DRIVERINTERNALERROR, the application should end immediately with the message 
		//   that the user should contact the adapter manufacturer.
		// 
		// The framework attempts the path of resetting the device
		// 
		DeviceLost = true;
	}
}

void Framework::Draw()
{
}

void Framework::Update()
{
}

void Framework::Cleanup()
{
//	for_each(d3di.begin(), d3di.end(), std::mem_fun(&IUnknown::Release));
	for (std::vector<IUnknown*>::iterator it=d3di.begin(); it!=d3di.end(); ++it)
		(*it)->Release();

	d3di.clear();
}

void Framework::OnLostDevice()
{
	if (pSprite)
		pSprite->OnLostDevice();
	if (pFont)
		pFont->OnLostDevice();
}
