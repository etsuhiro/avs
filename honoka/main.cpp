#include "DxLib.h"
#include "avs.h"
#include <list>
#include <string>
#include <algorithm>

struct Asset {
	std::string name;
	int handle;
};

class AssetManager {
	std::list<Asset> m_assets;

public:
	~AssetManager(){
		for (auto& asset : m_assets){
			DeleteGraph(asset.handle);
		}
		m_assets.clear();
	}
	int Load(const char* path){
		auto it = std::find_if(m_assets.begin(), m_assets.end(), [&path](Asset& asset){return asset.name == path; });
		if (it != m_assets.end())
			return it->handle;

		int handle = LoadGraph(path);
		if (handle >= 0){
			Asset asset{ path, handle };
			m_assets.push_back(asset);
		}
		return handle;
	}
	void Remove(int handle){
		auto it = std::find_if(m_assets.begin(), m_assets.end(), [&handle](Asset& asset){return asset.handle == handle; });
		if (it != m_assets.end()){
			DeleteGraph(it->handle);
			m_assets.erase(it);
		}
	}
};

struct Sprite {
	int ghandle;
	int x;
	int y;
};

std::list<Sprite> sprites;
XmlEnum sinkxsd;

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	SetMainWindowText("穂乃果");
	SetOutApplicationLogValidFlag(FALSE);	// ログ出力を行わない
	//	SetGraphMode(800, 600, 32);	// ウィンドウのサイズ
	ChangeWindowMode(TRUE);	// ウィンドウモードで起動させる
	if (DxLib_Init() == -1)	// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}
	SetDrawScreen(DX_SCREEN_BACK);	// 裏画面描画モード

	{
		TiXmlDocument schema;
		// スキーマを読む
		if (!schema.LoadFile("sink.xsd")){
			printf("file read error (sink.xsd)\n");
			exit(0);
		}
		//	XmlEnum xmls;
		sinkxsd.ReadSchema(schema.RootElement());
	}

	AssetManager asset_manager;

	{
		OPENFILENAME ofn;
		TCHAR fname_full[MAX_PATH] = "";   // ファイル名(フルパス)を受け取る領域
		// 構造体に情報をセット
		ZeroMemory(&ofn, sizeof(ofn));				// 最初にゼロクリアしておく
		ofn.lStructSize = sizeof(ofn);				// 構造体のサイズ
		ofn.hwndOwner = NULL;						// コモンダイアログの親ウィンドウハンドル
		ofn.lpstrFilter = "script(*.xml)\0*.xml\0\0";	// ファイルの種類
		ofn.lpstrFile = fname_full;				// 選択されたファイル名(フルパス)を受け取る変数のアドレス
		ofn.nMaxFile = MAX_PATH;		// lpstrFileに指定した変数のサイズ
		ofn.Flags = OFN_FILEMUSTEXIST;		// フラグ指定
		ofn.lpstrTitle = "ファイルを開く";		// コモンダイアログのキャプション
		ofn.lpstrDefExt = "xml";					// デフォルトのファイルの種類
		// ファイルを開くコモンダイアログを作成
		if (GetOpenFileName(&ofn)){
		}
	}

	while(1){
		// キーの状態をチェック
		{
			char Buf[256];
			GetHitKeyStateAll(Buf);
			if (Buf[KEY_INPUT_LCONTROL] || Buf[KEY_INPUT_RCONTROL])
			{
				if (Buf[KEY_INPUT_O])
				{
					OPENFILENAME ofn;
					TCHAR fname_full[MAX_PATH] = "";   // ファイル名(フルパス)を受け取る領域
					// 構造体に情報をセット
					ZeroMemory(&ofn, sizeof(ofn));				// 最初にゼロクリアしておく
					ofn.lStructSize = sizeof(ofn);				// 構造体のサイズ
					ofn.hwndOwner = NULL;						// コモンダイアログの親ウィンドウハンドル
					ofn.lpstrFilter = "png(*.png)\0*.png\0\0";	// ファイルの種類
					ofn.lpstrFile = fname_full;				// 選択されたファイル名(フルパス)を受け取る変数のアドレス
					ofn.nMaxFile = MAX_PATH;		// lpstrFileに指定した変数のサイズ
					ofn.Flags = OFN_FILEMUSTEXIST;		// フラグ指定
					ofn.lpstrTitle = "ファイルを開く";		// コモンダイアログのキャプション
					ofn.lpstrDefExt = "png";					// デフォルトのファイルの種類
					// ファイルを開くコモンダイアログを作成
					if (GetOpenFileName(&ofn)){
						int handle = asset_manager.Load(fname_full);
						if (handle >= 0){
							Sprite sprite{ handle, 0, 0 };
							sprites.push_back(sprite);
						}
					}
				}
			}
		}
		for (auto& sprite: sprites)
		{
			DrawGraph(sprite.x, sprite.y, sprite.ghandle, TRUE);
		}

		// メッセージループに代わる処理をする
		if (ProcessMessage() == -1)
		{
			break;        // エラーが起きたらループを抜ける

		}
		ScreenFlip();
		ClearDrawScreen();
	}

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
