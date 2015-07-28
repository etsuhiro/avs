/*! @file ScriptEngine.cpp */

#include "ScriptEngine.h"
#include "ScriptEngine/ScriptState.h"
#include "XmlBin/miku.h"
#include "sinkenum.h"
#include "3DListener.h"
#include "ImageListener.h"
#include "SoundListener.h"
#include "TextListener.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

using namespace avs;

//
// 	�R���X�g���N�^
//
ScriptEngine::ScriptEngine()
	: m_Context(*(new ScriptContext))
	, m_idx(0)
	, m_FileLoadReq(false)
{
}

ScriptEngine::~ScriptEngine()
{
	delete &m_Context;
}

/**
 * �X�N���v�g��V�K�ɃZ�b�g����ꍇ�ɏ�����
 *
 * SetScript�̒��O�ł�����ł��悢�B
 * Label�W�����v�ɂ��SetScript���Ă΂ꂽ�ꍇ�ɂ�Init���Ȃ��B
 */
void ScriptEngine::Init()
{
	m_Context.m_label = 0;
	m_Context.SetState_Run();
}

/**
 * �G���W���ɃX�N���v�g���Z�b�g����
 *
 * �X�N���v�g���ǂݍ��܂ꂽ�炱�̊֐��ŃG���W���ɒm�点��B
 * �����o�b�t�@�ɓǂݍ��񂾏ꍇ�ł��A�K�v�B
 */
void ScriptEngine::SetScript(const char *script ///< �X�N���v�g�̃|�C���^
							)
{	
	m_Context.m_ScriptBuf = script;
	m_Context.m_Headline = 0;	// �X�N���v�g���ς��ƃ��Z�b�g
	m_idx = 0;
	m_FileLoadReq = false;
}

RunningStatus ScriptEngine::Run(float elapsedTime)
{
	if ((m_Context.m_WaitTime -= elapsedTime) > 0.f)
		return WAIT;
	else
		m_Context.m_WaitTime = 0;

	if (m_Context.m_ScriptBuf == 0)	return FINISH;

	if (m_FileLoadReq)	return LABEL_JUMP;

	if (m_Context.m_pTextListener){
		if (m_Context.m_pTextListener->IsBusy())	return BUSY;
	}
	if (m_Context.m_pImageListener){
		if (m_Context.m_pImageListener->IsBusy())	return BUSY;
	}
	if (m_Context.m_pSoundListener){
		if (m_Context.m_pSoundListener->IsBusy())	return BUSY;
	}
	if (m_Context.m_p3DListener){
		if (m_Context.m_p3DListener->IsBusy())	return BUSY;
	}

	RunningStatus stat;
LOOP:
	stat = run(m_idx);
	if (stat==LABEL_JUMP){
		labelJump();
		if (m_Context.m_file==0){	// ���t�@�C�������x���W�����v
			goto LOOP;
		}
	}
	return stat;
}

RunningStatus ScriptEngine::run(int idx /**< �J�n�C���f�b�N�X */)
{
	do {
LOOP:
		const miku::Node *pNode = reinterpret_cast<const miku::Node *>(&m_Context.m_ScriptBuf[idx]);
//	printf("[%d]", idx);
		if (pNode->isElementType()){
			const miku::ElementNode *pElement = static_cast<const miku::ElementNode *>(pNode);
			// �^�O�����߂��ėv�f�ʂɃX�e�[�g���ڍs
			m_Context.TagProc(pElement);
//	std::printf("enter %s\n", m_Context.TagName());
			// �����̉��߁B�q�v�f�X�L�b�v�̔��������ꍇ������
			bool child = m_Context.AttrProc(pElement);
			// �q�v�f�̎��s
			if (child && pElement->getChildNode()){
				idx = pElement->getChildNode();
				continue;
			}

			// �q�v�f�������Ȃ��ꍇ�̃^�O�̏I��
//	std::printf("terminate1 %s[%d]\n", m_Context.TagName(), idx);
			RunningStatus retval = m_Context.TagTerminateImm(pElement, idx);
			if (retval){
				m_idx = idx;
//	std::printf("term1 %s\n", m_Context.TagName());
				return retval;
			}
		} else {
			// �e�L�X�g�̏���
			const miku::TextNode *pText = static_cast<const miku::TextNode *>(pNode);
			m_Context.MsgProc(&m_Context.m_ScriptBuf[pText->entity]);
			MsgIdx(idx);	// ���ǃ`�F�b�N
		}

		while (pNode->isTerminate()){
			// �����Ȃ���΂P�K�w��̃G�������g�ɖ߂�
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<const miku::Node *>(&m_Context.m_ScriptBuf[idx]);
			const miku::ElementNode *pElement = static_cast<const miku::ElementNode *>(pNode);
			// �^�O�̏I��
//	std::printf("terminate2 %s[%d]\n", m_Context.TagName(), idx);
			RunningStatus retval = m_Context.TagTerminate(pElement, idx);
			if (retval==WHILE)	goto LOOP;
			if (retval){
				m_idx = idx;
//	std::printf("term2 %s\n", m_Context.TagName());
				return retval;
			}

			if (idx==0)	break;	// root�܂ŏオ���Ă����̂ŏI���Breturn�ł��ǂ�
		}
		idx = pNode->getBrotherNode();
	} while (idx);
	m_idx = 0;
	return FINISH;
}

void ScriptEngine::GotoOption(int idx)
{
	m_Context.SetState_GotoOption();
	run(idx);
	labelJump();
}

void ScriptEngine::labelJump()
{
	m_idx = 0;
	if (m_Context.m_label){
		m_Context.SetState_LabelSearch();
	} else {
		m_Context.SetState_Run();	// label���ȗ����ꂽ��擪����
	}
	if (m_Context.m_file){
		m_FileLoadReq = true;
		ScriptLoad(m_Context.m_file);
	}
}

//! �C�x���g���X�i�[��o�^����
void ScriptEngine::AddTextListener(avsTextListener* pListener)
{
	m_Context.m_pTextListener = pListener;
}

//! �ϐ����X�i�[��o�^����
void ScriptEngine::AddVariableListener(VariableListener* pListener)
{
	m_Context.m_pVariableListener = pListener;
}

//! �G���[���b�Z�[�W���X�i�[��o�^����
void ScriptEngine::AddErrorMessageListener(ErrorMessageListener* pListener)
{
	m_Context.m_pErrorMessageListener = pListener;
}

//! �C���[�W���X�i�[��o�^����
void ScriptEngine::AddImageListener(avsImageListener* pListener)
{
	m_Context.m_pImageListener = pListener;
}

//! �T�E���h���X�i�[��o�^����
void ScriptEngine::AddSoundListener(avsSoundListener* pListener)
{
	m_Context.m_pSoundListener = pListener;
}

//! 3D���X�i�[��o�^����
void ScriptEngine::Add3DListener(avs3DListener* pListener)
{
	m_Context.m_p3DListener = pListener;
}

const char *ScriptEngine::getHeadline() const
{
	return m_Context.m_Headline;
}

//! �W�����v�t�@�C�����𓾂�
const char* ScriptEngine::getJumpFile() const
{
	return m_Context.m_file;
}
