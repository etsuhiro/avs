/*! @file LayerTool.cpp */
#include <d3d9.h>
#include <d3dx9tex.h>
#include "Resource.h"
#include "Setup.h"
#include "image/ImageManager.h"


#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

#include "LayerTool.h"

using namespace image;

extern WindowsSetupData	g_SetupData;

LayerTool* LayerTool::pInstance = 0;

LayerTool::LayerTool()
	: m_hWnd(0)
	, m_moveMode(false)
{
}

LayerTool::~LayerTool()
{
}

void LayerTool::Create(HINSTANCE hInst, HWND hWndParent)
{
	if (pInstance==0){
		pInstance = new LayerTool;
		pInstance->m_hWnd = CreateDialog(hInst, L"LayerTool", hWndParent, (DLGPROC)Proc);
		if (pInstance->m_hWnd==0){
			// 作成失敗
			delete pInstance;
			pInstance = 0;
		}
	}
}

void LayerTool::Update()
{
}

void LayerTool::appendHistory(WCHAR* path)
{
	std::wstring str(path);
	m_history.remove(str);
	m_history.push_front(str);
	if (m_history.size()==11){
		m_history.pop_back();
	}
}

#if 0
void LayerTool::setHistory(HWND hDlg)
{
	int i=0;
	for (std::list<std::wstring>::iterator it = m_history.begin(); it != m_history.end(); it++){
		SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_INSERTSTRING, (WPARAM)i++, (LPARAM)((*it).c_str()));
	}
}
#endif

// プロシージャ
LRESULT CALLBACK LayerTool::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static bool		comboFocus;	// 使途不明
	static HWND		hTab;
	static HFONT	hFont;
	char			str[256];
	TCHAR			str_t[256];
	TCITEM			ti;

	// 現在のタブ番号取得
	int num = TabCtrl_GetCurSel(GetDlgItem(hDlg, IDC_TAB1));
	if (num < 0) num = 0;

//	if (pImgMan->ImgTag(num).LoadFlg) return FALSE;

	switch (msg) {
	case WM_INITDIALOG:
		DragAcceptFiles(hDlg, TRUE);
		hTab = GetDlgItem(hDlg, IDC_TAB1);
		memset(&ti, 0, sizeof(TCITEM));
		ti.mask = TCIF_TEXT;
		for (int i=0;i<g_SetupData.nLayer;i++){
			sprintf_s(str, "Layer%d", i);
			memset( str_t, 0x0, sizeof(str_t) );
			MultiByteToWideChar( CP_OEMCP, MB_PRECOMPOSED, str, strlen(str), str_t, sizeof(str_t)/2 );
			ti.pszText = str_t;
			TabCtrl_InsertItem(hTab, i, &ti);
		}
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 255));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 255));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 255));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
//		SendMessage(hTab, WM_SETFONT, (WPARAM)hFont, (LPARAM)MAKELONG(TRUE, 0));
		TabCtrl_SetCurSel(hTab, num);
		// 初期値をセット
		StatusSet(hDlg, num);
//		if (pInstance){
//			pInstance->setHistory(hDlg);
//		}
#if 0
		for (int i=0;i<image::FILE_NAME_BACKUP;i++){
			if (SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_GETCOUNT, 0, 0) > i) break;
			memset( str_t, 0x0, sizeof(str_t) );
			MultiByteToWideChar( CP_OEMCP, MB_PRECOMPOSED, pImgMan->ImgTag(num).Backup[i], strlen(pImgMan->ImgTag(num).Backup[i]), str_t, sizeof(str_t)/2 );
			SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)str_t);
		}
#endif
		SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_SETCURSEL, 0, 0);
		return FALSE;

	case WM_DROPFILES:
		{
			TCHAR filepath[MAX_PATH];
			HDROP hDrop = (HDROP)wParam;
			num = TabCtrl_GetCurSel(hTab);
			DragQueryFile(hDrop, 0, filepath, sizeof(filepath));
//			SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM)filepath);
//			sprintf_s(pImgMan->ImgTag(num).filename, str);
			if (pImgMan->Load(num, filepath)){
				wcscpy_s(pImgMan->ImgTag(num).filename, 256, filepath);
			}
//			if (pInstance)	pInstance->appendHistory(filepath);
			if (pInstance)	pInstance->update(num);
            DragFinish(hDrop);
		}
		return FALSE;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDCANCEL:
			DeleteObject(hFont);
			EndDialog(hDlg, IDCANCEL);
			DestroyWindow(hDlg);
			delete pInstance;
			pInstance = 0;
			return FALSE;
