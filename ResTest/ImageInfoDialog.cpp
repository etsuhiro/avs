#include "stdafx.h"
#include "ImageInfoDialog.h"
#include <commctrl.h>
#include "resource.h"
#include "AvsCore/ImageListener.h"

namespace {
	// 整数値のコントロール
	int EditBoxInt(HWND hDlg, int nIDDlgItem, int param)
	{
		BOOL translated = FALSE;
		int value = (int)GetDlgItemInt(hDlg, nIDDlgItem, &translated, TRUE);
		if (translated == TRUE)
			param = value;
		else
			SetDlgItemInt(hDlg, nIDDlgItem, param, TRUE);
		return param;
	}
	float EditBoxInt(HWND hDlg, int nIDDlgItem, float param)
	{
		return (float)EditBoxInt(hDlg, nIDDlgItem, (int)param);
	}

	void SetDlgItemFloat(HWND hDlg, int nIDDlgItem, float param)
	{
		TCHAR buf[32];
		_stprintf_s(buf, _T("%g"), param);
		SetDlgItemText(hDlg, nIDDlgItem, buf);
	}

	int SpinButtonInt(HWND hDlg, int delta, int nIDDlgItem, int param)
	{
		if (delta < 0){
			param++;
		}
		else {
			param--;
		}
		SetDlgItemInt(hDlg, nIDDlgItem, param, TRUE);
		return param;
	}
	float SpinButtonInt(HWND hDlg, int delta, int nIDDlgItem, float param)
	{
		if (delta < 0){
			param+=1.f;
		}
		else {
			param-=1.f;
		}
		SetDlgItemFloat(hDlg, nIDDlgItem, param);
		return param;
	}

	// float値のコントロール
	float EditBoxFloat(HWND hDlg, int nIDDlgItem, float param)
	{
		TCHAR buf[32];
		BOOL translated = FALSE;
		int result = GetDlgItemText(hDlg, nIDDlgItem, buf, sizeof(buf));
		if (result > 0){
			TCHAR* endptr;
			float value = _tcstof(buf, &endptr);
			if (buf != endptr){
				param = value;
			}
			else {
				SetDlgItemFloat(hDlg, nIDDlgItem, param);
			}
		}
		return param;
	}

	float EditBoxFloat(HWND hDlg, int nIDDlgItem, float param, float min, float max)
	{
		float value = EditBoxFloat(hDlg, nIDDlgItem, param);
		if (value < min){
			value = min;
			SetDlgItemFloat(hDlg, nIDDlgItem, value);
		}
		if (value > max){
			value = max;
			SetDlgItemFloat(hDlg, nIDDlgItem, value);
		}
		return value;
	}
}

void ImageInfoDialog::Create(HINSTANCE hInst, HWND hWndParent)
{
	// モードレスダイアログボックスを作成します
	CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWndParent, (DLGPROC)Proc);
}

