#pragma once

#include <Windows.h>
#include "AvsCore/ImageListener.h"

class ImageInfoDialog {
	avs::ImageInfo* m_pInfo;
	avs::ImageInfo m_orgInfo;
public:
	ImageInfoDialog(avs::ImageInfo* info);
	void Init(HWND hDlg);
	LRESULT Proc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
};