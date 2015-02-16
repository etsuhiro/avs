/*! @file Wd3xImageTag.h */

#pragma	once

#include	<d3dx9tex.h>

#include	"../avs/ImageTag.h"

namespace image {
	const int FILE_NAME_BACKUP =  10;	// �����ő吔

	struct Wd3xImageTag : public ImageTag {

		IDirect3DTexture9*	pTex;			// �摜�|�C���^
		bool				LoadFlg;		// ���[�h�t���O
		char	backup[FILE_NAME_BACKUP][FILE_NAME_LENGTH];	// �R���{�{�b�N�X�p����

		Wd3xImageTag()
			: ImageTag()
		{
			pTex = NULL;
			LoadFlg = false;

			for	( int i = 0; i < FILE_NAME_BACKUP; i++){
				backup[i][0] = '\0';
			}
		}
	};
}
