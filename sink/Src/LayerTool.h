/*! @file layerTool.h */
#pragma	once

#include <commctrl.h>
#include <string>
#include <list>

class LayerTool {
public:
	//! ���C���[�c�[���쐬
	static void Create(HINSTANCE hInst, HWND hWndParent);
	//! �X�V
	static void Update();
	//! �h���b�O�ŉ摜�ړ�
	static void ImageMove(UINT msg, WPARAM wParam, LPARAM lParam);
	//! 

private:
	LayerTool();
	~LayerTool();
	void update(int num);
	void setHistory(HWND hDlg);	// !<�����Z�b�g
	void appendHistory(WCHAR* path);

	static LRESULT CALLBACK Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
	static void StatusSet(HWND hDlg, int num);

	HWND	m_hWnd;
	bool	m_moveMode;	//!< �摜�ړ����[�h
	POINT	mouseStartPos;
	POINT	layerStartPos;
	std::list<std::wstring>	m_history;
	static	LayerTool* pInstance;
};
