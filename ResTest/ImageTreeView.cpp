#include "stdafx.h"
#include "ImageTreeView.h"
#include <commctrl.h>
#include "resource.h"
#include "AvsCore/ImageListener.h"

// プロシージャ
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
		}
		break;
	}

	return FALSE;
}
