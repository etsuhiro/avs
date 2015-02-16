/*! @file ImageManager.h */
/*!
	�摜�}�l�[�W��
 */

#pragma	once

#include	<d3dx9tex.h>
#include	<string>

#include	"../avs/ImageCanvas.h"

namespace image {
	const int FILE_NAME_BACKUP =  10;	//!< �����ő吔

	struct D3dxImageTag : public avs::ImageTag {
		IDirect3DTexture9*	pTex;			//!< �摜�|�C���^
//		bool				LoadFlg;		//!< ���[�h�t���O
		TCHAR				filename[MAX_PATH];	//!< path��g���q�͊܂܂Ȃ��t�@�C����
//		char				Backup[FILE_NAME_BACKUP][MAX_PATH];	// �R���{�{�b�N�X�p����
	};

	class ImageManager {
	private:
		const LPDIRECT3DDEVICE9 m_pDevice;
		int						m_maxLayer;
		const char*				m_Path;	// �g��Ȃ��c���ȁH
//		int						m_Load;

		D3dxImageTag*			m_Tag;
//		char					m_File[256][MAX_PATH];		// �t�@�C���� ����Ȃ����

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
