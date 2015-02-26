#include "stdafx.h"
#include "ImageInfoDialog.h"
#include <commctrl.h>
#include "resource.h"

void ImageInfoDialog::Create(HINSTANCE hInst, HWND hWndParent)
{
	// モードレスダイアログボックスを作成します
	CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWndParent, (DLGPROC)Proc);
}

// プロシージャ
LRESULT CALLBACK ImageInfoDialog::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static int x = 0;
	static int y = 0;
	static TV_INSERTSTRUCT tv{};

	switch (msg) {
	case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_EDIT1, x, TRUE);							// X : 表示座標の X 値
		SetDlgItemInt(hDlg, IDC_EDIT2, y, TRUE);							// Y : 表示座標の Y 値
		{
			HWND hWnd = GetDlgItem(hDlg, IDC_TREE1);
			tv.hInsertAfter = TVI_LAST;
			tv.item.mask = TVIF_TEXT;
			tv.hParent = TVI_ROOT;
			tv.item.pszText = TEXT("親１");
			TreeView_InsertItem(hWnd, &tv);
			tv.item.pszText = TEXT("親２");
			HTREEITEM hParent = TreeView_InsertItem(hWnd, &tv);
			tv.hParent = hParent;
			tv.item.pszText = TEXT("子１");
			TreeView_InsertItem(hWnd, &tv);
			tv.item.pszText = TEXT("子２");
			TreeView_InsertItem(hWnd, &tv);
		}
		break;

	case WM_DROPFILES:
		break;

	case WM_COMMAND:
		// アイテムの内容が変更された時
		if (HIWORD(wParam) == EN_UPDATE){
			BOOL translated = FALSE;
			int value = 0;
			// どのアイテムが変更されたか
			switch (LOWORD(wParam)) {
			case IDC_EDIT1: // X
				value = (int)GetDlgItemInt(hDlg, IDC_EDIT1, &translated, TRUE);
				if (translated == TRUE) x = value;
				else
					SetDlgItemInt(hDlg, IDC_EDIT1, x, TRUE);
				return TRUE;
				break;

			case IDC_EDIT2: // Y
				value = (int)GetDlgItemInt(hDlg, IDC_EDIT2, &translated, TRUE);
				if (translated == TRUE) y = value;
				return TRUE;
				break;
			}
		}
		break;

	case WM_NOTIFY:
	{
		LPNMHDR nhm = (NMHDR*)lParam;
		NM_UPDOWN* nmud = (NM_UPDOWN*)nhm;

		switch (nhm->code){
		case UDN_DELTAPOS:	// スピンボタンを押した時
			switch (nmud->hdr.idFrom){
			case IDC_SPIN1:		// X
				if (nmud->iDelta < 0){
					x++;
				}
				else {
					x--;
				}
				SetDlgItemInt(hDlg, IDC_EDIT1, x, TRUE);
				break;
			case IDC_SPIN2:		// Y
				if (nmud->iDelta < 0){
					y++;
				}
				else {
					y--;
				}
				SetDlgItemInt(hDlg, IDC_EDIT2, y, TRUE);
				break;
			}
		}
	}
	break;

	}

	return FALSE;
}
