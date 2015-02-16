/*! @file ImageTag.h */
/*!
	�C���[�W�^�O
 */

#ifndef avs_image_tag_h_
#define avs_image_tag_h_

#include	<d3dx9tex.h>

namespace avs {

	struct ImageTag {
		short x,y;							// �摜�f�[�^�̒��S�_�̕\�����W�i�X�N���[�����������_�ō��オ�}�C�i�X�A�E�����v���X�j
		short u,v;							// �摜�\���J�n�I�t�Z�b�g�i��������_�j
		short w,h;							// �摜�\���T�C�Y�i0�̏ꍇ�͉摜�f�[�^�̃T�C�Y�A�}�C�i�X�Ŕ��]�j
		float r,g,b,a;						// 0�`1.f
		float mag_x,mag_y;					// �\���g��k���{��
		short rot_x,rot_y,rot_z;			// �摜�̉�]�p�x

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
