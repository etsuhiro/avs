/*! @file 3DListener.h */
#pragma once

namespace avs
{
	/*!
		@brief ScriptEngineの3D用リスナー
		
		ScriptEngineに3Dの命令が発生するとコールバックされる。
	 */
	class avs3DListener {
	public:
		virtual ~avs3DListener() {}
		virtual void ActorAsset(const char* name, const char* action, const char* file, const char* group) = 0;
		virtual void Actor(const char* name, const char* action, float x, float y, float z, float rot, float scale, unsigned int change, int loop, const char* set) = 0;
		virtual void ActorConnect(const char* name, const char* add, const char* joint) = 0;
		virtual void Stage(int id, const char* file, const char* name, bool draw) = 0;
		virtual void Event(int id, const char* file, const char* action, int method, float x, float y, float z) = 0;
		virtual void StageDisp(int id, bool draw) = 0;
		virtual void StageDisp(bool draw) = 0;
		virtual void StageRemove(int id) = 0;
		virtual void StageRemove() = 0;
		virtual void DrawSync(bool) = 0;
		virtual void Camera(const char* file, const char* name, float near, float far, float fov, bool loop, int frame, const char* set, float x, float y, float z, float rot) = 0;
		virtual void Light(int id,const char* file, const char* name) = 0;
		virtual void Slow(float time) = 0;
		virtual void Effect(const char* file, const char* name, const char* joint, float x, float y, float z) = 0;
		virtual void Sync(int id, int frame) = 0;
		virtual bool IsBusy() = 0;
	};
}
