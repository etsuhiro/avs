#pragma once

#include "avs.h"

class DxLibAvsImage : public avs::AvsImage {
	virtual void AddAsset(const char* file);
	virtual void ReleaseAsset(const char* file);
	virtual void Fade(float time, float a, float r, float g, float b);
	virtual void DrawSync(bool);
	virtual avs::AvsSprite* CreateSprite(int id, const char* file, const avs::ImageInfo& info, avs::AvsSprite* parent);

public:
	DxLibAvsImage();
	void Draw();
	static void SetScreenSize(int x, int y);
};

