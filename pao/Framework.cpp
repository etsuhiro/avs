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

Framework* Framework::instance;	// インスタンスを保持

bool Framework::Init( HWND hWnd )
{
	// Create the D3D object, which is needed to create the D3DDevice.
	if( NULL == ( pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
	{
		MessageBox(NULL, L"Dirext3Dの初期化に失敗しました。", L"InitD3D", MB_OK|MB_ICONSTOP);
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
	ZeroMemory( &d3dpp, sizeof(d3dpp) );	// 初期値でよいものの設定を省くため
	d3dpp.Windowed = TRUE;	// ウィンドウモード。FALSEでフルスクリーンモード
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;	// ディスプレイドライバにお任せ
	d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
	d3dpp.PresentationInterval = D3DPRESENT_DONOTWAIT;	// 大きなスプライトの移動もチラつかない

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
		MessageBox(NULL, L"CreateDeviceに失敗しました。", L"InitD3D", MB_OK|MB_ICONSTOP);
		return false;
	}
	d3di.push_back(pd3dDevice);

    // Device state would normally be set here

	// バックバッファを取得
	if (pd3dDevice->GetBackBuffer(
		0,	// スワップ チェーンを指定する符号なし整数。
		0,	// 返すバック バッファ オブジェクトのインデックス。
			// バック バッファは 0 からバック バッファの総数 - 1 までナンバリングされる。
			// 0 の戻り値は最初のバック バッファを示し、フロント バッファではない。
			// このメソッドを使ってフロント バッファにアクセスすることはできない。 
		D3DBACKBUFFER_TYPE_MONO,	// MicrosoftR DirectXR 9.0 ではステレオ ビューはサポートされていないので、
									// このパラメータの有効な値は D3DBACKBUFFER_TYPE_MONO だけである。 
		&pBack		// 返されるバック バッファ サーフェイスを表す IDirect3DSurface9 インターフェイスへのポインタのアドレス。
		) != D3D_OK)
	{
		MessageBox(NULL, L"GetBackBufferに失敗しました。", L"InitD3D", MB_OK|MB_ICONSTOP);
		return false;
	}
	d3di.push_back(pBack);

	// スプライト
	if (D3DXCreateSprite( pd3dDevice, &pSprite ) != D3D_OK)
	{
		MessageBox(NULL, L"CreateSpriteに失敗しました。", L"InitD3D", MB_OK|MB_ICONSTOP);
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
		MessageBox(NULL, L"CreateFontに失敗しました。", L"InitD3D", MB_OK|MB_ICONSTOP);
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

	wcex.style			= CS_CLASSDC;	// DirectXアプリはこれ
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
		// 選択されたメニューの解析:
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
	case WM_CLOSE:    // ウィンドウが閉じられようとしたときに送られてくる
		if( MessageBox( hWnd, L"本当に終了してもいいですか？", L"確認", MB_YESNO ) == IDNO )
		{
			return 0; // 閉じないためにはDefWindowProc()に処理させてはならない
		}
		// 閉じていいのならDefWindowProc()にまかせればいい
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

// バージョン情報ボックスのメッセージ ハンドラです。
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

	if (MovieIsPlay()) return;	// どうしてもここに必要

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
