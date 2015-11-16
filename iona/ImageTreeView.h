#pragma once

#include <pao/Windows/DialogProc.h>
#include <Windows.h>
#include <commctrl.h>

HTREEITEM TreeViewAddItem(HWND hTree, HTREEITEM parent, const LPTSTR text);

class ITreeViewControl {
public:
	virtual void Setup(HWND hTree) = 0;
};

class ImageTreeView : public pao::IDialogProc {
	ITreeViewControl* m_pControl;
	virtual LRESULT OnInitDialog() override;
	virtual LRESULT OnClose() override;
	virtual LRESULT OnContextMenu(int xPos, int yPos, HWND hwnd) override;

public :
	ImageTreeView(HINSTANCE hInst, UINT nIDResource, HWND hWndParent);
	void SetTitle(LPCTSTR lpszTitle);
	void AddControl(ITreeViewControl* pControl);
};
