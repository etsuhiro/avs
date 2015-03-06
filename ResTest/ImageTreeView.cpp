#include "stdafx.h"
#include "ImageTreeView.h"
#include <commctrl.h>
#include "resource.h"
#include "AvsCore/ImageListener.h"

// �v���V�[�W��
LRESULT CALLBACK ImageTreeView::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static TV_INSERTSTRUCT tv{};

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

	return FALSE;
}
