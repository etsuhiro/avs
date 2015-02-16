/*! @file SoundTool.h */

#pragma	once

#include <commctrl.h>

class SoundTool {
public:
	SoundTool();
	~SoundTool();

	void	Create(HINSTANCE hInst, HWND hWnd);
	void	Update(HWND hDlg);

private:
	static LRESULT CALLBACK Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);

};
