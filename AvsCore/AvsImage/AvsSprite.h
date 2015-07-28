#pragma once

#include "AvsCore/ImageListener.h"

namespace avs
{
	class AvsSprite {
		int m_id;
		avs::ImageInfo m_info;
		AvsSprite* m_parent;

		virtual void SetSprite(const avs::ImageInfo& info) = 0;

	public:
		AvsSprite(int id, const avs::ImageInfo& info, AvsSprite* parent);
		virtual ~AvsSprite();
		void Update();
		int GetId();
		AvsSprite* GetParent();
		void Disown();
		avs::ImageInfo& ImageInfo();
		static void Delete(avs::AvsSprite* sprite);
	};
}
