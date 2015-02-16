// ここにサンプルプログラムをペーストしてください
// 初期状態では LoadGraphScreen のサンプルプログラムが入力されています。
#include "DxLib.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance,
				 LPSTR lpCmdLine, int nCmdShow )
{
//	SetGraphMode(800, 600, 32);	// ウィンドウのサイズ
	ChangeWindowMode(TRUE);	// ウィンドウモードで起動させる
	if( DxLib_Init() == -1 )	// ＤＸライブラリ初期化処理
	{
		 return -1;				// エラーが起きたら直ちに終了
	}

	// ＢＭＰ画像の表示
//	LoadGraphScreen( 0 , 0 , "test1.bmp" , TRUE ) ;
	LoadGraphScreen(0, 0, "bg_18_1.png", TRUE);
	LoadGraphScreen(0, -40, "st_sor_s_916.png", TRUE);

	WaitKey() ;					// キーの入力待ち((7-3)『WaitKey』を使用)

	DxLib_End() ;				// ＤＸライブラリ使用の終了処理

	return 0 ;					// ソフトの終了
}
