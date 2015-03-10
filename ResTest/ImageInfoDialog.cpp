#include "stdafx.h"
#include "ImageInfoDialog.h"
#include <commctrl.h>
#include "resource.h"

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

void modyfyCheck(HWND hDlg, avs::ImageInfo& image, avs::ImageInfo& org)
{
	if (image.x != org.x
	 || image.r != org.r)
		EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), TRUE);	// Undoボタンを選択可に
}

ImageInfoDialog::ImageInfoDialog(avs::ImageInfo* info)
	: m_pInfo(info)
{}

void ImageInfoDialog::Init(HWND hDlg)
{
	if (m_pInfo){
		m_orgInfo = *m_pInfo;
		SetDlgItemInt(hDlg, IDC_EDIT1, (int)m_pInfo->x, TRUE);							// X : 表示座標の X 値
		SetDlgItemInt(hDlg, IDC_EDIT2, (int)m_pInfo->y, TRUE);							// Y : 表示座標の Y 値
		SetDlgItemInt(hDlg, IDC_EDIT3, (int)m_pInfo->u, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT4, (int)m_pInfo->v, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT5, (int)m_pInfo->w, TRUE);
		SetDlgItemInt(hDlg, IDC_EDIT6, (int)m_pInfo->h, TRUE);
		SetDlgItemFloat(hDlg, IDC_EDIT7, m_pInfo->hscale);
		SetDlgItemFloat(hDlg, IDC_EDIT8, m_pInfo->vscale);
		SetDlgItemFloat(hDlg, IDC_EDIT9, m_pInfo->r);
		SetDlgItemFloat(hDlg, IDC_EDIT10, m_pInfo->g);
		SetDlgItemFloat(hDlg, IDC_EDIT11, m_pInfo->b);
		SetDlgItemFloat(hDlg, IDC_EDIT12, m_pInfo->a);
		SetDlgItemFloat(hDlg, IDC_EDIT13, m_pInfo->rot);
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(m_pInfo->r * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(m_pInfo->g * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(m_pInfo->b * 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(m_pInfo->a * 100));
	}
	else {
		SetDlgItemText(hDlg, IDC_EDIT1, L"");
		SetDlgItemText(hDlg, IDC_EDIT2, L"");
		SetDlgItemText(hDlg, IDC_EDIT3, L"");
		SetDlgItemText(hDlg, IDC_EDIT4, L"");
		SetDlgItemText(hDlg, IDC_EDIT5, L"");
		SetDlgItemText(hDlg, IDC_EDIT6, L"");
		SetDlgItemText(hDlg, IDC_EDIT7, L"");
		SetDlgItemText(hDlg, IDC_EDIT8, L"");
		SetDlgItemText(hDlg, IDC_EDIT9, L"");
		SetDlgItemText(hDlg, IDC_EDIT10, L"");
		SetDlgItemText(hDlg, IDC_EDIT11, L"");
		SetDlgItemText(hDlg, IDC_EDIT12, L"");
		SetDlgItemText(hDlg, IDC_EDIT13, L"");
		EnableWindow(GetDlgItem(hDlg, IDC_SLIDER1), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_SLIDER2), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_SLIDER3), FALSE);
		EnableWindow(GetDlgItem(hDlg, IDC_SLIDER4), FALSE);
	}
	EnableWindow(GetDlgItem(hDlg, IDC_BUTTON1), FALSE);	// Undoボタンを選択不可に
}

// プロシージャ
LRESULT ImageInfoDialog::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (m_pInfo == nullptr)	return FALSE;
	avs::ImageInfo& image = *m_pInfo;

	switch (msg) {
	case WM_INITDIALOG:
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
		SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_SETRANGE, (WPARAM)TRUE, (LPARAM)MAKELONG(0, 100));
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
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT2:
				image.y = EditBoxInt(hDlg, IDC_EDIT2, image.y);
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT3:
				image.u = EditBoxInt(hDlg, IDC_EDIT3, image.u);
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT4:
				image.v = EditBoxInt(hDlg, IDC_EDIT4, image.v);
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT5:
				image.w = EditBoxInt(hDlg, IDC_EDIT5, image.w);
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT6:
				image.h = EditBoxInt(hDlg, IDC_EDIT6, image.h);
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT7:
				image.hscale = EditBoxFloat(hDlg, IDC_EDIT7, image.hscale);
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT8:
				image.vscale = EditBoxFloat(hDlg, IDC_EDIT8, image.vscale);
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT9:
				image.r = EditBoxFloat(hDlg, IDC_EDIT9, image.r, 0.f, 1.f);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER1), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.r * 100));
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT10:
				image.g = EditBoxFloat(hDlg, IDC_EDIT10, image.g, 0.f, 1.f);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER2), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.g * 100));
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT11:
				image.b = EditBoxFloat(hDlg, IDC_EDIT11, image.b, 0.f, 1.f);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER3), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.b * 100));
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT12:
				image.a = EditBoxFloat(hDlg, IDC_EDIT12, image.a, 0.f, 1.f);
				SendMessage(GetDlgItem(hDlg, IDC_SLIDER4), TBM_SETPOS, (WPARAM)TRUE, (LPARAM)(image.a * 100));
				modyfyCheck(hDlg, image, m_orgInfo);
				return TRUE;
			case IDC_EDIT13:
				image.rot = EditBoxFloat(hDlg, IDC_EDIT13, image.rot);
				modyfyCheck(hDlg, image, m_orgInfo);
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
	{
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

