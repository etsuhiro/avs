#include "ImageController.h"
#include "AvsSprite.h"
#include "common/crc32.h"
#include <math.h>

using namespace avs;

namespace {
	static float lerp(float origin, float target, float rate)
	{
		return origin*(1.f - rate) + target*rate;
	}

	float linear(float elapsedTime, float transTime)
	{
		return elapsedTime / transTime;
	}

	float sinusoidal(float elapsedTime, float transTime)
	{
#define HALF_PI 1.5707963f
#define W_PI 6.2831853f
		return sinf(W_PI *  elapsedTime / transTime - HALF_PI) * 0.5f + 0.5f;
	}

	float easein(float elapsedTime, float transTime)
	{
		return 1.f - cosf(HALF_PI *  elapsedTime / transTime);
	}

	float easeout(float elapsedTime, float transTime)
	{
		return sinf(HALF_PI *  elapsedTime / transTime);
	}
}

Cgt::Cgt(AvsSprite* sprite, const avs::ImageInfo& target, float time, unsigned int change, int method)
	: m_sprite(sprite)
	, m_arrival(target)
	, m_transTime(time)
	, m_elapsedTime(0)
	, m_change(change)
	, m_func(linear)
	, m_loop(false)
{
	static struct {
		int hash;
		float(*func)(float, float);
		bool loop;
	} methodTable[] = {
		{ 0, linear, false },
		{ getCRC32("loop"), linear, true },
		{ getCRC32("sin"), sinusoidal, true },
		{ getCRC32("easein"), easein, false },
		{ getCRC32("easeout"), easeout, false },
	};
	for (int i = 0; i < 5; i++){
		if (methodTable[i].hash == method){
			m_func = methodTable[i].func;
			m_loop = methodTable[i].loop;
			break;
		}
	}
	m_departure = sprite->ImageInfo();
}

bool Cgt::IsTarget(AvsSprite* p)
{
	return p == m_sprite;
}

int Cgt::Update(float elapsedTime)
{
	bool finish = (m_transTime == 0);
	m_elapsedTime += elapsedTime;
	if (m_elapsedTime >= m_transTime){
		if (m_loop){
			m_elapsedTime -= m_transTime;
		}
		else {
			finish = true;
		}
	}
	if (finish){
		Finalize();
		return -1;
	}

	float rate = m_func(m_elapsedTime, m_transTime);

	avs::ImageInfo& info = m_sprite->ImageInfo();

	if (m_change & avs::CHG_X){
		info.x = lerp(m_departure.x, m_arrival.x, rate);
	}
	if (m_change & avs::CHG_Y){
		info.y = lerp(m_departure.y, m_arrival.y, rate);
	}
	if (m_change & avs::CHG_XPIVOT){
		info.xpivot = lerp(m_departure.xpivot, m_arrival.xpivot, rate);
	}
	if (m_change & avs::CHG_YPIVOT){
		info.ypivot = lerp(m_departure.ypivot, m_arrival.ypivot, rate);
	}
	if (m_change & avs::CHG_HSCALE){
		info.hscale = lerp(m_departure.hscale, m_arrival.hscale, rate);
	}
	if (m_change & avs::CHG_VSCALE){
		info.vscale = lerp(m_departure.vscale, m_arrival.vscale, rate);
	}
	if (m_change & avs::CHG_R){
		info.r = lerp(m_departure.r, m_arrival.r, rate);
	}
	if (m_change & avs::CHG_G){
		info.g = lerp(m_departure.g, m_arrival.g, rate);
	}
	if (m_change & avs::CHG_B){
		info.b = lerp(m_departure.b, m_arrival.b, rate);
	}
	if (m_change & avs::CHG_A){
		info.a = lerp(m_departure.a, m_arrival.a, rate);
	}
	if (m_change & avs::CHG_U){
		info.u = lerp(m_departure.u, m_arrival.u, rate);
	}
	if (m_change & avs::CHG_V){
		info.v = lerp(m_departure.v, m_arrival.v, rate);
	}
	if (m_change & avs::CHG_W){
		info.w = lerp(m_departure.w, m_arrival.w, rate);
	}
	if (m_change & avs::CHG_H){
		info.h = lerp(m_departure.h, m_arrival.h, rate);
	}
	if (m_change & avs::CHG_ROT){
		info.rot = lerp(m_departure.rot, m_arrival.rot, rate);
	}
	if (m_change & avs::CHG_PRIORITY){
		info.priority = lerp(m_departure.priority, m_arrival.priority, rate);
	}

	return 0;
}

void Cgt::Finalize()
{
	avs::ImageInfo& info = m_sprite->ImageInfo();
	avs::ImageInfo& final = (m_loop) ? m_departure : m_arrival;

	if (m_change & avs::CHG_X){
		info.x = final.x;
	}
	if (m_change & avs::CHG_Y){
		info.y = final.y;
	}
	if (m_change & avs::CHG_XPIVOT){
		info.xpivot = final.xpivot;
	}
	if (m_change & avs::CHG_YPIVOT){
		info.ypivot = final.ypivot;
	}
	if (m_change & avs::CHG_HSCALE){
		info.hscale = final.hscale;
	}
	if (m_change & avs::CHG_VSCALE){
		info.vscale = final.vscale;
	}
	if (m_change & avs::CHG_R){
		info.r = final.r;
	}
	if (m_change & avs::CHG_G){
		info.g = final.g;
	}
	if (m_change & avs::CHG_B){
		info.b = final.b;
	}
	if (m_change & avs::CHG_A){
		info.a = final.a;
	}
	if (m_change & avs::CHG_U){
		info.u = final.u;
	}
	if (m_change & avs::CHG_V){
		info.v = final.v;
	}
	if (m_change & avs::CHG_W){
		info.w = final.w;
	}
	if (m_change & avs::CHG_H){
		info.h = final.h;
	}
	if (m_change & avs::CHG_ROT){
		info.rot = final.rot;
		// Šp“x‚ð0`360–¢–ž‚É³‹K‰»
		while (info.rot >= 360.f)
			info.rot -= 360.f;
		while (info.rot < 0.f)
			info.rot += 360.f;
	}
	if (m_change & avs::CHG_PRIORITY){
		info.priority = final.priority;
	}
}
