#pragma once

#include <pao/Windows/DialogProc.h>
#include <Windows.h>

class TreeViewItem {
	static HTREEITEM s_parent;
	HTREEITEM store;

public:
	TreeViewItem(HWND hTree, const LPTSTR text);
	~TreeViewItem();
};

class ITreeViewControl {
public:
	virtual void Setup(HWND hTree);
};

class ImageTreeView : public pao::IDialogProc {
	ITreeViewControl* m_pControl;
	virtual LRESULT OnInitDialog() override;
	virtual LRESULT OnClose() override;
	virtual LRESULT OnContextMenu(int xPos, int yPos, HWND hwnd) override;

public :
	void AddControl(ITreeViewControl* pControl);
};