// プロシージャ
LRESULT CALLBACK ImageInfoDialog::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static struct avs::ImageInfo image = {};
	static TV_INSERTSTRUCT tv{};

	switch (msg) {
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));

		SetDlgItemInt(hDlg, IDC_EDIT1, (int)image.x, TRUE);							// X : 表示座標の X 値
		SetDlgItemInt(hDlg, IDC_EDIT2, (int)image.y, TRUE);							// Y : 表示座標の Y 値
		SetDlgItemInt(hDlg, IDC_EDIT3, (int)image.u, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT4, (int)image.v, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT5, (int)image.w, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT6, (int)image.h, TRUE);
		SetDlgItemFloat(hDlg, IDC_EDIT7, image.hscale);
		SetDlgItemFloat(hDlg, IDC_EDIT8, image.vscale);
		SetDlgItemFloat(hDlg, IDC_EDIT9, image.r);
		SetDlgItemFloat(hDlg, IDC_EDIT10, image.g);
		SetDlgItemFloat(hDlg, IDC_EDIT11, image.b);
		SetDlgItemFloat(hDlg, IDC_EDIT12, image.a);
		SetDlgItemFloat(hDlg, IDC_EDIT13, image.rot);
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.r * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.g * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.b * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.a * 100));
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);	// Undoボタンを選択不可に
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
			// どのアイテムが変更されたか
			switch (LOWORD(wParam)) {
			case IDC_EDIT1:
				image.x = EditBoxInt(hDlg, IDC_EDIT1, image.x);
				return TRUE;
			case IDC_EDIT2:
				image.y = EditBoxInt(hDlg, IDC_EDIT2, image.y);
				return TRUE;
			case IDC_EDIT3:
				image.u = EditBoxInt(hDlg, IDC_EDIT3, image.u);
				return TRUE;
			case IDC_EDIT4:
				image.v = EditBoxInt(hDlg, IDC_EDIT4, image.v);
				return TRUE;
			case IDC_EDIT5:
				image.w = EditBoxInt(hDlg, IDC_EDIT5, image.w);
				return TRUE;
			case IDC_EDIT6:
				image.h = EditBoxInt(hDlg, IDC_EDIT6, image.h);
				return TRUE;
			case IDC_EDIT7:
				image.hscale = EditBoxFloat(hDlg, IDC_EDIT7, image.hscale);
				return TRUE;
			case IDC_EDIT8:
				image.vscale = EditBoxFloat(hDlg, IDC_EDIT8, image.vscale);
				return TRUE;
			case IDC_EDIT9:
				image.r = EditBoxFloat(hDlg, IDC_EDIT9, image.r, 0.f, 1.f);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.r * 100));
				return TRUE;
			case IDC_EDIT10:
				image.g = EditBoxFloat(hDlg, IDC_EDIT10, image.g, 0.f, 1.f);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.g * 100));
				return TRUE;
			case IDC_EDIT11:
				image.b = EditBoxFloat(hDlg, IDC_EDIT11, image.b, 0.f, 1.f);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.b * 100));
				return TRUE;
			case IDC_EDIT12:
				image.a = EditBoxFloat(hDlg, IDC_EDIT12, image.a, 0.f, 1.f);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.a * 100));
				return TRUE;
			case IDC_EDIT13:
				image.rot = EditBoxFloat(hDlg, IDC_EDIT13, image.rot);
				return TRUE;
			}
		}
		break;

	case WM_HSCROLL:	// バーを動かしてる時
		image.r = (float)(SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_GETPOS, 0, 0)) / 100;
		image.g = (float)(SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_GETPOS, 0, 0)) / 100;
		image.b = (float)(SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_GETPOS, 0, 0)) / 100;
		image.a = (float)(SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_GETPOS, 0, 0)) / 100;
		SetDlgItemFloat(hDlg, IDC_EDIT9, image.r);
		SetDlgItemFloat(hDlg, IDC_EDIT10, image.g);
		SetDlgItemFloat(hDlg, IDC_EDIT11, image.b);
		SetDlgItemFloat(hDlg, IDC_EDIT12, image.a);
		return TRUE;

	case WM_NOTIFY:
		if (wParam == IDC_TREE1)
		{
			TV_DISPINFO* ptv_disp = (TV_DISPINFO *)lParam;
			if (ptv_disp->hdr.code == TVN_SELCHANGING){
				// 項目が選択された
				return TRUE;
			}
			if (ptv_disp->hdr.code == TVN_ENDLABELEDIT)
			{
				// ツリーの項目編集が終わったのでデータをセット
				TreeView_SetItem(GetDlgItem(hDlg, IDC_TREE1), &ptv_disp->item);
				return TRUE;
			}
		} else {
			LPNMHDR nhm = (NMHDR*)lParam;
			NM_UPDOWN* nmud = (NM_UPDOWN*)nhm;

			switch (nhm->code){
			case UDN_DELTAPOS:	// スピンボタンを押した時
				switch (nmud->hdr.idFrom){
				case IDC_SPIN1:
					image.x = SpinButtonInt(hDlg, nmud->iDelta, IDC_EDIT1, image.x);
					return TRUE;
				case IDC_SPIN2:
					image.y = SpinButtonInt(hDlg, nmud->iDelta, IDC_EDIT2, image.y);
					return TRUE;
				case IDC_SPIN3:
					image.u = SpinButtonInt(hDlg, nmud->iDelta, IDC_EDIT3, image.u);
					return TRUE;
				case IDC_SPIN4:
					image.v = SpinButtonInt(hDlg, nmud->iDelta, IDC_EDIT4, image.v);
					return TRUE;
				case IDC_SPIN5:
					image.w = SpinButtonInt(hDlg, nmud->iDelta, IDC_EDIT5, image.w);
					return TRUE;
				case IDC_SPIN6:
					image.h = SpinButtonInt(hDlg, nmud->iDelta, IDC_EDIT6, image.h);
					return TRUE;
				case IDC_SPIN13:
					image.rot = SpinButtonInt(hDlg, nmud->iDelta, IDC_EDIT13, image.rot);
					return TRUE;
				}
			}
		}
		break;
	}

	return FALSE;
}

