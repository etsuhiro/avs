/*! @file Wd3xImageTag.h */

#pragma	once

#include	<d3dx9tex.h>

#include	"../avs/ImageTag.h"

namespace image {
	const int FILE_NAME_BACKUP =  10;	// 履歴最大数

	struct Wd3xImageTag : public ImageTag {

		IDirect3DTexture9*	pTex;			// 画像ポインタ
		bool				LoadFlg;		// ロードフラグ
		char	backup[FILE_NAME_BACKUP][FILE_NAME_LENGTH];	// コンボボックス用履歴

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
