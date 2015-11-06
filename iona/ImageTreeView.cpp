#include "stdafx.h"
#include "ImageTreeView.h"
#include <windowsx.h>
#include <commctrl.h>
#include "resource.h"
#include "AvsCore/ImageListener.h"

#pragma comment( lib, "Comctl32.lib" )

void ImageTreeView::Create(HINSTANCE hInst, HWND hWndParent)
{
	// モードレスダイアログボックスを作成します
//	CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWndParent, (DLGPROC)Proc);
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWndParent, (DLGPROC)Proc);
}

BOOL g_fdragging = FALSE;
static HTREEITEM    hDragItem;

BOOL begin_drag(HWND h_tree, LPNMTREEVIEW p_nmtree)
{
#if 0
	//Create an image list that holds our drag image
	auto h_drag = TreeView_CreateDragImage(h_tree, p_nmtree->itemNew.hItem);
	//begin the drag operation
	ImageList_BeginDrag(h_drag, 0, 0, 0);
	//hide the cursor
	ShowCursor(FALSE);
	//capture the mouse
	SetCapture(GetParent(h_tree));
	//set global flag to indicate we are in the middle of a drag operation
	g_fdragging = TRUE;
	//convert coordinates to screen coordinates
	ClientToScreen(h_tree, &(p_nmtree->ptDrag));
	//paint our drag image and lock the screen.
	ImageList_DragEnter(NULL, p_nmtree->ptDrag.x, p_nmtree->ptDrag.y);
#else
	NM_TREEVIEW *lpnmtv;
	int         level;
	UINT        xIndent;
	RECT        rcItem;
	HTREEITEM   htItem;

	lpnmtv = (NM_TREEVIEW *)p_nmtree;

	SetCapture(GetParent(h_tree));   // マウスキャプチャ開始

	g_fdragging = TRUE;             // ドラッグ中のフラグ

	// ドラッグアイテムのハンドルを取得
	hDragItem = lpnmtv->itemNew.hItem;

	// ドラッグアイテムを選択する
	TreeView_SelectItem(h_tree, hDragItem);

	// ドラッグの画像を用意する
	HIMAGELIST hIml_DD;

	hIml_DD = TreeView_CreateDragImage(h_tree, hDragItem);
	//ドラッグ画像の表示位置を求める計算
	htItem = hDragItem;
	level = 0;
	do{
		htItem = TreeView_GetParent(h_tree, htItem);
		level++;
	} while (htItem);
	xIndent = TreeView_GetIndent(h_tree);
	TreeView_GetItemRect(h_tree, hDragItem, &rcItem, FALSE);

	// ドラッグ画像を表示する
	ImageList_BeginDrag(hIml_DD, 0, lpnmtv->ptDrag.x - rcItem.left -
		xIndent*level, lpnmtv->ptDrag.y - rcItem.top);
#endif
	return TRUE;

}

// プロシージャ
LRESULT CALLBACK ImageTreeView::Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static TV_INSERTSTRUCT tv{};
	static avs::ImageInfo image[4];

	HWND hTree = GetDlgItem(hDlg, IDC_TREE1);

	switch (msg) {
	case WM_INITDIALOG:
	{
		tv.hInsertAfter = TVI_LAST;
		tv.item.mask = TVIF_TEXT;
		tv.hParent = TVI_ROOT;
		tv.item.pszText = TEXT("親１");
		TreeView_InsertItem(hTree, &tv);
		tv.item.pszText = TEXT("親２");
		HTREEITEM hParent = TreeView_InsertItem(hTree, &tv);
		tv.hParent = hParent;
		tv.item.pszText = TEXT("子１");
		TreeView_InsertItem(hTree, &tv);
		tv.item.pszText = TEXT("子２");
		TreeView_InsertItem(hTree, &tv);
	}
	break;

	case WM_DROPFILES:
		break;

	case WM_NOTIFY:
		if (wParam == IDC_TREE1)
		{
			TV_DISPINFO* ptv_disp = (TV_DISPINFO *)lParam;
			switch (ptv_disp->hdr.code){
			case TVN_BEGINDRAG:                                                    //!< ドラッグ開始
				begin_drag(hTree, reinterpret_cast<LPNMTREEVIEW>(lParam));
				break;

			case TVN_SELCHANGING:
				// 項目が選択された
				return TRUE;

			case TVN_ENDLABELEDIT:
				// ツリーの項目編集が終わったのでデータをセット
				TreeView_SetItem(hTree, &ptv_disp->item);
				return TRUE;
			}
		}
		break;

	case WM_MOUSEMOVE:
	{
		POINT pnt;
		HTREEITEM h_item = NULL;

		pnt.x = GET_X_LPARAM(lParam);
		pnt.y = GET_Y_LPARAM(lParam);
		if (g_fdragging)
		{
			//unlock window and allow updates to occur
			ImageList_DragLeave(NULL);
			ClientToScreen(hDlg, &pnt);
			//check with the tree control to see if we are on an item
			TVHITTESTINFO tv_ht;
			ZeroMemory(&tv_ht, sizeof(TVHITTESTINFO));
			tv_ht.flags = TVHT_ONITEM;
			tv_ht.pt.x = pnt.x;
			tv_ht.pt.y = pnt.y;
			ScreenToClient(hTree, &(tv_ht.pt));
			h_item = (HTREEITEM)SendMessage(hTree, TVM_HITTEST, 0, (LPARAM)&tv_ht);
			if (h_item)
			{  //if we had a hit, then drop highlite the item
				SendMessage(hTree, TVM_SELECTITEM, TVGN_DROPHILITE, (LPARAM)h_item);
			}
			//paint the image in the new location
			ImageList_DragMove(pnt.x, pnt.y);
			//lock the screen again
			ImageList_DragEnter(NULL, pnt.x, pnt.y);
			return TRUE;
		}
		break;
	}

	case WM_LBUTTONUP:
	{
		HTREEITEM h_item = NULL;
		TVHITTESTINFO tv_ht;
		TVITEM tv;
		ZeroMemory(&tv, sizeof(TVITEM));
		ZeroMemory(&tv_ht, sizeof(TVHITTESTINFO));

		if (g_fdragging)
		{
			ImageList_DragLeave(NULL);
			ImageList_EndDrag();
			ReleaseCapture();
			//determin if we let up on an item
			GetCursorPos(&(tv_ht.pt));
			ScreenToClient(hTree, &(tv_ht.pt));
			tv_ht.flags = TVHT_ONITEM;
			h_item = (HTREEITEM)SendMessage(hTree, TVM_HITTEST, 0, (LPARAM)&tv_ht);
			ShowCursor(TRUE);
			g_fdragging = FALSE;
			if (h_item)
			{
				/*we need to defer changing the selection until done processing this message post message allows us to do this. */
//				PostMessage(hDlg, M_CHANGEFOCUS, (WPARAM)0, (LPARAM)h_item);
			}
			return TRUE;
		}
		break;
	}

	}

	return FALSE;
}
