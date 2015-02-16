/*! @file Setup.h */
#pragma once

#include "avs/SetupData.h"
#include "windows.h"

struct WindowsSetupData : public avs::SetupData {
	TCHAR dataPath[MAX_PATH];
	char imagePath[256];
	char windowSrc[256];
	char clickIcon[256];
	wchar_t fontData[128];
	int fontSize;

	void Init();
	void Load();
};
