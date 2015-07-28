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
// 	コンストラクタ
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
 * スクリプトを新規にセットする場合に初期化
 *
 * SetScriptの直前でも直後でもよい。
 * LabelジャンプによりSetScriptが呼ばれた場合にはInitしない。
 */
void ScriptEngine::Init()
{
	m_Context.m_label = 0;
	m_Context.SetState_Run();
}

/**
 * エンジンにスクリプトをセットする
 *
 * スクリプトが読み込まれたらこの関数でエンジンに知らせる。
 * 同じバッファに読み込んだ場合でも、必要。
 */
void ScriptEngine::SetScript(const char *script ///< スクリプトのポインタ
							)
{	
	m_Context.m_ScriptBuf = script;
	m_Context.m_Headline = 0;	// スクリプトが変わるとリセット
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
		if (m_Context.m_file==0){	// 自ファイル内ラベルジャンプ
			goto LOOP;
		}
	}
	return stat;
}

RunningStatus ScriptEngine::run(int idx /**< 開始インデックス */)
{
	do {
LOOP:
		const miku::Node *pNode = reinterpret_cast<const miku::Node *>(&m_Context.m_ScriptBuf[idx]);
//	printf("[%d]", idx);
		if (pNode->isElementType()){
			const miku::ElementNode *pElement = static_cast<const miku::ElementNode *>(pNode);
			// タグを解釈して要素別にステートを移行
			m_Context.TagProc(pElement);
//	std::printf("enter %s\n", m_Context.TagName());
			// 属性の解釈。子要素スキップの判定をする場合がある
			bool child = m_Context.AttrProc(pElement);
			// 子要素の実行
			if (child && pElement->getChildNode()){
				idx = pElement->getChildNode();
				continue;
			}

			// 子要素を持たない場合のタグの終了
//	std::printf("terminate1 %s[%d]\n", m_Context.TagName(), idx);
			RunningStatus retval = m_Context.TagTerminateImm(pElement, idx);
			if (retval){
				m_idx = idx;
//	std::printf("term1 %s\n", m_Context.TagName());
				return retval;
			}
		} else {
			// テキストの処理
			const miku::TextNode *pText = static_cast<const miku::TextNode *>(pNode);
			m_Context.MsgProc(&m_Context.m_ScriptBuf[pText->entity]);
			MsgIdx(idx);	// 既読チェック
		}

		while (pNode->isTerminate()){
			// 次がなければ１階層上のエレメントに戻る
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<const miku::Node *>(&m_Context.m_ScriptBuf[idx]);
			const miku::ElementNode *pElement = static_cast<const miku::ElementNode *>(pNode);
			// タグの終了
//	std::printf("terminate2 %s[%d]\n", m_Context.TagName(), idx);
			RunningStatus retval = m_Context.TagTerminate(pElement, idx);
			if (retval==WHILE)	goto LOOP;
			if (retval){
				m_idx = idx;
//	std::printf("term2 %s\n", m_Context.TagName());
				return retval;
			}

			if (idx==0)	break;	// rootまで上がってきたので終了。returnでも良い
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
		m_Context.SetState_Run();	// labelが省略されたら先頭から
	}
	if (m_Context.m_file){
		m_FileLoadReq = true;
		ScriptLoad(m_Context.m_file);
	}
}

//! イベントリスナーを登録する
void ScriptEngine::AddTextListener(avsTextListener* pListener)
{
	m_Context.m_pTextListener = pListener;
}

//! 変数リスナーを登録する
void ScriptEngine::AddVariableListener(VariableListener* pListener)
{
	m_Context.m_pVariableListener = pListener;
}

//! エラーメッセージリスナーを登録する
void ScriptEngine::AddErrorMessageListener(ErrorMessageListener* pListener)
{
	m_Context.m_pErrorMessageListener = pListener;
}

//! イメージリスナーを登録する
void ScriptEngine::AddImageListener(avsImageListener* pListener)
{
	m_Context.m_pImageListener = pListener;
}

//! サウンドリスナーを登録する
void ScriptEngine::AddSoundListener(avsSoundListener* pListener)
{
	m_Context.m_pSoundListener = pListener;
}

//! 3Dリスナーを登録する
void ScriptEngine::Add3DListener(avs3DListener* pListener)
{
	m_Context.m_p3DListener = pListener;
}

const char *ScriptEngine::getHeadline() const
{
	return m_Context.m_Headline;
}

//! ジャンプファイル名を得る
const char* ScriptEngine::getJumpFile() const
{
	return m_Context.m_file;
}
