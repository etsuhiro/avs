/*! @file layerTool.h */
#pragma	once

#include <commctrl.h>
#include <string>
#include <list>

class LayerTool {
public:
	//! レイヤーツール作成
	static void Create(HINSTANCE hInst, HWND hWndParent);
	//! 更新
	static void Update();
	//! ドラッグで画像移動
	static void ImageMove(UINT msg, WPARAM wParam, LPARAM lParam);
	//! 

private:
	LayerTool();
	~LayerTool();
	void update(int num);
	void setHistory(HWND hDlg);	// !<履歴セット
	void appendHistory(WCHAR* path);

	static LRESULT CALLBACK Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
	static void StatusSet(HWND hDlg, int num);

	HWND	m_hWnd;
	bool	m_moveMode;	//!< 画像移動モード
	POINT	mouseStartPos;
	POINT	layerStartPos;
	std::list<std::wstring>	m_history;
	static	LayerTool* pInstance;
};
