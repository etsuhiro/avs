/*! @file ImageManager.h */
/*!
	画像マネージャ
 */

#pragma	once

#include	<d3dx9tex.h>
#include	<string>

#include	"../avs/ImageCanvas.h"

namespace image {
	const int FILE_NAME_BACKUP =  10;	//!< 履歴最大数

	struct D3dxImageTag : public avs::ImageTag {
		IDirect3DTexture9*	pTex;			//!< 画像ポインタ
//		bool				LoadFlg;		//!< ロードフラグ
		TCHAR				filename[MAX_PATH];	//!< pathや拡張子は含まないファイル名
//		char				Backup[FILE_NAME_BACKUP][MAX_PATH];	// コンボボックス用履歴
	};

	class ImageManager {
	private:
		const LPDIRECT3DDEVICE9 m_pDevice;
		int						m_maxLayer;
		const char*				m_Path;	// 使わない…かな？
//		int						m_Load;

		D3dxImageTag*			m_Tag;
//		char					m_File[256][MAX_PATH];		// ファイル名 いらないよね

	public:
		D3dxImageTag&	ImgTag( int x ) { return m_Tag[x]; }

		void	Update();
		void	Draw( ID3DXSprite* pSpr );
		bool	Load( int layer, const TCHAR *fname );
		ImageManager( const LPDIRECT3DDEVICE9& pDevice, int maxLayer, const char *path );
		~ImageManager( void );
	};

	extern ImageManager*	pImgMan;
};
