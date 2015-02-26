#pragma once

#include <Windows.h>

class ImageInfoDialog {
public:
	static void Create(HINSTANCE hInst, HWND hWndParent);

private:
	static LRESULT CALLBACK Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
};