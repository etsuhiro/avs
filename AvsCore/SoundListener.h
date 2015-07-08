/*! @file SoundListener.h */
#pragma once

namespace avs
{
	/*!
		@brief ScriptEngine�̃T�E���h�p���X�i�[
		
		ScriptEngine�ɃT�E���h�̏o�͂���������ƃR�[���o�b�N�����B
	 */
	class avsSoundListener {
	protected:
		~avsSoundListener() {}
	public:
		virtual void Voice(const char* file, int chrId) = 0;
		virtual void Se(int id, const char* file, float volume, float pan) = 0;
		virtual void Bgm(const char* file, const char* action, float volume, float fadeTime) = 0;
		virtual void Stream(const char* file, float volume, int sync) = 0;
		virtual void Movie(const char* file) = 0;
		virtual bool IsBusy() = 0;
	};
}
