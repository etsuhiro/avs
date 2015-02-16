#include <d3d9.h>
#include <d3dx9tex.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

#define _USE_MATH_DEFINES
#include <math.h>

#include "../Setup.h"
#include "ImageManager.h"

extern WindowsSetupData g_SetupData;

using namespace avs;

namespace image {

ImageManager*	pImgMan = NULL;

ImageManager::ImageManager( const LPDIRECT3DDEVICE9& pDevice, int maxLayer, const char* path )
	: m_pDevice(pDevice)
	, m_maxLayer(maxLayer)
	, m_Path(path)
{
	pImgMan = this;

	m_Tag = new D3dxImageTag[m_maxLayer];
	if (m_Tag){
		for (int i=0; i<m_maxLayer; i++){
			m_Tag[i].pTex = NULL;
			m_Tag[i].filename[0] = '\0';
		}
	}
}

ImageManager::~ImageManager( void )
{
	pImgMan = NULL;
	delete[]	m_Tag;
}

bool ImageManager::Load( int layer, const TCHAR *fname )
{
	if (layer >= m_maxLayer || layer<0){
		MessageBox( NULL, L"レイヤー番号が異常です", L"LoadImage", MB_OK|MB_ICONSTOP);
		return false;
	}

//	TCHAR file_t[MAX_PATH];

//	MultiByteToWideChar( CP_OEMCP, MB_PRECOMPOSED, fname, -1, file_t, MAX_PATH );

	D3DXIMAGE_INFO image_info;
	HRESULT rest = D3DXCreateTextureFromFileEx(
												m_pDevice,
												fname,
										//			0,0,		// 幅と高さはファイルから得る。ただし2のべき乗に切り上げられる
												D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2,		// 幅と高さはファイルから得る
												1,		// ミップレベル　必要ないので１を指定
												0,		// 特に使用しない
												D3DFMT_FROM_FILE,
												D3DPOOL_MANAGED,
												D3DX_FILTER_NONE, D3DX_FILTER_NONE,
												0,		// カラーキーは使用しない
												&image_info,	// pSrcInfo　あると便利かも
												NULL,
												&(m_Tag[layer].pTex));
	if (rest != D3D_OK){
		switch( rest ){
		case D3DERR_NOTAVAILABLE:
			MessageBox( NULL, L"紹介されたテクニックをサポートしてません", L"LoadImage", MB_OK|MB_ICONSTOP); break;
		case D3DERR_OUTOFVIDEOMEMORY:
			MessageBox( NULL, L"Direct3D処理に必要なディスプレイメモリが不足しています", L"LoadImage", MB_OK|MB_ICONSTOP); break;
		case D3DERR_INVALIDCALL:
			MessageBox( NULL, L"呼び出し無効です。メソッドパラメータを確認してください。", L"LoadImage", MB_OK|MB_ICONSTOP); break;
		case D3DXERR_INVALIDDATA:
			MessageBox( NULL, L"無効なデータです", L"LoadImage", MB_OK|MB_ICONSTOP); break;
		case E_OUTOFMEMORY:
			MessageBox( NULL, L"Direct3Dが呼び出し完了のためのメモリを割り当てできませんでした", L"LoadImage", MB_OK|MB_ICONSTOP); break;
		}
		return false;
	}

	m_Tag[layer].w = image_info.Width;
	m_Tag[layer].h = image_info.Height;

	m_Tag[layer].x = 0;
	m_Tag[layer].y = 0;
	m_Tag[layer].u = 0;
	m_Tag[layer].v = 0;

	m_Tag[layer].r = 1.f;
	m_Tag[layer].g = 1.f;
	m_Tag[layer].b = 1.f;
	m_Tag[layer].a = 1.f;

	m_Tag[layer].mag_x = 1.f;
	m_Tag[layer].mag_y = 1.f;

	m_Tag[layer].rot_x = 0;
	m_Tag[layer].rot_y = 0;
	m_Tag[layer].rot_z = 0;

//	m_Tag[layer].LoadFlg = true;
	return true;
}

void	ImageManager::Update()
{
}

void	ImageManager::Draw( ID3DXSprite* pSpr )
{
	for	( int i = 0; i < m_maxLayer; i++){
		if ( m_Tag[i].filename[0] == '\0')	continue;

		D3DXMATRIX	mtx, tmpm;

		D3DXMatrixIdentity( &mtx );		// 単位行列化

		float w = (float)m_Tag[i].w;
		float h = (float)m_Tag[i].h;

		D3DXVECTOR3 pos((float)m_Tag[i].x + 0.5f,
						(float)m_Tag[i].y + 0.5f,
						0);
		D3DXVECTOR3 center(w/2, h/2, 0);

		if	( w < 0){
			D3DXPLANE pln(1,0,0,0);
			pos.x *= -1;
			center.x *= -1;
			w *= -1;
			D3DXMatrixReflect( &tmpm, &pln );
			D3DXMatrixMultiply( &mtx, &mtx, &tmpm );
		}
		if	( h < 0){
			D3DXPLANE pln(0,1,0,0);
			pos.y *= -1;
			center.y *= -1;
			h *= -1;
			D3DXMatrixReflect( &tmpm, &pln );
			D3DXMatrixMultiply( &mtx, &mtx, &tmpm );
		}

		RECT rect;
		rect.left   = m_Tag[i].u;
		rect.top    = m_Tag[i].v;
		rect.right  = rect.left + (int)w;
		rect.bottom = rect.top  + (int)h;

		D3DCOLOR	color;
		color = D3DCOLOR((int)(m_Tag[i].b*255)+
						 (int)((u_short)(m_Tag[i].g*255)<<8)+
						 (int)((u_short)(m_Tag[i].r*255)<<16)+
						 (int)((u_short)(m_Tag[i].a*255)<<24));
		{
			D3DXVECTOR3	rcen(m_Tag[i].x,m_Tag[i].y,0);
			D3DXVECTOR3	scale(m_Tag[i].mag_x,m_Tag[i].mag_y,1.f);
			D3DXQUATERNION quat;

			D3DXQuaternionRotationYawPitchRoll( &quat,
												m_Tag[i].rot_y*M_PI/(float)0x8000,
												m_Tag[i].rot_x*M_PI/(float)0x8000,
												m_Tag[i].rot_z*M_PI/(float)0x8000
												);

			D3DXMatrixTransformation(
				&tmpm,
				&rcen,
				NULL,
				&scale,
				&rcen,
				&quat,
				NULL
			);
			D3DXMatrixMultiply( &mtx, &mtx, &tmpm );
		}
		pSpr->SetTransform( &mtx );
		pSpr->Draw( m_Tag[i].pTex, &rect, &center, &pos, color );
	}
}

}