#if 0
		case IDC_COMBO1:
			comboFocus = TRUE;
			switch(HIWORD(wParam)){
			case CBN_DROPDOWN: //コンボボックスのリストが開かれた時
//				if (pInstance){
//					pInstance->setHistory(hDlg);
//				}
				return FALSE;

			case CBN_SELCHANGE: // リストから選択された時
				{
					int idxNum = SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_GETCURSEL, 0, 0);
					int txtLen = SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_GETLBTEXTLEN, (WPARAM)idxNum, 0);
					if (txtLen != CB_ERR){
						if (SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_GETLBTEXT, (WPARAM)idxNum, (LPARAM)str_t) != CB_ERR){
							memset( str, 0x0, sizeof(str) );
							WideCharToMultiByte( CP_OEMCP, 0, str_t, sizeof(str_t), str, sizeof(str), NULL, NULL );
							sprintf_s(pImgMan->ImgTag(num).filename, str);
						}
					}
				}
				return FALSE;

			case CBN_SETFOCUS:
				comboFocus = true;
				return FALSE;

			case CBN_KILLFOCUS:
				comboFocus = false;
				return FALSE;
			}
			return FALSE;
#endif
		case IDC_BUTTON1: // 「開く」ボタン
			{
				OPENFILENAME ofn;
				TCHAR fname_full[MAX_PATH] = L"";   // ファイル名(フルパス)を受け取る領域
				TCHAR fname[MAX_PATH];        // ファイル名を受け取る領域
				// 構造体に情報をセット
				ZeroMemory( &ofn, sizeof(ofn) );				// 最初にゼロクリアしておく
				ofn.lStructSize		= sizeof(ofn);				// 構造体のサイズ
				ofn.hwndOwner		= hDlg;						// コモンダイアログの親ウィンドウハンドル
				ofn.lpstrFilter		= L"png(*.png)\0*.png\0\0";	// ファイルの種類
				ofn.lpstrFile		= fname_full;				// 選択されたファイル名(フルパス)を受け取る変数のアドレス
				ofn.lpstrFileTitle	= fname;	// 選択されたファイル名を受け取る変数のアドレス
				ofn.nMaxFile		= MAX_PATH;		// lpstrFileに指定した変数のサイズ
				ofn.nMaxFileTitle	= MAX_PATH;			// lpstrFileTitleに指定した変数のサイズ
				ofn.Flags			= OFN_FILEMUSTEXIST;		// フラグ指定
				ofn.lpstrTitle		= L"ファイルを開く";		// コモンダイアログのキャプション
				ofn.lpstrDefExt		= L"png";					// デフォルトのファイルの種類
				// 初期フォルダの指定
				ofn.lpstrInitialDir = g_SetupData.dataPath;

				// ファイルを開くコモンダイアログを作成
				if(!GetOpenFileName(&ofn)) return FALSE;

				if (pImgMan->Load(num, fname_full)){
					wcscpy_s(pImgMan->ImgTag(num).filename, 256, fname);
				}
//				if (pInstance)	pInstance->appendHistory(fname_full);
				if (pInstance)	pInstance->update(num);
//				WideCharToMultiByte( CP_OEMCP, 0, fname_full, -1, pImgMan->ImgTag(num).filename, MAX_PATH, NULL, NULL );
			}
			return FALSE;

		case IDC_BUTTON2: // 「破棄」ボタン
			pImgMan->ImgTag(num).filename[0] = 0;
			SetDlgItemText(hDlg, IDC_COMBO1, L"");
			SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_SETCURSEL, -1, 0);
			return FALSE;
		}

		// アイテムの内容が変更された時
		if (HIWORD(wParam) == EN_UPDATE){
			BOOL txtCheck = false;
			int box = 0;
			// どのアイテムが変更されたか
			switch (LOWORD(wParam)) {
			case IDC_EDIT1: // X
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT1, &txtCheck, TRUE);
				if (txtCheck && pImgMan->ImgTag(num).x != box) pImgMan->ImgTag(num).x = box;
				break;

			case IDC_EDIT2: // Y
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT2, &txtCheck, TRUE);
				if (txtCheck && pImgMan->ImgTag(num).y != box) pImgMan->ImgTag(num).y = box;
				break;

			case IDC_EDIT3: // U
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT3, &txtCheck, FALSE);
				if (txtCheck && pImgMan->ImgTag(num).u != box) pImgMan->ImgTag(num).u = box;
				break;

			case IDC_EDIT4: // V
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT4, &txtCheck, FALSE);
				if (txtCheck && pImgMan->ImgTag(num).v != box) pImgMan->ImgTag(num).v = box;
				break;

			case IDC_EDIT5: // W
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT5, &txtCheck, TRUE);
				if (txtCheck && pImgMan->ImgTag(num).w != box) pImgMan->ImgTag(num).w = box;
				break;

			case IDC_EDIT6: // H
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT6, &txtCheck, TRUE);
				if (txtCheck && pImgMan->ImgTag(num).h != box) pImgMan->ImgTag(num).h = box;
				break;

			case IDC_EDIT7: // X倍率
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT7, &txtCheck, FALSE);
				if (txtCheck && pImgMan->ImgTag(num).mag_x != box) pImgMan->ImgTag(num).mag_x = (float)box / 100.f;
				break;

			case IDC_EDIT8: // Y倍率
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT8, &txtCheck, FALSE);
				if (txtCheck && pImgMan->ImgTag(num).mag_y != box) pImgMan->ImgTag(num).mag_y = (float)box / 100.f;
				break;

			case IDC_EDIT9: // R
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT9, &txtCheck, FALSE);
				if (txtCheck){
					if (box < 0){
						pImgMan->ImgTag(num).r = 0.f;
					} else
					if (box > 255){
						pImgMan->ImgTag(num).r = 1.f;
					} else {
						pImgMan->ImgTag(num).r = (float)box / 255.f;
					}
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).r*255));
				}
				break;

			case IDC_EDIT10: // G
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT10, &txtCheck, FALSE);
				if (txtCheck){
					if (box < 0){
						pImgMan->ImgTag(num).g = 0.f;
					} else
					if (box > 255){
						pImgMan->ImgTag(num).g = 1.f;
					} else {
						pImgMan->ImgTag(num).g = (float)box / 255.f;
					}
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).g*255));
				}
				break;

			case IDC_EDIT11: // B
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT11, &txtCheck, FALSE);
				if (txtCheck){
					if (box < 0){
						pImgMan->ImgTag(num).b = 0.f;
					} else
					if (box > 255){
						pImgMan->ImgTag(num).b = 1.f;
					} else {
						pImgMan->ImgTag(num).b = (float)box / 255.f;
					}
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).b*255));
				}
				break;

			case IDC_EDIT12: // A
				{
					char kari[256];
					memset(str, 0x0, sizeof(str));
					memset(kari, 0x0, sizeof(kari));
					memset(str_t, 0x0, sizeof(str_t));

					GetDlgItemText(hDlg, IDC_EDIT12, str_t, sizeof(str_t));
					WideCharToMultiByte( CP_OEMCP, 0, str_t, sizeof(str_t), str, sizeof(str), NULL, NULL );
					sprintf_s(kari, "%.2f", pImgMan->ImgTag(num).a);

					if (strcmp(str,kari)){
						if (pImgMan->ImgTag(num).a < 0.f){
							pImgMan->ImgTag(num).a = 0.f;
						} else
						if (pImgMan->ImgTag(num).a > 1.f){
							pImgMan->ImgTag(num).a = 1.f;
						} else {
							pImgMan->ImgTag(num).a = (float)atof(str);
						}
						SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).a*100));
					}
				}
				break;
				
			case IDC_EDIT13: // RX
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT13, &txtCheck, TRUE);
				if (txtCheck && pImgMan->ImgTag(num).rot_x != box) pImgMan->ImgTag(num).rot_x = (short)(box * 0x4000 / 90);
				break;

			case IDC_EDIT14: // RY
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT14, &txtCheck, TRUE);
				if (txtCheck && pImgMan->ImgTag(num).rot_y != box) pImgMan->ImgTag(num).rot_y = (short)(box * 0x4000 / 90);
				break;

			case IDC_EDIT15: // RZ
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT15, &txtCheck, TRUE);
				if (txtCheck && pImgMan->ImgTag(num).rot_z != box) pImgMan->ImgTag(num).rot_z = (short)(box * 0x4000 / 90);
				break;

			case IDC_EDIT16: // 画角
				box = (int)GetDlgItemInt(hDlg, IDC_EDIT16, &txtCheck, TRUE);
				if (txtCheck && g_SetupData.fovy != box) g_SetupData.fovy = box;
				break;
			}
		}
		return FALSE;

    case WM_HSCROLL:	// バーを動かしてる時
		pImgMan->ImgTag(num).r = (float)(SendMessage(GetDlgItem(hDlg,IDC_SLIDER1), TBM_GETPOS, 0, 0)) / 255;
		pImgMan->ImgTag(num).g = (float)(SendMessage(GetDlgItem(hDlg,IDC_SLIDER2), TBM_GETPOS, 0, 0)) / 255;
		pImgMan->ImgTag(num).b = (float)(SendMessage(GetDlgItem(hDlg,IDC_SLIDER3), TBM_GETPOS, 0, 0)) / 255;
		pImgMan->ImgTag(num).a = (float)(SendMessage(GetDlgItem(hDlg,IDC_SLIDER4), TBM_GETPOS, 0, 0)) / 100;
		SetDlgItemInt(hDlg, IDC_EDIT9,  (UINT)(SendMessage(GetDlgItem(hDlg,IDC_SLIDER1), TBM_GETPOS, 0, 0)), FALSE);
		SetDlgItemInt(hDlg, IDC_EDIT10, (UINT)(SendMessage(GetDlgItem(hDlg,IDC_SLIDER2), TBM_GETPOS, 0, 0)), FALSE);
		SetDlgItemInt(hDlg, IDC_EDIT11, (UINT)(SendMessage(GetDlgItem(hDlg,IDC_SLIDER3), TBM_GETPOS, 0, 0)), FALSE);
		memset(str, 0x0, sizeof(str));
		memset(str_t, 0x0, sizeof(str_t));
		sprintf_s(str, "%.2f", pImgMan->ImgTag(num).a);
		MultiByteToWideChar( CP_OEMCP, MB_PRECOMPOSED, str, strlen(str), str_t, sizeof(str_t)/2 );
		SetDlgItemText(hDlg, IDC_EDIT12, str_t);
		return FALSE;

	case WM_NOTIFY:
		{
			LPNMHDR nhm = (NMHDR*)lParam;
			NM_UPDOWN* nmud = (NM_UPDOWN*)nhm;

			switch(nhm->code){
			case TCN_SELCHANGE:	// タブが切り替わった時
				// 選択中のタブ番号取得
				num = TabCtrl_GetCurSel(hTab);
				// 初期値をセット
				StatusSet(hDlg, num);
				// 履歴のセット
//				if (pInstance){
//					pInstance->setHistory(hDlg);
//				}
				SendMessage(GetDlgItem(hDlg,IDC_COMBO1), CB_SETCURSEL, 0, 0);
				return FALSE;

			case UDN_DELTAPOS:	// スピンボタンを押した時
				switch(nmud->hdr.idFrom){
				case IDC_SPIN1:		// X
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).x++;
					} else {
						pImgMan->ImgTag(num).x--;
					}
					SetDlgItemInt(hDlg, IDC_EDIT1,  pImgMan->ImgTag(num).x, TRUE);
					break;
				case IDC_SPIN2:		// Y
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).y++;
					} else {
						pImgMan->ImgTag(num).y--;
					}
					SetDlgItemInt(hDlg, IDC_EDIT2,  pImgMan->ImgTag(num).y, TRUE);
					break;
				case IDC_SPIN3:		// U
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).u++;
					} else {
						pImgMan->ImgTag(num).u--;
					}
					SetDlgItemInt(hDlg, IDC_EDIT3,  pImgMan->ImgTag(num).u, TRUE);
					break;
				case IDC_SPIN4:		// V
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).v++;
					} else {
						pImgMan->ImgTag(num).v--;
					}
					SetDlgItemInt(hDlg, IDC_EDIT4,  pImgMan->ImgTag(num).v, TRUE);
					break;
				case IDC_SPIN5:		// W
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).w++;
					} else {
						pImgMan->ImgTag(num).w--;
					}
					SetDlgItemInt(hDlg, IDC_EDIT5,  pImgMan->ImgTag(num).w, TRUE);
					break;
				case IDC_SPIN6:		// H
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).h++;
					} else {
						pImgMan->ImgTag(num).h--;
					}
					SetDlgItemInt(hDlg, IDC_EDIT6,  pImgMan->ImgTag(num).h, TRUE);
					break;
				case IDC_SPIN7:		// X倍率
					{
						/* 固定小数点(float)→整数((unsigned)int)で情報落ちするので丸める */
						int		tmp = (int)(pImgMan->ImgTag(num).mag_x*100 + .5f);

						if (nmud->iDelta < 0){
							tmp++;
						} else
						if ( --tmp < 0){ tmp = 0; }
						SetDlgItemInt(hDlg, IDC_EDIT7,  (UINT)(tmp), FALSE);
						pImgMan->ImgTag(num).mag_x = (float)(tmp/100.f);
					}
					break;
				case IDC_SPIN8:		// Y倍率
					{
						/* 固定小数点(float)→整数((unsigned)int)で情報落ちするので丸める */
						int		tmp = (int)(pImgMan->ImgTag(num).mag_y*100 + .5f);

						if (nmud->iDelta < 0){
							tmp++;
						} else
						if ( --tmp < 0){ tmp = 0; }
						SetDlgItemInt(hDlg, IDC_EDIT8,  (UINT)(tmp), FALSE);
						pImgMan->ImgTag(num).mag_y = (float)(tmp/100.f);
					}
					break;
				case IDC_SPIN9:		// R
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).r += 1.f / 255.f;
						if (pImgMan->ImgTag(num).r > 1) pImgMan->ImgTag(num).r = 1;
					} else {
						pImgMan->ImgTag(num).r -= 1.f / 255.f;
						if (pImgMan->ImgTag(num).r < 0) pImgMan->ImgTag(num).r = 0;
					}
					SetDlgItemInt(hDlg, IDC_EDIT9,  (UINT)(pImgMan->ImgTag(num).r*255), FALSE);
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).r*255));
					break;
				case IDC_SPIN10:	// G
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).g += 1.f / 255.f;
						if (pImgMan->ImgTag(num).g > 1) pImgMan->ImgTag(num).g = 1;
					} else {
						pImgMan->ImgTag(num).g -= 1.f / 255.f;
						if (pImgMan->ImgTag(num).g < 0) pImgMan->ImgTag(num).g = 0;
					}
					SetDlgItemInt(hDlg, IDC_EDIT10, (UINT)(pImgMan->ImgTag(num).g*255), FALSE);
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).g*255));
					break;
				case IDC_SPIN11:	// B
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).b += 1.f / 255.f;
						if (pImgMan->ImgTag(num).b > 1) pImgMan->ImgTag(num).b = 1;
					} else {
						pImgMan->ImgTag(num).b -= 1.f / 255.f;
						if (pImgMan->ImgTag(num).b < 0) pImgMan->ImgTag(num).b = 0;
					}
					SetDlgItemInt(hDlg, IDC_EDIT11, (UINT)(pImgMan->ImgTag(num).b*255), FALSE);
					SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).b*255));
					break;
				case IDC_SPIN12:	// A
					{
						/* 固定小数点(float)→整数((unsigned)int)で情報落ちするので丸める */
						int		tmp = (int)(pImgMan->ImgTag(num).a*100 + .5f);
						if (nmud->iDelta < 0){
							tmp++;
						} else
						if ( --tmp < 0){ tmp = 0; }
						pImgMan->ImgTag(num).a = (float)(tmp/100.f);
						if (pImgMan->ImgTag(num).a > 1.f) pImgMan->ImgTag(num).a = 1.f;
						memset(str, 0x0, sizeof(str));
						memset(str_t, 0x0, sizeof(str_t));
						sprintf_s(str, "%.2f", pImgMan->ImgTag(num).a);
						MultiByteToWideChar( CP_OEMCP, MB_PRECOMPOSED, str, strlen(str), str_t, sizeof(str_t)/2 );
						SetDlgItemText(hDlg, IDC_EDIT12, str_t);
						SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).a*100));
					}
					break;
				case IDC_SPIN13:	// RX
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).rot_x += (short)(0x4000/90);
					} else {
						pImgMan->ImgTag(num).rot_x -= (short)(0x4000/90);
					}
					SetDlgItemInt(hDlg, IDC_EDIT13, (UINT)(pImgMan->ImgTag(num).rot_x/(0x4000/90)), TRUE);
					break;
				case IDC_SPIN14:	// RY
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).rot_y += (short)(0x4000/90);
					} else {
						pImgMan->ImgTag(num).rot_y -= (short)(0x4000/90);
					}
					SetDlgItemInt(hDlg, IDC_EDIT14, (UINT)(pImgMan->ImgTag(num).rot_y/(0x4000/90)), TRUE);
					break;
				case IDC_SPIN15:	// RZ
					if (nmud->iDelta < 0){
						pImgMan->ImgTag(num).rot_z += (short)(0x4000/90);
					} else {
						pImgMan->ImgTag(num).rot_z -= (short)(0x4000/90);
					}
					SetDlgItemInt(hDlg, IDC_EDIT15, (UINT)(pImgMan->ImgTag(num).rot_z/(0x4000/90)), TRUE);
					break;
				case IDC_SPIN16:	// 画角
					if (nmud->iDelta < 0){
						if (g_SetupData.fovy < 179) g_SetupData.fovy++;
					} else {
						if (g_SetupData.fovy > 0) g_SetupData.fovy--;
					}
					SetDlgItemInt(hDlg, IDC_EDIT16, (UINT)g_SetupData.fovy, TRUE);
					break;
				}
				return FALSE;
			}

		}
		return FALSE;
	}

	return FALSE;
}

