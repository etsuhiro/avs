#pragma once

#include "ImageController.h"
#include <list>

namespace avs
{
	class AvsImage : public avs::avsImageListener {
		std::list<avs::AvsSprite*> sprite_list;
		std::list<avs::Cgt> cgt_list;
	protected:
		bool m_busy;

	private:
		virtual void AddAsset(const char* file) {};
		virtual void ReleaseAsset(const char* file) {};
		virtual void Cg(int id, const char* file, const avs::ImageInfo& info, int parent);
		virtual void CgTrans(int id, float time, const avs::ImageInfo& info, unsigned int change, int method);
		virtual void Fade(float time, float a, float r, float g, float b) {};
		virtual void DrawSync(bool) {};
		virtual bool IsBusy();
		virtual avs::AvsSprite* CreateSprite(int id, const char* file, const avs::ImageInfo& info, avs::AvsSprite* parent) = 0;

		void FinalizeSprite(avs::AvsSprite*);
		avs::AvsSprite* SearchSprite(int id);

	public:
		AvsImage();
		virtual ~AvsImage();
		void Update(float elapsedTime);
		void Draw();
	};
}
