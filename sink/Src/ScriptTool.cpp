/*! @file ScriptTool.cpp */
#include <d3d9.h>
#include <d3dx9tex.h>

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

#include "ScriptTool.h"
#include "ScriptData.h"
#include	"tinyxml.h"
#include "Resource.h"
#include "Setup.h"
//#include "avs/ImageCanvas.h"

extern HWND				hScpWnd;
extern WindowsSetupData	g_SetupData;

ScriptData		g_ScriptData;

ScriptTool::ScriptTool()
{
}

ScriptTool::~ScriptTool()
{
}

void ScriptTool::Create(HINSTANCE hInst, HWND hWnd)
{
	OPENFILENAME	ofn;
	TCHAR		fname_full[MAX_PATH] = L"";   // ファイル名(フルパス)を受け取る領域
	TCHAR		fname[MAX_PATH];        // ファイル名を受け取る領域

	// 構造体に情報をセット
	ZeroMemory(&ofn, sizeof(ofn));					// 最初にゼロクリアしておく
	ofn.lStructSize		= sizeof(ofn);				// 構造体のサイズ
	ofn.hwndOwner		= hWnd;						// コモンダイアログの親ウィンドウハンドル
	ofn.lpstrFilter		= L"Script(*.*)\0*.*\0\0";	// ファイルの種類
	ofn.lpstrFile		= fname_full;				// 選択されたファイル名(フルパス)を受け取る変数のアドレス
	ofn.lpstrFileTitle	= fname;					// 選択されたファイル名を受け取る変数のアドレス
	ofn.nMaxFile		= MAX_PATH;					// lpstrFileに指定した変数のサイズ
	ofn.nMaxFileTitle	= MAX_PATH;					// lpstrFileTitleに指定した変数のサイズ
	ofn.Flags			= OFN_FILEMUSTEXIST;		// フラグ指定
	ofn.lpstrTitle		= L"ファイルを開く";		// コモンダイアログのキャプション
	ofn.lpstrDefExt		= L"Script";				// デフォルトのファイルの種類
	// 初期フォルダの指定
	ofn.lpstrInitialDir = g_SetupData.dataPath;
	// ファイルを開くコモンダイアログを作成
	if(!GetOpenFileName(&ofn)) return;
	// ファイルの読み込み
	g_ScriptData.LoadFile(fname);

	// 既に表示されているか？
	if (IsWindow(hScpWnd) == 0){
		// されていないならダイアログ作成
		hScpWnd = CreateDialog(hInst, L"ScriptTool", hWnd, (DLGPROC)Proc);
	}

}

// プロシージャ
LRESULT CALLBACK ScriptTool::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) {
	case WM_INITDIALOG:
		return FALSE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			hScpWnd = NULL;
			EndDialog(hDlg, IDCANCEL);
			DestroyWindow(hDlg);
			hDlg = NULL;
			return FALSE;
		case IDC_BUTTON1: // 「実行」ボタン
			return FALSE;
		}
		return FALSE;
	}

	return FALSE;
}

// ScriptTool更新
void ScriptTool::Update(HWND hDlg)
{
	/* スクリプト */
	g_ScriptData.Proc();

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
}
