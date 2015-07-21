#include "AvsSprite.h"
#include <math.h>

#define F_PI 3.14159265358979323846f

using namespace avs;
AvsSprite::AvsSprite(int id, const avs::ImageInfo& info, AvsSprite* parent)
	: m_id(id)
	, m_info(info)
	, m_parent(parent)
{}

AvsSprite::~AvsSprite()
{}

void AvsSprite::Update()
{
	avs::ImageInfo info = m_info;
	AvsSprite* pParent = m_parent;
	while (pParent){
		avs::ImageInfo& parent = pParent->m_info;
		info.a *= parent.a;
		// ƒ¿‚ª0‚Ìê‡‚Í”ñ•\Ž¦‚È‚Ì‚ÅŒvŽZ‚ð’[Ü‚é
		if (info.a == 0)	break;
		info.hscale *= parent.hscale;
		info.vscale *= parent.vscale;
		info.x *= parent.hscale;
		info.y *= parent.vscale;

		// e‚Ì‰ñ“]Šp‚ª0‚Ìê‡‚Í‰ñ“]ŒvŽZ‚ð’[Ü‚é
		if (parent.rot != 0){
			float rad = parent.rot / 180.f * F_PI;
			float fsin = sinf(rad);
			float fcos = cosf(rad);
			float x = info.x * fcos - info.y * fsin;
			float y = info.x * fsin + info.y * fcos;
			info.x = x;
			info.y = y;
		}

		info.x += parent.x;
		info.y += parent.y;
		info.rot += parent.rot;
		pParent = pParent->m_parent;
	}
	SetSprite(info);
}

int AvsSprite::GetId()
{
	return m_id;
}

AvsSprite* AvsSprite::GetParent()
{
	return m_parent;
}

void AvsSprite::Disown()
{
	m_parent = 0;
}

avs::ImageInfo& AvsSprite::ImageInfo()
{
	return m_info;
}
