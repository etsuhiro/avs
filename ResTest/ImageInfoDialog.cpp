#include "stdafx.h"
#include "ImageInfoDialog.h"
#include <commctrl.h>
#include "resource.h"
#include "AvsCore/ImageListener.h"

namespace {
	// �����l�̃R���g���[��
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
		return (float)SpinButtonInt(hDlg, delta, nIDDlgItem, (int)param);
	}

	// float�l�̃R���g���[��
	float EditBoxFloat(HWND hDlg, int nIDDlgItem, float param)
	{
		TCHAR buf[32];
		BOOL translated = FALSE;
		int result = GetDlgItemText(hDlg, nIDDlgItem, buf, sizeof(buf));
		if (result > 0){
			char str[32];
			WideCharToMultiByte(CP_OEMCP, 0, buf, result, str, sizeof(str), NULL, NULL);
			param = (float)atof(str);
		}
		else {
			swprintf(buf, 256, L"%.2f", param);
			SetDlgItemText(hDlg, nIDDlgItem, buf);
		}
		return param;
	}
}

void ImageInfoDialog::Create(HINSTANCE hInst, HWND hWndParent)
{
	// ���[�h���X�_�C�A���O�{�b�N�X���쐬���܂�
	CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWndParent, (DLGPROC)Proc);
}

// �v���V�[�W��
LRESULT CALLBACK ImageInfoDialog::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static struct avs::ImageInfo image = {};
	static TV_INSERTSTRUCT tv{};

	switch (msg) {
	case WM_INITDIALOG:
		SetDlgItemInt(hDlg, IDC_EDIT1, (int)image.x, TRUE);							// X : �\�����W�� X �l
		SetDlgItemInt(hDlg, IDC_EDIT2, (int)image.y, TRUE);							// Y : �\�����W�� Y �l
		{
			HWND hWnd = GetDlgItem(hDlg, IDC_TREE1);
			tv.hInsertAfter = TVI_LAST;
			tv.item.mask = TVIF_TEXT;
			tv.hParent = TVI_ROOT;
			tv.item.pszText = TEXT("�e�P");
			TreeView_InsertItem(hWnd, &tv);
			tv.item.pszText = TEXT("�e�Q");
			HTREEITEM hParent = TreeView_InsertItem(hWnd, &tv);
			tv.hParent = hParent;
			tv.item.pszText = TEXT("�q�P");
			TreeView_InsertItem(hWnd, &tv);
			tv.item.pszText = TEXT("�q�Q");
			TreeView_InsertItem(hWnd, &tv);
		}
		break;

	case WM_DROPFILES:
		break;

	case WM_COMMAND:
		// �A�C�e���̓��e���ύX���ꂽ��
		if (HIWORD(wParam) == EN_UPDATE){
			// �ǂ̃A�C�e�����ύX���ꂽ��
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
			case IDC_EDIT12:
				image.a = EditBoxFloat(hDlg, IDC_EDIT12, image.a);
				return TRUE;
			}
		}
		break;

	case WM_NOTIFY:
	{
		LPNMHDR nhm = (NMHDR*)lParam;
		NM_UPDOWN* nmud = (NM_UPDOWN*)nhm;

		switch (nhm->code){
		case UDN_DELTAPOS:	// �X�s���{�^������������
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
			}
		}
	}
	break;

	}

	return FALSE;
}

