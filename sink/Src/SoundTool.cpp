/*! @file SoundTool.cpp */
#include <d3d9.h>
#include <d3dx9tex.h>

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

#include "SoundTool.h"
#include "resource.h"
#include "sound/mysound.h"
#include "Setup.h"

extern HWND				hSndWnd;
extern WindowsSetupData	g_SetupData;

SoundTool::SoundTool()
{
}

SoundTool::~SoundTool()
{
}

void SoundTool::Create(HINSTANCE hInst, HWND hWnd)
{
	OPENFILENAME ofn;
	TCHAR fname_full[MAX_PATH] = L"";   // ファイル名(フルパス)を受け取る領域
	TCHAR fname[MAX_PATH];        // ファイル名を受け取る領域

	// 構造体に情報をセット
	ZeroMemory( &ofn, sizeof(ofn) );				// 最初にゼロクリアしておく
	ofn.lStructSize		= sizeof(ofn);				// 構造体のサイズ
	ofn.hwndOwner		= hWnd;						// コモンダイアログの親ウィンドウハンドル
	ofn.lpstrFilter		= L"ogg(*.ogg)\0*.ogg\0\0";	// ファイルの種類
	ofn.lpstrFile		= fname_full;				// 選択されたファイル名(フルパス)を受け取る変数のアドレス
	ofn.lpstrFileTitle	= fname;					// 選択されたファイル名を受け取る変数のアドレス
	ofn.nMaxFile		= MAX_PATH;					// lpstrFileに指定した変数のサイズ
	ofn.nMaxFileTitle	= MAX_PATH;					// lpstrFileTitleに指定した変数のサイズ
	ofn.Flags			= OFN_FILEMUSTEXIST;		// フラグ指定
	ofn.lpstrTitle		= L"ファイルを開く";		// コモンダイアログのキャプション
	ofn.lpstrDefExt		= L"ogg";					// デフォルトのファイルの種類
	// 初期フォルダの指定
	ofn.lpstrInitialDir = g_SetupData.dataPath;
	// ファイルを開くコモンダイアログを作成
	if(!GetOpenFileName(&ofn)) return;
//	sound::SoundPlay(fname_full);

	char path[MAX_PATH];
	WideCharToMultiByte(CP_ACP, 0, fname_full, -1, path, MAX_PATH, NULL, NULL);
	sound::Bgm.Req(path);
#if 0
	// 既に表示されているか？
	if (IsWindow(hSndWnd) == 0){
		// されていないならダイアログ作成
		hSndWnd = CreateDialog(hInst, L"SoundTool", hWnd, (DLGPROC)Proc);
	}
#endif
}

// プロシージャ
LRESULT CALLBACK SoundTool::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		// Pan値スライダー初期化
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(-100, 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)0);
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			hSndWnd = NULL;
			EndDialog(hDlg, IDCANCEL);
			DestroyWindow(hDlg);
			hDlg = NULL;
			return FALSE;
		case IDC_BUTTON1: // 「再生」＆「一時停止」ボタン
			return FALSE;
		case IDC_BUTTON2: // 「停止」ボタン
			return FALSE;
		}
		return FALSE;
	}

	return FALSE;
}

// ScriptTool更新
void SoundTool::Update(HWND hDlg)
{
#if 0
	if (g_ScriptData.CheckLoad()){
		int		buf_size = strlen(g_ScriptData.MultiBuffer()) + 1;
		char*	tmp_buff = new char[buf_size];
		char*	pb = tmp_buff, *pl = tmp_buff;
		TIXML_STRING	xml_str;

		memset( tmp_buff, 0x0, sizeof(char)*buf_size );
		xml_str.reserve( buf_size*2 );
		/* 改行コードを LF → CR+LF */
		strcpy_s( tmp_buff, buf_size*sizeof(char), g_ScriptData.MultiBuffer() );
		for	( int i = 0; i < (int)buf_size; i++, pb++){
			if	( !(*pb)){ break; }
#if	0
			if	( *pb == 0x0a){
				xml_str.append( pl, pb - pl );
				xml_str += 0x0d;	// '\r';	// CR
				xml_str += 0x0a;	// '\n';	// LF
				pl = pb;
			}
#endif
		}
		xml_str.append( pl, pb - pl );

		delete[]	tmp_buff;

		// エディットコントロールに読み込んだテキストの表示
		size_t	buf_size2 = xml_str.size() + 1;
		TCHAR*	str_t = new TCHAR [buf_size2];
		memset(str_t, 0x0, sizeof(TCHAR)*(buf_size2));
		MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, xml_str.c_str(), -1, str_t, buf_size2);
		SetDlgItemText(hDlg, IDC_EDIT1, str_t);
		g_ScriptData.BreakLoadFlg();
		delete [] str_t;

		// ScriptToolの表示を更新
		InvalidateRect(hDlg, NULL, TRUE);
	}
#endif
}
