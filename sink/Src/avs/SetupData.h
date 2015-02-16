#pragma once

namespace avs
{
	struct Pos {
		int x;
		int y;
	};
	struct Area : public Pos {
		int w;
		int h;
	};

	struct TextWindow {
		Pos pos;	// �E�B���h�E�̕\���ʒu
		Pos text;	// �e�L�X�g�̊J�n�ʒu�ipos����̃I�t�Z�b�g�j
		int linefeed;	// ���s��
		Pos name;	// ���O�̊J�n�ʒu�ipos����̃I�t�Z�b�g�j
		Pos click;	// �N���b�N�A�C�R���̕\���ʒu�ipos����̃I�t�Z�b�g�j
		Area button1;	// �{�^���P�́E�E�E
	};

	struct TextArea {
		int clum;	// ��������
		int row;	// �s��
		int extra;	// �Ԃ牺���p������
	};

	struct SetupData {
		int width;
		int height;
		int nLayer;
		int defaultTime;
		TextWindow window;
		TextArea text;
		int fovy;		// ��p(����p)
	};

}

