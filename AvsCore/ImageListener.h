/*! @file ImageListener.h */
#pragma once

namespace avs
{
	struct ImageInfo {
		float x,y;
		float hscale,vscale;
		float r,g,b,a;
		short u,v,w,h;
		float rot;
		int	priority;
	};
	const int CHG_X = 1<<0;
	const int CHG_Y = 1<<1;
	const int CHG_HSCALE = 1<<2;
	const int CHG_VSCALE = 1<<3;
	const int CHG_R = 1<<4;
	const int CHG_G = 1<<5;
	const int CHG_B = 1<<6;
	const int CHG_A = 1<<7;
	const int CHG_U = 1<<8;
	const int CHG_V = 1<<9;
	const int CHG_W = 1<<10;
	const int CHG_H = 1<<11;
	const int CHG_ROT = 1<<12;
	const int CHG_PRIORITY = 1<<13;

	/*!
		@brief ScriptEngineの画像用リスナー
		
		ScriptEngineに画像の命令が発生するとコールバックされる。
	 */
	class avsImageListener {
	public:
		virtual ~avsImageListener() {}
		virtual void AddAsset(const char* file) = 0;
		virtual void ReleaseAsset(const char* file) = 0;
		virtual void Cg(int id, const char* file, const avs::ImageInfo& info, int parent) = 0;
		virtual void CgTrans(int id, float time, const avs::ImageInfo& info, unsigned int change, int method, float offset, const char* action) = 0;
		virtual void Fade(float time, float a, float r, float g, float b) = 0;
		virtual void DrawSync(bool) = 0;
		virtual bool IsBusy() = 0;
	};
}
