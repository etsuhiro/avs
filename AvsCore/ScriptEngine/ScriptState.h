/*! @file ScriptState.h */
/*!
	ScriptEngine�̃X�e�[�g�p�^�[��
 */

#pragma once

#include "XmlBin/miku.h"
#include "RunningStatus.h"

namespace avs {

class ScriptContext;
class avsTextListener;
class avsImageListener;
class avsSoundListener;
class avs3DListener;
class VariableListener;
class ErrorMessageListener;

//! State�N���X
class ScriptState {
public:
	virtual ~ScriptState(){}

	//! �^�O�̉���
	virtual void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	//! �����̉���
	virtual bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	//! �^�O�̏I������
	virtual RunningStatus TagTerminateImm(ScriptContext&, const miku::ElementNode *pElement, int index);
	virtual RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	//! �e�L�X�g�̏���
	virtual void MsgProc(ScriptContext&, const char *str);
	//! �f�o�b�O�p�Ƀ^�O����Ԃ�
	virtual const char* TagName() const = 0;

//	ScriptState*	m_pPreviousState;
};

//! ���s�X�e�[�g
class Run : public ScriptState {
// override:
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

//! �����X�e�[�g
class Invalid : public ScriptState {
// override:
	const char* TagName() const;
};

//! �N���b�N��X�e�[�g
class WaitClick : public ScriptState {
// override:
	void MsgProc(ScriptContext&, const char *str);
	const char* TagName() const;
};

class CmdHeadline : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	void MsgProc(ScriptContext&, const char *str);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdP : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	void MsgProc(ScriptContext&, const char *str);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdGoto : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdBr : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdClick : public ScriptState {
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdIf : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdWhile : public CmdIf {
	RunningStatus TagTerminateImm(ScriptContext&, const miku::ElementNode *pElement, int index);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdSelect : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdOption : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	void MsgProc(ScriptContext&, const char *str);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class GotoOption : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	void MsgProc(ScriptContext&, const char *str);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdSet : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdWait : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdAsset : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdCg : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdCgt : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	const char* TagName() const;
};

class CmdFade : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	const char* TagName() const;
};

class CmdActor : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdStage : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdEvent : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdCamera : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdLight : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdEffect : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	const char* TagName() const;
};

class CmdSync : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdDrawSync : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdSe : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	const char* TagName() const;
};

class CmdBgm : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	const char* TagName() const;
};

class CmdStream : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	const char* TagName() const;
};

class CmdMovie : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdChild : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdWindow : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

class CmdSlow : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	const char* TagName() const;
};

//! ���x���T�[�`�X�e�[�g
class LabelSearch : public ScriptState {
	bool AttrProc(ScriptContext&, const miku::ElementNode *pElement);
	void TagProc(ScriptContext&, const miku::ElementNode *pElement);
	RunningStatus TagTerminate(ScriptContext&, const miku::ElementNode *pElement, int index);
	void MsgProc(ScriptContext&, const char *) {}
	const char* TagName() const;
};

#define CONCRETE_STATE(STATE)							\
	private:											\
		static STATE		m_##STATE;					\
	public:												\
	void SetState_##STATE() { SetState(m_##STATE); }

//! �R���e�L�X�g
class ScriptContext {
	CONCRETE_STATE(Run);
	CONCRETE_STATE(Invalid);
	CONCRETE_STATE(WaitClick);
	CONCRETE_STATE(CmdP);
	CONCRETE_STATE(CmdBr);
	CONCRETE_STATE(CmdClick);
	CONCRETE_STATE(CmdIf);
	CONCRETE_STATE(CmdWhile);
	CONCRETE_STATE(CmdGoto);
	CONCRETE_STATE(LabelSearch);
	CONCRETE_STATE(CmdHeadline);
	CONCRETE_STATE(CmdSelect);
	CONCRETE_STATE(CmdOption);
	CONCRETE_STATE(GotoOption);
	CONCRETE_STATE(CmdSet);
	CONCRETE_STATE(CmdWait);
	CONCRETE_STATE(CmdAsset);
	CONCRETE_STATE(CmdCg);
	CONCRETE_STATE(CmdCgt);
	CONCRETE_STATE(CmdFade);
	CONCRETE_STATE(CmdActor);
	CONCRETE_STATE(CmdStage);
	CONCRETE_STATE(CmdEvent);
	CONCRETE_STATE(CmdCamera);
	CONCRETE_STATE(CmdLight);
	CONCRETE_STATE(CmdEffect);
	CONCRETE_STATE(CmdSync);
	CONCRETE_STATE(CmdDrawSync);
	CONCRETE_STATE(CmdSe);
	CONCRETE_STATE(CmdBgm);
	CONCRETE_STATE(CmdStream);
	CONCRETE_STATE(CmdMovie);
	CONCRETE_STATE(CmdChild);
	CONCRETE_STATE(CmdWindow);
	CONCRETE_STATE(CmdSlow);
public:
	ScriptContext();
	//! �X�e�[�g�̃Z�b�g
	void SetState(ScriptState& state);
	//! �X�e�[�g�̃Z�b�g
	void SetState(const miku::ElementNode *pElement);
	//! �e�̃X�e�[�g���Z�b�g����
	void SetParentState(const miku::Node *pNode);
	//! �^�O�̉���
	void TagProc(const miku::ElementNode *pElement);
	//! �����̉���
	bool AttrProc(const miku::ElementNode *pElement);
	//! �^�O�̏I������
	RunningStatus TagTerminateImm(const miku::ElementNode *pElement, int index);
	RunningStatus TagTerminate(const miku::ElementNode *pElement, int index);
	//! �e�L�X�g�̏���
	void MsgProc(const char *str);
	//! �f�o�b�O�p�Ƀ^�O����Ԃ�
	const char* TagName() const;

	//! �X�N���v�g�G���[ �����Ȏq�v�f
	void ErrorInvalidElement(int cmd, int element);
	//! �X�N���v�g�G���[ �����ȑ���
	void ErrorInvalidAttr(int cmd, int attr);
	//! �X�N���v�g�G���[ �K�{�̑������Ȃ�
	void ErrorRequiredAttr(int cmd, int attr);
	//! �X�N���v�g�G���[ �����ȕ����f�[�^
	void ErrorInvalidText(const char *tag, const char* str);
	//! �X�N���v�g�G���[ �W�����v��̃��x����������Ȃ�
	void ErrorUnfoundLabel(unsigned int label);

	//! �ϐ���Ԃ�
	int GetVariable(const char* str);
	//! �ϐ����Z�b�g����
	void SetVariable(const char* str, int val);

	const char*		m_ScriptBuf;
	avsTextListener*	m_pTextListener;
	avsImageListener*	m_pImageListener;
	avsSoundListener*	m_pSoundListener;
	avs3DListener*		m_p3DListener;
	VariableListener*	m_pVariableListener;
	ErrorMessageListener*	m_pErrorMessageListener;
	const char*		m_Headline;
	unsigned int	m_label;	///< �W�����v�惉�x���̃n�b�V���l
	const char*		m_file;
	float			m_WaitTime;
	float			m_time;
	int				m_id;
//	int				m_nOption;	///< �I�����̐�

private:
	ScriptState*	m_pState;
	int				m_var[10];	///< �ꎞ�ϐ�
};

#undef CONCRETE_STATE

}

