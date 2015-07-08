/*! @file TextListener.h */
#pragma once

namespace avs
{
	/*!
		@brief ScriptEngine�̃e�L�X�g�p���X�i�[
		
		ScriptEngine�Ƀe�L�X�g�̏o�͂���������ƃR�[���o�b�N�����B
	 */
	class avsTextListener {
	protected:
		~avsTextListener() {}
	public:
		enum TEXT_ATTR {
			DEFAULT		//!< 
		};

		//! �e�L�X�g�o��
		virtual void Put(const char* str /**< �e�L�X�g�̃|�C���^ */,
						 float time /**< �e�L�X�g�̕\������ */,
						 int id) = 0;
		//! ���s���w��
		virtual void Crlf() = 0;
		//! �������w��
		virtual void Attr(TEXT_ATTR attr) = 0;
		//! �I�����̏o��
		virtual void Option(const char* str, /**< �I�����e�L�X�g�̃|�C���^ */
							int num /**< ���ʎq�i�X�N���v�g�C���f�b�N�X�j*/) = 0;
		//! �e�L�X�g������
		virtual void Clear() = 0;
		//! �N���b�N�҂��v��
		virtual void Click() = 0;

		virtual void Window(float x, float y) = 0;

		virtual bool IsBusy() = 0;
	};
}
