#include "DxLibAvsImage.h"
#include "DxLib.h"
#include <list>
#include <algorithm>
#include <math.h>

namespace{
	int s_center_x = 640/2;
	int s_center_y = 480/2;
}

struct Asset {
	std::string name;
	int handle;
	int referCount;
};

class AssetManager {
	std::list<Asset> m_assets;

public:
	~AssetManager(){
		for (auto& asset : m_assets){
			DeleteGraph(asset.handle);
		}
		m_assets.clear();
	}
	int Load(const char* path){
		auto it = std::find_if(m_assets.begin(), m_assets.end(), [&path](Asset& asset){return asset.name == path; });
		if (it != m_assets.end()){
			it->referCount++;
			return it->handle;
		}

		int handle = LoadGraph(path);
		if (handle >= 0){
			Asset asset{ path, handle, 1 };
			m_assets.push_back(asset);
		}
		return handle;
	}
	void Remove(int handle){
		auto it = std::find_if(m_assets.begin(), m_assets.end(), [&handle](Asset& asset){return asset.handle == handle; });
		if (it != m_assets.end()){
			if (--it->referCount == 0){
				DeleteGraph(it->handle);
				m_assets.erase(it);
			}
		}
	}
};

AssetManager asset_manager;

class DxLibAvsSprite : public avs::AvsSprite {
	int m_ghandle;
	avs::ImageInfo m_local;

	DxLibAvsSprite(int ghandle, int id, const avs::ImageInfo& info, avs::AvsSprite* parent)
		: avs::AvsSprite(id, info, parent)
		, m_ghandle(ghandle)
	{
		int sizeX, sizeY;
		GetGraphSize(m_ghandle, &sizeX, &sizeY);
		if (info.w == 0)	ImageInfo().w = sizeX;
		if (info.h == 0)	ImageInfo().h = sizeY;
		ImageInfo().xpivot = 0.5f*ImageInfo().w;
		ImageInfo().ypivot = 0.5f*ImageInfo().h;
	}

	virtual ~DxLibAvsSprite()
	{
		asset_manager.Remove(m_ghandle);
	}

	virtual void SetSprite(const avs::ImageInfo& info)
	{
		m_local = info;
	}

	virtual void Draw() override
	{
//		DrawGraph(m_local.x, m_local.y, m_ghandle, TRUE);
		DrawRotaGraph3(m_local.x + s_center_x, m_local.y + s_center_y, m_local.xpivot, m_local.ypivot, m_local.hscale, m_local.vscale, m_local.rot, m_ghandle, TRUE);
	}

public:
	static DxLibAvsSprite* Create(int id, const char* file, const avs::ImageInfo& info, avs::AvsSprite* parent)
	{
		std::string fname_full(file);
		fname_full += ".png";
		int handle = asset_manager.Load(fname_full.c_str());
		if (handle >= 0){
			return new DxLibAvsSprite(handle, id, info, parent);
		}
		return NULL;
	}
};


avs::AvsSprite* DxLibAvsImage::CreateSprite(int id, const char* file, const avs::ImageInfo& info, avs::AvsSprite* parent)
{
	return DxLibAvsSprite::Create(id, file, info, parent);
}

DxLibAvsImage::DxLibAvsImage()
{
}

void DxLibAvsImage::Draw()
{
	AvsImage::Draw();
}

void DxLibAvsImage::AddAsset(const char* file)
{
}

void DxLibAvsImage::ReleaseAsset(const char* file)
{
}

void DxLibAvsImage::Fade(float time, float a, float r, float g, float b)
{
}

void DxLibAvsImage::DrawSync(bool)
{
}

void DxLibAvsImage::SetScreenSize(int x, int y)
{
	s_center_x = x/2;
	s_center_y = y/2;
}