// LayerTool更新
void LayerTool::update(int num)
{
	// アイテムに値をセット
	StatusSet(m_hWnd, num);
	// 履歴
//	setHistory(m_hWnd);
	SendMessage(GetDlgItem(m_hWnd,IDC_COMBO1), CB_SETCURSEL, 0, 0);
	// LayerToolの表示を更新
	InvalidateRect(m_hWnd, NULL, TRUE);
}

// アイテムに値をセット
void LayerTool::StatusSet(HWND hDlg, int num)
{
	if	(!pImgMan){ return; }

	TCHAR	str_t[256];

	SetDlgItemText(hDlg, IDC_COMBO1, pImgMan->ImgTag(num).filename);						// ファイル名
	SetDlgItemInt(hDlg, IDC_EDIT1,  pImgMan->ImgTag(num).x, TRUE);							// X : 表示座標の X 値
	SetDlgItemInt(hDlg, IDC_EDIT2,  pImgMan->ImgTag(num).y, TRUE);							// Y : 表示座標の Y 値
	SetDlgItemInt(hDlg, IDC_EDIT3,  pImgMan->ImgTag(num).u, TRUE);							// U : 画像取得座標の X 値
	SetDlgItemInt(hDlg, IDC_EDIT4,  pImgMan->ImgTag(num).v, TRUE);							// V : 画像取得座標の Y 値
	SetDlgItemInt(hDlg, IDC_EDIT5,  pImgMan->ImgTag(num).w, TRUE);							// W : 幅
	SetDlgItemInt(hDlg, IDC_EDIT6,  pImgMan->ImgTag(num).h, TRUE);							// H : 高さ
	SetDlgItemInt(hDlg, IDC_EDIT7,  (UINT)(pImgMan->ImgTag(num).mag_x*100), FALSE);			// X倍率 : X の画像倍率
	SetDlgItemInt(hDlg, IDC_EDIT8,  (UINT)(pImgMan->ImgTag(num).mag_y*100), FALSE);			// Y倍率 : Y の画像倍率
	SetDlgItemInt(hDlg, IDC_EDIT9,  (UINT)(pImgMan->ImgTag(num).r*255), FALSE);				// R : カラー R
	SetDlgItemInt(hDlg, IDC_EDIT10, (UINT)(pImgMan->ImgTag(num).g*255), FALSE);				// G : カラー G
	SetDlgItemInt(hDlg, IDC_EDIT11, (UINT)(pImgMan->ImgTag(num).b*255), FALSE);				// B : カラー B
	SetDlgItemInt(hDlg, IDC_EDIT13, (UINT)(pImgMan->ImgTag(num).rot_x/(0x4000/90)), TRUE);	// RX : X 回転
	SetDlgItemInt(hDlg, IDC_EDIT14, (UINT)(pImgMan->ImgTag(num).rot_y/(0x4000/90)), TRUE);	// RY : Y 回転
	SetDlgItemInt(hDlg, IDC_EDIT15, (UINT)(pImgMan->ImgTag(num).rot_z/(0x4000/90)), TRUE);	// RZ : Z 回転
	SetDlgItemInt(hDlg, IDC_EDIT16, (UINT)(g_SetupData.fovy), TRUE);									// 画角
	SendMessage(GetDlgItem(hDlg,IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).r*255));	// R のスライダー
	SendMessage(GetDlgItem(hDlg,IDC_SLIDER2), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).g*255));	// G のスライダー
	SendMessage(GetDlgItem(hDlg,IDC_SLIDER3), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).b*255));	// B のスライダー
	SendMessage(GetDlgItem(hDlg,IDC_SLIDER4), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(pImgMan->ImgTag(num).a*100));	// A のスライダー
	swprintf(str_t, 256, L"%.2f", pImgMan->ImgTag(num).a);
	SetDlgItemText(hDlg, IDC_EDIT12, str_t);				// A : アルファ
}

