#include "stdafx.h"
#include "ImageTreeView.h"
#include "resource.h"
#include <windowsx.h>
#include <commctrl.h>
#include "AvsCore/ImageListener.h"

#pragma comment( lib, "Comctl32.lib" )

BOOL g_fdragging = FALSE;
static HTREEITEM    hDragItem;
static HMENU hPopMenu;

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

	SetCapture(GetParent(h_tree));   // �}�E�X�L���v�`���J�n

	g_fdragging = TRUE;             // �h���b�O���̃t���O

	// �h���b�O�A�C�e���̃n���h�����擾
	hDragItem = lpnmtv->itemNew.hItem;

	// �h���b�O�A�C�e����I������
	TreeView_SelectItem(h_tree, hDragItem);

	// �h���b�O�̉摜��p�ӂ���
	HIMAGELIST hIml_DD;

	hIml_DD = TreeView_CreateDragImage(h_tree, hDragItem);
	//�h���b�O�摜�̕\���ʒu�����߂�v�Z
	htItem = hDragItem;
	level = 0;
	do{
		htItem = TreeView_GetParent(h_tree, htItem);
		level++;
	} while (htItem);
	xIndent = TreeView_GetIndent(h_tree);
	TreeView_GetItemRect(h_tree, hDragItem, &rcItem, FALSE);

	// �h���b�O�摜��\������
	ImageList_BeginDrag(hIml_DD, 0, lpnmtv->ptDrag.x - rcItem.left -
		xIndent*level, lpnmtv->ptDrag.y - rcItem.top);
#endif
	return TRUE;

}

void InitializeMenuItem(HMENU hmenu, LPTSTR lpszItemName, int nId, HMENU hmenuSub)
{
	MENUITEMINFO mii;

	mii.cbSize = sizeof(MENUITEMINFO);
	mii.fMask = MIIM_ID | MIIM_TYPE;
	mii.wID = nId;

	if (lpszItemName != NULL) {
		mii.fType = MFT_STRING;
		mii.dwTypeData = lpszItemName;
	}
	else
		mii.fType = MFT_SEPARATOR;

	if (hmenuSub != NULL) {
		mii.fMask |= MIIM_SUBMENU;
		mii.hSubMenu = hmenuSub;
	}

	InsertMenuItem(hmenu, -1, TRUE, &mii); // -1,TRUE�ōŉ��i�ւ̒ǉ����Ӗ�����
}

HTREEITEM TreeViewAddItem(HWND hTree, HTREEITEM parent, const LPTSTR text)
{
	TV_INSERTSTRUCT tv{};
	tv.hInsertAfter = TVI_LAST;
	tv.item.mask = TVIF_TEXT;
	tv.hParent = (parent)? parent : TVI_ROOT;
	tv.item.pszText = text;
	HTREEITEM item = TreeView_InsertItem(hTree, &tv);
	return item;
}

void ImageTreeView::AddControl(ITreeViewControl* pControl)
{
	m_pControl = pControl;
}

LRESULT ImageTreeView::OnInitDialog()
{
	HWND hTree = GetDlgItem(m_hwnd, IDC_TREE1);
	if (m_pControl)
		m_pControl->Setup(hTree);

	hPopMenu = CreatePopupMenu();
	InitializeMenuItem(hPopMenu, TEXT("�؂���(&T)"), IDM_SAVE, NULL);
	InitializeMenuItem(hPopMenu, TEXT("�R�s�[(&C)"), IDM_SAVE, NULL);
	InitializeMenuItem(hPopMenu, TEXT("�\��t��(&P)"), IDM_SAVE, NULL);
	InitializeMenuItem(hPopMenu, TEXT("�폜(&D)"), IDM_SAVE, NULL);
	InitializeMenuItem(hPopMenu, NULL, 0, NULL);
	InitializeMenuItem(hPopMenu, TEXT("�}��(&I)"), IDM_SAVE, NULL);

	return TRUE;
}

LRESULT ImageTreeView::OnContextMenu(int xPos, int yPos, HWND hwnd)
{
	HWND hTree = GetDlgItem(m_hwnd, IDC_TREE1);
	TV_HITTESTINFO tvhtst;
	tvhtst.pt.x = xPos;
	tvhtst.pt.y = yPos;
	ScreenToClient(hTree, &tvhtst.pt);
	HTREEITEM hItem = TreeView_HitTest(hTree, &tvhtst);
	if (hItem) {
		TreeView_SelectItem(hTree, hItem);

		TrackPopupMenu(hPopMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			xPos, yPos, 0/*�K��0*/, m_hwnd, NULL);
	}
	else if (hwnd == hTree){
		TreeView_SelectItem(hTree, NULL);
		TrackPopupMenu(hPopMenu, TPM_TOPALIGN | TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			xPos, yPos, 0/*�K��0*/, m_hwnd, NULL);
		TV_INSERTSTRUCT tv{};
		tv.hInsertAfter = TVI_LAST;
		tv.item.mask = TVIF_TEXT;
		tv.hParent = TVI_ROOT;
		tv.item.pszText = TEXT("�ǉ�");
		TreeView_InsertItem(hTree, &tv);
	}
	return TRUE;
}

LRESULT ImageTreeView::OnClose()
{
	DestroyMenu(hPopMenu);
	EndDialog(m_hwnd, WM_CLOSE);
	return TRUE;
}
