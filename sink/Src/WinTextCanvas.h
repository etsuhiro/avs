/*! @file WinTextCanvas.h */

#pragma	once

#include "avs/TextCanvas.h"

class WinTextCanvas : public avs::TextCanvas
{

private:
	void draw(int x, int y, const char *str, int n);
	void se();

	ID3DXFont*			pTextFont;
};

