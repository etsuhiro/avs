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
		Pos pos;	// ウィンドウの表示位置
		Pos text;	// テキストの開始位置（posからのオフセット）
		int linefeed;	// 改行幅
		Pos name;	// 名前の開始位置（posからのオフセット）
		Pos click;	// クリックアイコンの表示位置（posからのオフセット）
		Area button1;	// ボタン１の・・・
	};

	struct TextArea {
		int clum;	// 横文字数
		int row;	// 行数
		int extra;	// ぶら下げ用文字数
	};

	struct SetupData {
		int width;
		int height;
		int nLayer;
		int defaultTime;
		TextWindow window;
		TextArea text;
		int fovy;		// 画角(視野角)
	};

}

