/*! @file ScriptEngine.h */

#pragma once

#include "ScriptEngine/RunningStatus.h"

namespace miku {
	struct Node;
	struct ElementNode;
}

namespace avs
{
class ScriptContext;
class avsTextListener;
class avsImageListener;
class avsSoundListener;
class avs3DListener;
class VariableListener;
class ErrorMessageListener;

//! �X�N���v�g�G���W��
class ScriptEngine {
public:
	ScriptEngine();
	virtual ~ScriptEngine();

	//! ���s
	/*! �X�N���v�g��i�߂邽�тɌĂ�
	 * @retval FINISH �X�N���v�g���I�����܂����B
	 * @retval CLICK �N���b�N�҂��B�Ă�Run()���ĂԂƑ��s����B
	 * @retval SELECT�@�I�����BGotoOption()�őI������B
	 * @retval LABEL_JUMP�@���̃X�N���v�g�փW�����v�B�t�@�C���ǂݍ��݂����ASetScript()�Œm�点��B
	 */
	RunningStatus Run();

	void Init();
	void SetScript(const char *script);

	//! �w�肵���I�����փW�����v
	void GotoOption(int n);

	//! ���s�J�n�̃C���f�b�N�X���Z�b�g����
	/*! ���[�h���čĊJ�A�o�b�N�W�����v���Ŏg�p */
	void SetIndex(int idx){ m_idx = idx; }

	//! ���s�C���f�b�N�X�𓾂�
	/*! ���̒l���Z�[�u���A���[�h�ĊJ���Ɏg�p���� */
	int GetIndex() const { return m_idx; }

	//! �C�x���g���X�i�[��o�^����
	/*! �������o�^�ł���̂�1��ނ��� */
	void AddTextListener(avsTextListener* pListener);

	//! �ϐ����X�i�[��o�^����
	void AddVariableListener(VariableListener* pListener);

	//! �G���[���b�Z�[�W���X�i�[��o�^����
	void AddErrorMessageListener(ErrorMessageListener* pListener);

	//! �C���[�W���X�i�[��o�^����
	void AddImageListener(avsImageListener* pListener);

	//! �T�E���h���X�i�[��o�^����
	void AddSoundListener(avsSoundListener* pListener);

	//! 3D���X�i�[��o�^����
	void Add3DListener(avs3DListener* pListener);

	const char *getHeadline() const;

	//! �W�����v�t�@�C�����𓾂�
	const char* getJumpFile() const;

	//! �҂����Ԃ𓾂�
	float getWaitTime() const;

	//! �ʒm�C�x���g�𓾂�
	int getEvent() const { return m_event; }

	//! �\�����b�Z�[�W������������X�N���v�g�̃C���f�b�N�X��n��
	/*! �o�b�N���O�y�ъ��ǃt���O�Z�b�g�p */
	virtual void MsgIdx(int) {}

	/**
	 * �X�N���v�g�t�@�C���ǂݍ��݃��N�G�X�g
	 *
	 * ���̃X�N���v�g�̓ǂݍ��݃��N�G�X�g����������ƌĂ΂��R�[���o�b�N
	 */
	virtual void ScriptLoad(const char *) {}

private:
	RunningStatus run(int idx);
	void labelJump();

	ScriptContext&	m_Context;
	int				m_idx;
	int				m_event;
	bool			m_FileLoadReq;
};

}
