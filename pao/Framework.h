/*! @file Framework.h */
#pragma once

#include <d3d9.h>
#include <vector>
#include <d3dx9tex.h>

// DirectX用フレームワーク

namespace pao
{
	class Framework {
	public:
		Framework(HINSTANCE hInst);
		~Framework();
		void SetMenu(LPCTSTR menu);
		void MainLoop(LPCTSTR windowName, int width=CW_USEDEFAULT, int heght=0);

		WNDCLASSEX wcex;
		DWORD style;
		DWORD exstyle;

	protected:
		bool LoadBG(LPCTSTR pSrcFile, int w, int h);
		bool LoadFont(LPCTSTR pFacename, int height);

		bool DeviceLost;
		LPDIRECT3D9			pD3D;		//!< Used to create the D3DDevice
		LPDIRECT3DDEVICE9	pd3dDevice;	//!< Our rendering device
		LPDIRECT3DSURFACE9	pBack;		//!< バックサーフェイス
		LPDIRECT3DSURFACE9	pBG;		//!< BG用サーフェイス
		ID3DXSprite*		pSprite;	//!< Sprite for batching draw text calls
		ID3DXFont*			pFont;
		std::vector<IUnknown *>	d3di;	//!< Cleanup用

		//! バージョン情報ボックスのメッセージ ハンドラ 汎用品
		static INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

		virtual bool	MovieIsPlay(){ return false; }

	private:
		virtual bool Init( HWND hWnd );
		virtual bool Setup( HWND hWnd );
		virtual void Update();
		virtual void Render();
		virtual void Draw();
		virtual void Cleanup();
		virtual void OnLostDevice();
		virtual LRESULT WmCommand( HWND hWnd, WPARAM wParam, LPARAM lParam );
		virtual LRESULT WmLButtondown( HWND hWnd, WPARAM wParam, LPARAM lParam );
		virtual LRESULT WmMButtondown( HWND hWnd, WPARAM wParam, LPARAM lParam );
		virtual LRESULT WmRButtondown( HWND hWnd, WPARAM wParam, LPARAM lParam );
		virtual LRESULT WmMouseMove( HWND hWnd, WPARAM wParam, LPARAM lParam );

		static LRESULT WINAPI WndProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam );

		BOOL bMenu;	// メニューバー表示
		static Framework* instance;
	};
}

