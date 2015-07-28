#pragma once

#include "AvsCore/ImageListener.h"

namespace avs
{
	class AvsSprite;

	class Cgt {
		AvsSprite* m_sprite;
		avs::ImageInfo m_departure;
		avs::ImageInfo m_arrival;
		float m_transTime;
		float m_elapsedTime;
		unsigned int m_change;
		float(*m_func)(float, float);
		bool m_loop;

	public:
		Cgt(AvsSprite* sprite, const avs::ImageInfo& target, float time, unsigned int change, int method);
		bool IsTarget(AvsSprite* p);
		int Update(float elapsedTime);
		void Finalize();
	};
}
