/*! @file ImageTag.h */
/*!
	イメージタグ
 */

#ifndef avs_image_tag_h_
#define avs_image_tag_h_

#include	<d3dx9tex.h>

namespace avs {

	struct ImageTag {
		short x,y;							// 画像データの中心点の表示座標（スクリーン中央が原点で左上がマイナス、右下がプラス）
		short u,v;							// 画像表示開始オフセット（左上を原点）
		short w,h;							// 画像表示サイズ（0の場合は画像データのサイズ、マイナスで反転）
		float r,g,b,a;						// 0～1.f
		float mag_x,mag_y;					// 表示拡大縮小倍率
		short rot_x,rot_y,rot_z;			// 画像の回転角度

		ImageTag()
		{
			x = 0;
			y = 0;
			u = 0;
			v = 0;
			w = 0;
			h = 0;
			r = 1.f;
			g = 1.f;
			b = 1.f;
			a = 1.f;
			mag_x = 1.f;
			mag_y = 1.f;
			rot_x = 0;
			rot_y = 0;
			rot_z = 0;
		}
	};
}

#endif
