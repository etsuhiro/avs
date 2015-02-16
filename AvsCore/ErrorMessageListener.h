/*! @file ErrorMessage.h */

namespace avs
{
	/*!
		@brief ScriptEngine�̃G���[���b�Z�[�W���X�i�[
		
		ScriptEngine�ɃG���[�����������ꍇ�A���b�Z�[�W���R�[���o�b�N����B
	 */
	class ErrorMessageListener {
	public:
		virtual ~ErrorMessageListener() {}

		//! �X�N���v�g�G���[ �����Ȏq�v�f
		virtual void ErrorInvalidElement(int cmd, int element) = 0;
		//! �X�N���v�g�G���[ �����ȑ���
		virtual void ErrorInvalidAttr(int cmd, int attr) = 0;
		//! �X�N���v�g�G���[ �K�{�̑������Ȃ�
		virtual void ErrorRequiredAttr(int cmd, int attr) = 0;
		//! �X�N���v�g�G���[ �����ȕ����f�[�^
		virtual void ErrorInvalidText(const char *tag, const char* str) = 0;
		//! �X�N���v�g�G���[ �W�����v��̃��x����������Ȃ�
		virtual void ErrorUnfoundLabel(unsigned int label) = 0;
	};
}
