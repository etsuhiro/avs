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
		// αが0の場合は非表示なので計算を端折る
		if (info.a == 0)	break;
		info.hscale *= parent.hscale;
		info.vscale *= parent.vscale;
		info.x *= parent.hscale;
		info.y *= parent.vscale;

		// 親の回転角が0の場合は回転計算を端折る
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

void AvsSprite::Draw()
{
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

void AvsSprite::Delete(avs::AvsSprite* sprite)
{
	delete sprite;
}
