#include "stdafx.h"
#include "ImageTreeView.h"
#include <commctrl.h>
#include "resource.h"
#include "AvsCore/ImageListener.h"
#include "ImageInfoDialog.h"

void ImageTreeView::Create(HINSTANCE hInst, HWND hWndParent)
{
	// ���[�h���X�_�C�A���O�{�b�N�X���쐬���܂�
	CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWndParent, (DLGPROC)Proc);
}

// �v���V�[�W��
LRESULT CALLBACK ImageTreeView::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static TV_INSERTSTRUCT tv{};
	static ImageInfoDialog* pImageInfoDialog{};
	static avs::ImageInfo image[4];

	switch (msg) {
	case WM_INITDIALOG:
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

			if (pImageInfoDialog)	delete pImageInfoDialog;
			pImageInfoDialog = new ImageInfoDialog(nullptr);
//			pImageInfoDialog->Init(hDlg);
	}
		break;

	case WM_DROPFILES:
		break;

	case WM_NOTIFY:
		if (wParam == IDC_TREE1)
		{
			TV_DISPINFO* ptv_disp = (TV_DISPINFO *)lParam;
			if (ptv_disp->hdr.code == TVN_SELCHANGING){
				// ���ڂ��I�����ꂽ
				if (pImageInfoDialog)	delete pImageInfoDialog;
				pImageInfoDialog = new ImageInfoDialog(&image[0]);
				pImageInfoDialog->Init(hDlg);
				return TRUE;
			}
			if (ptv_disp->hdr.code == TVN_ENDLABELEDIT)
			{
				// �c���[�̍��ڕҏW���I������̂Ńf�[�^���Z�b�g
				TreeView_SetItem(GetDlgItem(hDlg, IDC_TREE1), &ptv_disp->item);
				return TRUE;
			}
		}
		break;
	}

	return (pImageInfoDialog)? pImageInfoDialog->Proc(hDlg, msg, wParam, lParam) : FALSE;
}
