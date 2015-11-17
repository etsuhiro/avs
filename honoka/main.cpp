//
// DxLibを使用したavsの実行環境
//
#include "DxLib.h"
#include "avs.h"
#include "DxLibAvsImage.h"
#include <list>
#include <string>

XmlEnum sinkxsd;

class DxLibMikuPrint : public IMikuPrint {
	virtual void TagName(const char* name, int indent) override
	{
		for (int i = 0; i<indent; i++)
			printfDx("  ");
		printfDx("<%s", name);
	}

	virtual void TagClose(const char* name, int indent) override
	{
		printfDx(">\n");
	}

	virtual void EmptyElementTagClose(const char* name, int indent) override
	{
		printfDx("/>\n");
	}

	virtual void EndTag(const char* name, int indent) override
	{
		for (int i = 0; i<indent; i++)
			printfDx("  ");
		printfDx("</%s>\n", name);
	}

	virtual void AttrName(const char* name) override
	{
		printfDx(" %s=???", name);
	}

	virtual void Text(const char* text, int indent) override
	{
		for (int i = 0; i<indent; i++)
			printfDx("  ");
		printfDx("%s\n", text);
	}
};

void LoadScript(std::vector<char>& scriptbuf, const char* path, XmlEnum& xmls)
{
	TiXmlDocument xml;
	// スクリプトを読む
	if (!xml.LoadFile(path)){
		printf("file read error %s\n", path);
		exit(0);
	}

	// バイナリに変換
	hashmap_t hashmap;
	XmlBin().Conv(scriptbuf, hashmap, xml, &xmls);

	mikuPrint(&scriptbuf[0], xmls, DxLibMikuPrint());
	ScreenFlip();
}

class MyScriptEngine : public avs::ScriptEngine
{
};

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

	MyScriptEngine script;
	DxLibAvsImage avsImage;
	script.AddImageListener(&avsImage);

	std::vector<char> scriptBuf;
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
			LoadScript(scriptBuf, fname_full, sinkxsd);
			script.SetScript(&scriptBuf[0]);
		}
	}

	int gametime = GetNowCount();
	while (1){
		// キーの状態をチェック
		{
			char Buf[256];
			GetHitKeyStateAll(Buf);
			if (Buf[KEY_INPUT_LCONTROL] || Buf[KEY_INPUT_RCONTROL])
			{
			}
		}

		int nowtime = GetNowCount();
		int elapsedtime = nowtime - gametime;
		gametime = nowtime;
		avs::RunningStatus stat = script.Run(elapsedtime);
		if (stat == avs::FINISH)
			break;

		avsImage.Update(elapsedtime);
		avsImage.Draw();

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