// 画像をマウスで移動
void LayerTool::ImageMove(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (pInstance==0)	return;

	// 在のレイヤー番号の取得
//	for (int nowTabNum = g_SetupData.nLayer-1; nowTabNum>=0; --nowTabNum){
	int nowTabNum = TabCtrl_GetCurSel(GetDlgItem(pInstance->m_hWnd, IDC_TAB1));
	avs::ImageTag& imtag = pImgMan->ImgTag(nowTabNum);

	// マウスの位置取得
	POINT	nowPos;
	nowPos.x = LOWORD(lParam) - g_SetupData.width / 2;
	nowPos.y = HIWORD(lParam) - g_SetupData.height / 2;

	if (!pInstance->m_moveMode){
		if (msg==WM_RBUTTONDOWN){
			// クリック位置とレイヤー位置がかぶってたら
			if ((imtag.x - abs(imtag.w)/2) <= nowPos.x
			 && (imtag.x + abs(imtag.w)/2) >= nowPos.x
			 && (imtag.y - abs(imtag.h)/2) <= nowPos.y
			 && (imtag.y + abs(imtag.h)/2) >= nowPos.y){
				pInstance->m_moveMode = true;
				pInstance->mouseStartPos = nowPos;
				pInstance->layerStartPos.x = imtag.x;
				pInstance->layerStartPos.y = imtag.y;
			}
		}
	} else {
		// 右クリック中なら
		if (wParam & MK_RBUTTON){
			if (nowPos.x < -(g_SetupData.width/2)) nowPos.x = -(g_SetupData.width/2);
			if (nowPos.x > (g_SetupData.width/2)) nowPos.x = (g_SetupData.width/2);
			if (nowPos.y < -(g_SetupData.height/2)) nowPos.y = -(g_SetupData.height/2);
			if (nowPos.y > (g_SetupData.height/2)) nowPos.y = (g_SetupData.height/2);
			// 動かした分をセット
			imtag.x = (short)(pInstance->layerStartPos.x-(pInstance->mouseStartPos.x-nowPos.x));
			imtag.y = (short)(pInstance->layerStartPos.y-(pInstance->mouseStartPos.y-nowPos.y));
			// LayerToolの x と y の更新
			SetDlgItemInt(pInstance->m_hWnd, IDC_EDIT1, imtag.x, TRUE);
			SetDlgItemInt(pInstance->m_hWnd, IDC_EDIT2, imtag.y, TRUE);
			InvalidateRect(GetDlgItem(pInstance->m_hWnd,IDC_EDIT1), NULL, TRUE);
			InvalidateRect(GetDlgItem(pInstance->m_hWnd,IDC_EDIT2), NULL, TRUE);
		} else {
			pInstance->m_moveMode = false;
		}
	}
}
