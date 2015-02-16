/*! @file ScriptState.cpp */

#include "ScriptState.h"
#include "../sinkenum.h"
#include "../VariableListener.h"
#include "../TextListener.h"
#include "../ImageListener.h"
#include "../SoundListener.h"
#include "../ErrorMessagelistener.h"
#include "../3DListener.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>

using namespace avs;

#define STATIC_MEMBER(STATE)	STATE	ScriptContext::m_##STATE
STATIC_MEMBER(Run);
STATIC_MEMBER(Invalid);
STATIC_MEMBER(WaitClick);
STATIC_MEMBER(CmdP);
STATIC_MEMBER(CmdBr);
STATIC_MEMBER(CmdClick);
STATIC_MEMBER(CmdIf);
STATIC_MEMBER(CmdWhile);
STATIC_MEMBER(CmdGoto);
STATIC_MEMBER(LabelSearch);
STATIC_MEMBER(CmdHeadline);
STATIC_MEMBER(CmdSelect);
STATIC_MEMBER(CmdOption);
STATIC_MEMBER(GotoOption);
STATIC_MEMBER(CmdSet);
STATIC_MEMBER(CmdWait);
STATIC_MEMBER(CmdAsset);
STATIC_MEMBER(CmdCg);
STATIC_MEMBER(CmdCgt);
STATIC_MEMBER(CmdFade);
STATIC_MEMBER(CmdActor);
STATIC_MEMBER(CmdStage);
STATIC_MEMBER(CmdEvent);
STATIC_MEMBER(CmdCamera);
STATIC_MEMBER(CmdLight);
STATIC_MEMBER(CmdEffect);
STATIC_MEMBER(CmdSync);
STATIC_MEMBER(CmdDrawSync);
STATIC_MEMBER(CmdSe);
STATIC_MEMBER(CmdBgm);
STATIC_MEMBER(CmdStream);
STATIC_MEMBER(CmdMovie);
STATIC_MEMBER(CmdChild);
STATIC_MEMBER(CmdWindow);
STATIC_MEMBER(CmdSlow);

//----------------------------------------------------------------------------------------------
// コンテキスト
//
ScriptContext::ScriptContext()
	: m_ScriptBuf(0)
	, m_pTextListener(0)
	, m_pImageListener(0)
	, m_pSoundListener(0)
	, m_p3DListener(0)
	, m_pVariableListener(0)
	, m_pErrorMessageListener(0)
	, m_Headline(0)
	, m_label(0)
	, m_file(0)
	, m_pState(&m_Run)
{
	std::memset(m_var, 0, sizeof(m_var));
}

int ScriptContext::GetVariable(const char* str)
{
	if (str[0]=='$'){
		unsigned int n = str[1] - '0';
		if (n < 10 && str[2]==0){
			return m_var[n];
		} else {
			if (m_pVariableListener){
				return m_pVariableListener->Get(str);
			}
			return 0;
		}
	}
	return std::atoi(str);
}

void ScriptContext::SetVariable(const char* str, int val)
{
	if (str[0]=='$'){
		unsigned int n = str[1] - '0';
		if (n < 10 && str[2]==0){
			m_var[n] = val;
		} else {
			if (m_pVariableListener){
				m_pVariableListener->Set(str, val);
			}
		}
	}
}

void ScriptContext::SetState(ScriptState& state)
{
//	state.m_pPreviousState = m_pState;
	m_pState = &state;
}

const miku::Node* getNextNode(const miku::Node* pNode, const char *data)
{
	const void *p =  data + pNode->getNextNode();
	return static_cast<const miku::Node *>(p);
}

void ScriptContext::SetParentState(const miku::Node *pNode)
{
//	const char* current = TagName();

	while (pNode->hasNextNode()){
		pNode = getNextNode(pNode, m_ScriptBuf);
	};
	pNode = getNextNode(pNode, m_ScriptBuf);

	SetState(static_cast<const miku::ElementNode *>(pNode));
	//const char* parent = TagName();
	//char buf[256];
	//sprintf_s(buf, 256, "<%s>から親の<%s>に変更", current, parent);
	//System::Windows::Forms::MessageBox::Show( gcnew System::String(	buf ), "warning" );
}

void ScriptContext::TagProc(const miku::ElementNode *pElement)
{
	return m_pState->TagProc( *this, pElement );
}

bool ScriptContext::AttrProc(const miku::ElementNode *pElement)
{
	return m_pState->AttrProc( *this, pElement );
}

RunningStatus ScriptContext::TagTerminateImm(const miku::ElementNode *pElement, int index)
{
	return m_pState->TagTerminateImm( *this, pElement, index );
}

RunningStatus ScriptContext::TagTerminate(const miku::ElementNode *pElement, int index)
{
	return m_pState->TagTerminate( *this, pElement, index );
}

void ScriptContext::MsgProc(const char *str)
{
	m_pState->MsgProc( *this, str );
}

const char* ScriptContext::TagName() const
{
	return m_pState->TagName();
}

void ScriptContext::SetState(const miku::ElementNode *pElement)
{
	switch (pElement->name){
	case sink::element::SCRIPT:
	case sink::element::LABEL:
		SetState_Run();
		break;
	case sink::element::HEADLINE:
		SetState_CmdHeadline();
		break;
	case sink::element::P:
		SetState_CmdP();
		break;
	case sink::element::IF:
		SetState_CmdIf();
		break;
	case sink::element::WHILE:
		SetState_CmdWhile();
		break;
	case sink::element::GOTO:
		SetState_CmdGoto();
		break;
	case sink::element::SELECT:
		SetState_CmdSelect();
		break;
	case sink::element::SET:
		SetState_CmdSet();
		break;
	case sink::element::BR:
		SetState_CmdBr();
		break;
	case sink::element::CLICK:
		SetState_CmdClick();
		break;
	case sink::element::OPTION:
		SetState_CmdOption();
		break;
	case sink::element::WAIT:
		SetState_CmdWait();
		break;
	case sink::element::ASSET:
		SetState_CmdAsset();
		break;
	case sink::element::CG:
		SetState_CmdCg();
		break;
	case sink::element::CGT:
		SetState_CmdCgt();
		break;
	case sink::element::FADE:
		SetState_CmdFade();
		break;
	case sink::element::ACTOR:
		SetState_CmdActor();
		break;
	case sink::element::STAGE:
		SetState_CmdStage();
		break;
	case sink::element::EVENT:
		SetState_CmdEvent();
		break;
	case sink::element::CAMERA:
		SetState_CmdCamera();
		break;
	case sink::element::LIGHT:
		SetState_CmdLight();
		break;
	case sink::element::EFFECT:
		SetState_CmdEffect();
		break;
	case sink::element::SYNC:
		SetState_CmdSync();
		break;
	case sink::element::DRAWSYNC:
		SetState_CmdDrawSync();
		break;
	case sink::element::SE:
		SetState_CmdSe();
		break;
	case sink::element::BGM:
		SetState_CmdBgm();
		break;
	case sink::element::STREAM:
		SetState_CmdStream();
		break;
	case sink::element::MOVIE:
		SetState_CmdMovie();
		break;
	case sink::element::CHILD:
		SetState_CmdChild();
		break;
	case sink::element::WINDOW:
		SetState_CmdWindow();
		break;
	case sink::element::SLOW:
		SetState_CmdSlow();
		break;
	default:	// 該当しない子要素
		SetState_Invalid();
	}
}

void ScriptContext::ErrorInvalidElement(int cmd, int element)
{
	if (m_pErrorMessageListener){
		m_pErrorMessageListener->ErrorInvalidElement( cmd, element );
	}
}

void ScriptContext::ErrorInvalidAttr(int cmd, int attr)
{
	if (m_pErrorMessageListener){
		m_pErrorMessageListener->ErrorInvalidAttr( cmd, attr );
	}
}

void ScriptContext::ErrorRequiredAttr(int cmd, int attr)
{
	if (m_pErrorMessageListener){
		m_pErrorMessageListener->ErrorRequiredAttr( cmd, attr );
	}
}

void ScriptContext::ErrorInvalidText(const char *tag, const char* str)
{
	if (m_pErrorMessageListener){
		m_pErrorMessageListener->ErrorInvalidText( tag, str );
	}
}

void ScriptContext::ErrorUnfoundLabel(unsigned int label)
{
	if (m_pErrorMessageListener){
		m_pErrorMessageListener->ErrorUnfoundLabel( label );
	}
}

//-----------------------------------------------------------------------------------------------
// ステート
//-----------------------------------------------------------------------------------------------
// デフォルト
void ScriptState::TagProc(ScriptContext&, const miku::ElementNode *pElement)
{
}
bool ScriptState::AttrProc(ScriptContext&, const miku::ElementNode *pElement)
{
	return false;
}
RunningStatus ScriptState::TagTerminateImm(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	return TagTerminate(context, pElement, index);
}
RunningStatus ScriptState::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetParentState(pElement);
	return CONTINUE;
}
void ScriptState::MsgProc(ScriptContext& context, const char *str)
{
	context.ErrorInvalidText( TagName(), str );
}

// 無効ステート
const char* Invalid::TagName() const
{
	return "Invalid";
}

// クリック後
void WaitClick::MsgProc(ScriptContext& context, const char *str)
{
}

const char* WaitClick::TagName() const
{
	return "WaitClick";
}


//-----------------------------------------------------------------------------------------------
// Run
//-----------------------------------------------------------------------------------------------
void Run::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	switch (pElement->name){
	case sink::element::HEADLINE:
		context.SetState_CmdHeadline();
		break;
	case sink::element::P:
		context.SetState_CmdP();
		break;
	case sink::element::IF:
		context.SetState_CmdIf();
		break;
	case sink::element::WHILE:
		context.SetState_CmdWhile();
		break;
	case sink::element::GOTO:
		context.SetState_CmdGoto();
		break;
	case sink::element::SELECT:
		context.SetState_CmdSelect();
		break;
	case sink::element::SET:
		context.SetState_CmdSet();
		break;
	case sink::element::CLICK:
		context.SetState_CmdClick();
		break;
	case sink::element::WAIT:
		context.SetState_CmdWait();
		break;
	case sink::element::ASSET:
		context.SetState_CmdAsset();
		break;
	case sink::element::CG:
		context.SetState_CmdCg();
		break;
	case sink::element::CGT:
		context.SetState_CmdCgt();
		break;
	case sink::element::FADE:
		context.SetState_CmdFade();
		break;
	case sink::element::ACTOR:
		context.SetState_CmdActor();
		break;
	case sink::element::STAGE:
		context.SetState_CmdStage();
		break;
	case sink::element::EVENT:
		context.SetState_CmdEvent();
		break;
	case sink::element::CAMERA:
		context.SetState_CmdCamera();
		break;
	case sink::element::LIGHT:
		context.SetState_CmdLight();
		break;
	case sink::element::EFFECT:
		context.SetState_CmdEffect();
		break;
	case sink::element::SYNC:
		context.SetState_CmdSync();
		break;
	case sink::element::DRAWSYNC:
		context.SetState_CmdDrawSync();
		break;
	case sink::element::SE:
		context.SetState_CmdSe();
		break;
	case sink::element::BGM:
		context.SetState_CmdBgm();
		break;
	case sink::element::STREAM:
		context.SetState_CmdStream();
		break;
	case sink::element::MOVIE:
		context.SetState_CmdMovie();
		break;
	case sink::element::CHILD:
		context.SetState_CmdChild();
		break;
	case sink::element::WINDOW:
		context.SetState_CmdWindow();
		break;
	case sink::element::SLOW:
		context.SetState_CmdSlow();
		break;
	case sink::element::SCRIPT:
	case sink::element::LABEL:
		break;
	default:	// 該当しない子要素
		context.ErrorInvalidElement(sink::element::SCRIPT, pElement->name);
		context.SetState_Invalid();
	}
}

bool Run::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	return true;
}

RunningStatus Run::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	return CONTINUE;
}

const char* Run::TagName() const
{
	return "Run";
}

//-----------------------------------------------------------------------------------------------
// CmdP
//-----------------------------------------------------------------------------------------------
bool CmdP::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char* voice = 0;
//	const char* face = 0;
//	const char* name = 0;
	context.m_time = 0;
	context.m_id = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::id:
			context.m_id = pAttr->value;
			break;
		case sink::attribute::voice:
			voice = &context.m_ScriptBuf[pAttr->value];
			break;
//		case sink::attribute::face:
//			face = &context.m_ScriptBuf[pAttr->value];
//			break;
//		case sink::attribute::name:
//			name = &context.m_ScriptBuf[pAttr->value];
//			break;
		case sink::attribute::time:
			context.m_time = pAttr->fvalue;
			break;
		default:	//<!該当しない属性
			context.ErrorInvalidAttr(sink::element::P, pAttr->name);
			break;
		}
	}
	if (context.m_pTextListener){
		context.m_pTextListener->Clear();
	}
	if (context.m_pSoundListener){
		if (voice){
			context.m_pSoundListener->Voice(voice, context.m_id);
		}
	}
	return true;
}

// タグP内でつかえるタグ
void CmdP::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	switch (pElement->name){
	case sink::element::BR:
		context.SetState_CmdBr();
		break;
	case sink::element::CLICK:
		context.SetState_CmdClick();
		break;
	case sink::element::SELECT:
		context.SetState_CmdSelect();
		break;
	case sink::element::WAIT:
		context.SetState_CmdWait();
		break;
	default:	// 該当しない子要素
		context.ErrorInvalidElement(sink::element::P, pElement->name);
		context.SetState_Invalid();
	}
}

void CmdP::MsgProc(ScriptContext& context, const char *str)
{
	if (context.m_pTextListener){
		context.m_pTextListener->Put(str, context.m_time, context.m_id);
	}
}

RunningStatus CmdP::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
//	if (context.m_pTextListener){
//		context.m_pTextListener->Click();
//	}
	context.SetState_WaitClick();
//	MsgIdx(index);	// 既読チェック
	return BUSY;
}

const char* CmdP::TagName() const
{
	return "CmdP";
}

//-----------------------------------------------------------------------------------------------
// CmdHeadLine
//-----------------------------------------------------------------------------------------------
bool CmdHeadline::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	return true;	// 子要素に本文
}

void CmdHeadline::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	context.ErrorInvalidElement(sink::element::HEADLINE, pElement->name);
	context.SetState_Invalid();
}

void CmdHeadline::MsgProc(ScriptContext& context, const char *str)
{
	context.m_Headline = str;
}

RunningStatus CmdHeadline::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdHeadline::TagName() const
{
	return "CmdHeadline";
}

//-----------------------------------------------------------------------------------------------
// CmdBr
//-----------------------------------------------------------------------------------------------
bool CmdBr::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	return false;	// 子要素を持たない
}

void CmdBr::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	assert(false);
}

RunningStatus CmdBr::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_pTextListener){
		context.m_pTextListener->Crlf();
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdBr::TagName() const
{
	return "CmdBr";
}

//-----------------------------------------------------------------------------------------------
// CmdClick
//-----------------------------------------------------------------------------------------------
RunningStatus CmdClick::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_pTextListener){
		context.m_pTextListener->Click();
	}
	context.SetState_WaitClick();
	return BUSY;
}

const char* CmdClick::TagName() const
{
	return "CmdClick";
}

//-----------------------------------------------------------------------------------------------
// CmdGoto
//-----------------------------------------------------------------------------------------------
bool CmdGoto::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	context.m_label = 0;	// デフォルトはトップ
	context.m_file = 0;		// デフォルトは同スクリプトファイル
	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::label:
			context.m_label = pAttr->value;
			break;
		case sink::attribute::file:
			context.m_file = &context.m_ScriptBuf[pAttr->value];
			break;
		default:
			context.ErrorInvalidAttr(sink::element::GOTO, pAttr->name);
		}
	}
	if (context.m_file==0 && context.m_label==0){
		if (context.m_pErrorMessageListener){
			context.m_pErrorMessageListener->ErrorRequiredAttr(sink::element::GOTO, sink::attribute::label);
		}
	}
	return false;
}

void CmdGoto::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	assert(false);
}

RunningStatus CmdGoto::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	return LABEL_JUMP;
}

const char* CmdGoto::TagName() const
{
	return "CmdGoto";
}

//-----------------------------------------------------------------------------------------------
// CmdIf
//-----------------------------------------------------------------------------------------------
bool CmdIf::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char* name=0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();

	switch (pAttr->name){
	case sink::attribute::var:
		name = &context.m_ScriptBuf[pAttr->value];
		pAttr++;
		break;
	case sink::attribute::key:
		if (context.m_pVariableListener==0)	return false;
		return context.m_pVariableListener->KeyOn(pAttr->value);
	default:
		context.ErrorRequiredAttr(sink::element::SET, sink::attribute::var);
		return false;
	}

	for ( ; pAttr != end; pAttr++){
		const char* val = &context.m_ScriptBuf[pAttr->value];
		int rval = context.GetVariable(val);
		int n = context.GetVariable(name);

		switch (pAttr->name){
		case sink::attribute::eq:
			return n==rval;
		case sink::attribute::ne:
			return n!=rval;
		case sink::attribute::gt:
			return n>rval;
		case sink::attribute::ge:
			return n>=rval;
		case sink::attribute::lt:
			return n<rval;
		case sink::attribute::le:
			return n<=rval;
		default:
			context.ErrorInvalidAttr(sink::element::SET, pAttr->name);
		}
	}
	return false;
}

void CmdIf::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	//if (pElement->name == sink::element::IF){
	//	// <if>のネストはできない
	//	if (context.m_pErrorMessageListener){
	//		context.m_pErrorMessageListener->ErrorInvalidElement(sink::element::IF, pElement->name);
	//	}
	//	context.SetState_Invalid();
	//} else {
//		m_pPreviousState->TagProc(context, pElement);	// 親タグとして処理をする
		context.SetState(pElement);
	//}
}

RunningStatus CmdIf::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdIf::TagName() const
{
	return "CmdIf";
}

//-----------------------------------------------------------------------------------------------
// CmdWhile
//-----------------------------------------------------------------------------------------------
RunningStatus CmdWhile::TagTerminateImm(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetParentState(pElement);
	return CONTINUE;
}

RunningStatus CmdWhile::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	// 繰り返し実行される
	return WHILE;
}

const char* CmdWhile::TagName() const
{
	return "CmdWhile";
}

//-----------------------------------------------------------------------------------------------
// CmdSelect
//-----------------------------------------------------------------------------------------------
bool CmdSelect::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	// タイムリミットかな
	return true;
}

void CmdSelect::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	switch (pElement->name){
	case sink::element::IF:
		context.SetState_CmdIf();
		break;
	case sink::element::OPTION:
		context.SetState_CmdOption();
		break;
	default:	// 該当しない子要素
		context.ErrorInvalidElement(sink::element::SELECT, pElement->name);
		context.SetState_Invalid();
	}
}

RunningStatus CmdSelect::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetParentState(pElement);
	return SELECT;
}

const char* CmdSelect::TagName() const
{
	return "CmdSelect";
}

//-----------------------------------------------------------------------------------------------
// CmdOption
//-----------------------------------------------------------------------------------------------
bool CmdOption::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	context.m_label = 0;
	context.m_file = 0;
	const miku::Attr *pAttr = pElement->getAttrPtr();
	for (int i=0; i<pElement->getAttrNum(); i++, pAttr++){
		switch (pAttr->name){
		case sink::attribute::label:
			context.m_label = pAttr->value;
			break;
		case sink::attribute::file:
			context.m_file = &context.m_ScriptBuf[pAttr->value];
			break;
		default:
			context.ErrorInvalidAttr(sink::element::OPTION, pAttr->name);
		}
	}
	if (context.m_file==0 && context.m_label==0){
		if (context.m_pErrorMessageListener){
			context.m_pErrorMessageListener->ErrorRequiredAttr(sink::element::OPTION, sink::attribute::label);
		}
	}
	return true;
}

void CmdOption::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	context.ErrorInvalidElement(sink::element::OPTION, pElement->name);
	context.SetState_Invalid();
}

static const char* optionMsg;

void CmdOption::MsgProc(ScriptContext& context, const char *str)
{
	optionMsg = str;
}

RunningStatus CmdOption::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_pTextListener){
		context.m_pTextListener->Option(optionMsg, index);
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdOption::TagName() const
{
	return "CmdOption";
}

//-----------------------------------------------------------------------------------------------
// GotoOption
//-----------------------------------------------------------------------------------------------
bool GotoOption::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	context.m_label = 0;
	context.m_file = 0;
	const miku::Attr *pAttr = pElement->getAttrPtr();
	for (int i=0; i<pElement->getAttrNum(); i++, pAttr++){
		switch (pAttr->name){
		case sink::attribute::label:
			context.m_label = pAttr->value;
			break;
		case sink::attribute::file:
			context.m_file = &context.m_ScriptBuf[pAttr->value];
			break;
		}
	}
	return false;
}

void GotoOption::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
}

void GotoOption::MsgProc(ScriptContext& context, const char *str)
{
}

RunningStatus GotoOption::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	return LABEL_JUMP;
}

const char* GotoOption::TagName() const
{
	return "GotoOption";
}

//-----------------------------------------------------------------------------------------------
// CmdSet
//-----------------------------------------------------------------------------------------------
bool CmdSet::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char* name=0;

	const miku::Attr *pAttr = pElement->getAttrPtr();

	if (pAttr->name==sink::attribute::var){
		name = &context.m_ScriptBuf[pAttr->value];
		pAttr++;
	} else {
		context.ErrorRequiredAttr(sink::element::SET, sink::attribute::var);
		return false;
	}

	for (int i=1; i<pElement->getAttrNum(); i++, pAttr++){
		const char* val = &context.m_ScriptBuf[pAttr->value];
		int rval = context.GetVariable(val);
		int n = context.GetVariable(name);

		switch (pAttr->name){
		case sink::attribute::set:
			context.SetVariable(name, rval);
			break;
		case sink::attribute::add:
			context.SetVariable(name, n+rval);
			break;
		case sink::attribute::sub:
			context.SetVariable(name, n-rval);
			break;
		case sink::attribute::min:
			if (n<rval)
				context.SetVariable(name, rval);
			break;
		case sink::attribute::max:
			if (n>rval)
			context.SetVariable(name, rval);
			break;
		default:
			context.ErrorInvalidAttr(sink::element::SET, pAttr->name);
		}
	}
	return false;
}

void CmdSet::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	assert(false);
}

RunningStatus CmdSet::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdSet::TagName() const
{
	return "CmdSet";
}

//-----------------------------------------------------------------------------------------------
// LabelSearch
//-----------------------------------------------------------------------------------------------
bool LabelSearch::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	return (pElement->name == sink::element::SCRIPT) || (pElement->name == sink::element::IF);
}

void LabelSearch::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
}

RunningStatus LabelSearch::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (pElement->name == sink::element::LABEL){
		const miku::Attr *pAttr = pElement->getAttrPtr();
		const miku::Attr *end = pAttr + pElement->getAttrNum();
		for ( ; pAttr != end; pAttr++){
			switch (pAttr->name){
			case sink::attribute::id:
				if (context.m_label == pAttr->value){
					context.SetState_Run();
				}
				return CONTINUE;
			}
		}
	}
	return CONTINUE;
}

const char* LabelSearch::TagName() const
{
	return "LabelSearch";
}

//-----------------------------------------------------------------------------------------------
// CmdWwait
//-----------------------------------------------------------------------------------------------
bool CmdWait::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	context.m_WaitTime = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::time:
			context.m_WaitTime = pAttr->fvalue;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::WAIT, pAttr->name);
		}
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdWait::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetState_WaitClick();
	return WAIT;
}

const char* CmdWait::TagName() const
{
	return "CmdWait";
}

//-----------------------------------------------------------------------------------------------
// CmdAsset
//-----------------------------------------------------------------------------------------------
bool CmdAsset::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char* file = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		default:
			context.ErrorInvalidAttr(sink::element::ASSET, pAttr->name);
		}
	}
	if (file==0){
		if (context.m_pErrorMessageListener){
			context.m_pErrorMessageListener->ErrorRequiredAttr(sink::element::ASSET, sink::attribute::file);
		}
	}
	if (context.m_pImageListener){
		if (file[0]=='-')
			context.m_pImageListener->ReleaseAsset(file+1);
		else
			context.m_pImageListener->AddAsset(file);
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdAsset::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_pImageListener){
		if (context.m_pImageListener->IsBusy()){
			context.SetState_WaitClick();
			return BUSY;
		}
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdAsset::TagName() const
{
	return "CmdAsset";
}

//-----------------------------------------------------------------------------------------------
// CmdCg
//-----------------------------------------------------------------------------------------------
void CmdCg::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	switch (pElement->name){
	case sink::element::CG:
		break;
	default:	// 該当しない子要素
		context.ErrorInvalidElement(sink::element::SELECT, pElement->name);
		context.SetState_Invalid();
	}
}

bool CmdCg::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	int id = 0;
	const char* file = 0;
	int parent = 0;
	ImageInfo info = {
			0,0,
			1.f,1.f,
			1.f,1.f,1.f,1.f,
			0,0,0,0,
			0,
			0,};

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::id:
			id = pAttr->value;
			break;
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			if (id==0){	// idが未定義なら自動セット
			}
			break;
		case sink::attribute::parent:
			parent = pAttr->value;
			break;
		case sink::attribute::x:
			info.x = pAttr->fvalue;
			break;
		case sink::attribute::y:
			info.y = pAttr->fvalue;
			break;
		case sink::attribute::r:
			info.r = pAttr->fvalue;
			break;
		case sink::attribute::g:
			info.g = pAttr->fvalue;
			break;
		case sink::attribute::b:
			info.b = pAttr->fvalue;
			break;
		case sink::attribute::a:
			info.a = pAttr->fvalue;
			break;
		case sink::attribute::u:
			info.u = pAttr->value;
			break;
		case sink::attribute::v:
			info.v = pAttr->value;
			break;
		case sink::attribute::w:
			info.w = pAttr->value;
			break;
		case sink::attribute::h:
			info.h = pAttr->value;
			break;
		case sink::attribute::hscale:
			info.hscale = pAttr->fvalue;
			break;
		case sink::attribute::vscale:
			info.vscale = pAttr->fvalue;
			break;
		case sink::attribute::rot:
			info.rot = pAttr->fvalue;
			break;
		case sink::attribute::priority:
			info.priority = pAttr->value;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::CG, pAttr->name);
		}
	}
	if (context.m_pImageListener){
		context.m_pImageListener->Cg(id, file, info, parent);
	}
	return true;	// 子要素を持つ
}

RunningStatus CmdCg::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_pImageListener){
		if (context.m_pImageListener->IsBusy()){
			// stateを変えない。ロードが終わったら再び実行されるように。
			return BUSY;
		}
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdCg::TagName() const
{
	return "CmdCg";
}

//-----------------------------------------------------------------------------------------------
// CmdCgt
//-----------------------------------------------------------------------------------------------
bool CmdCgt::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	int id = 0;
	ImageInfo info;
	unsigned int change = 0;
	float time = 0;
	float offset = 0;
	int method = 0;
	const char* action = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::id:
			id = pAttr->value;
			break;
		case sink::attribute::x:
			info.x = pAttr->fvalue;
			change |= CHG_X;
			break;
		case sink::attribute::y:
			info.y = pAttr->fvalue;
			change |= CHG_Y;
			break;
		case sink::attribute::r:
			info.r = pAttr->fvalue;
			change |= CHG_R;
			break;
		case sink::attribute::g:
			info.g = pAttr->fvalue;
			change |= CHG_G;
			break;
		case sink::attribute::b:
			info.b = pAttr->fvalue;
			change |= CHG_B;
			break;
		case sink::attribute::a:
			info.a = pAttr->fvalue;
			change |= CHG_A;
			break;
		case sink::attribute::u:
			info.u = pAttr->value;
			change |= CHG_U;
			break;
		case sink::attribute::v:
			info.v = pAttr->value;
			change |= CHG_V;
			break;
		case sink::attribute::w:
			info.w = pAttr->value;
			change |= CHG_W;
			break;
		case sink::attribute::h:
			info.h = pAttr->value;
			change |= CHG_H;
			break;
		case sink::attribute::hscale:
			info.hscale = pAttr->fvalue;
			change |= CHG_HSCALE;
			break;
		case sink::attribute::vscale:
			info.vscale = pAttr->fvalue;
			change |= CHG_VSCALE;
			break;
		case sink::attribute::rot:
			info.rot = pAttr->fvalue;
			change |= CHG_ROT;
			break;
		case sink::attribute::priority:
			info.priority = pAttr->value;
			change |= CHG_PRIORITY;
			break;
		case sink::attribute::time:
			time = pAttr->fvalue;
			break;
		case sink::attribute::scale:
			offset = pAttr->fvalue;
			break;
		case sink::attribute::method:
			method = pAttr->value;
			break;
		case sink::attribute::action:
			action = &context.m_ScriptBuf[pAttr->value];
			break;
		default:
			context.ErrorInvalidAttr(sink::element::CGT, pAttr->name);
		}
	}
	if (context.m_pImageListener){
		context.m_pImageListener->CgTrans(id, time, info, change, method, offset, action);
	}
	return false;	// 子要素を持たない
}

const char* CmdCgt::TagName() const
{
	return "CmdCgt";
}

//-----------------------------------------------------------------------------------------------
// CmdFade
//-----------------------------------------------------------------------------------------------
bool CmdFade::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	float r = 0;
	float g = 0;
	float b = 0;
	float a = 0;
	float time = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::r:
			r = pAttr->fvalue;
			break;
		case sink::attribute::g:
			g = pAttr->fvalue;
			break;
		case sink::attribute::b:
			b = pAttr->fvalue;
			break;
		case sink::attribute::a:
			a = pAttr->fvalue;
			break;
		case sink::attribute::time:
			time = pAttr->fvalue;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::FADE, pAttr->name);
		}
	}
	if (context.m_pImageListener){
		context.m_pImageListener->Fade(time, a, r, g, b);
	}
	return false;	// （今は）子要素を持たない
}

const char* CmdFade::TagName() const
{
	return "CmdFade";
}

//-----------------------------------------------------------------------------------------------
// CmdActor
//-----------------------------------------------------------------------------------------------
bool CmdActor::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char* name = 0;
	const char* action = 0;
	const char* file = 0;
	const char* joint = 0;
	const char* add = 0;
	const char* set = 0;
	const char* asset = 0;
	const char* group = 0;
	unsigned int change = 0;
	float x=0,y=0,z=0,rot=0,scale=0;
	int loop = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::name:
			name = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::action:
			action = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::asset:
			asset = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::group:
			group = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::joint:
			joint = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::add:
			add = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::set:
			set = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::x:
			x = pAttr->fvalue;
			change |= 1<<0;
			break;
		case sink::attribute::y:
			y = pAttr->fvalue;
			change |= 1<<1;
			break;
		case sink::attribute::z:
			z = pAttr->fvalue;
			change |= 1<<2;
			break;
		case sink::attribute::rot:
			rot = pAttr->fvalue;
			change |= 1<<3;
			break;
		case sink::attribute::scale:
			scale = pAttr->fvalue;
			change |= 1<<4;
			break;
		case sink::attribute::loop:
			loop = pAttr->value;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::ACTOR, pAttr->name);
		}
	}
	if (context.m_p3DListener){
		if (asset){
			if (file){
				if (action){
//					context.m_p3DListener->ActorAsset(name, action, asset, 0);
					context.m_p3DListener->ActorAsset(asset, action, file, 0);
				} else {
					context.m_p3DListener->ActorAsset(asset, 0, file, 0);
				}
			} else {
				context.m_p3DListener->ActorAsset(name, action, asset, 0);
			}
		} else
		if (file){
			context.m_p3DListener->ActorAsset(name, action, file, group);
		} else
		if (add)
			context.m_p3DListener->ActorConnect(name, add, joint);
		else
			context.m_p3DListener->Actor(name, action, x, y, z, rot, scale, change, loop, set);
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdActor::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_p3DListener){
		if (context.m_p3DListener->IsBusy()){
			context.SetState_WaitClick();
			return BUSY;
		}
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdActor::TagName() const
{
	return "CmdActor";
}

//-----------------------------------------------------------------------------------------------
// CmdStage
//-----------------------------------------------------------------------------------------------
bool CmdStage::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	int id = 0;
	const char *file = 0;
	const char *name = 0;
	int draw = -1;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::id:
			id = pAttr->value;
			break;
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::name:
			name = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::draw:
			draw = pAttr->value;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::STAGE, pAttr->name);
		}
	}
	if (context.m_p3DListener){
		if (file){
			context.m_p3DListener->Stage(id, file, name, (draw!=0));	// ステージ登録
		} else if (draw>=0){
			if (id)
				context.m_p3DListener->StageDisp(id, (draw!=0));
			else
				context.m_p3DListener->StageDisp((draw!=0));
		} else {
			if (id)
				context.m_p3DListener->StageRemove(id);	// 指定されたステージ削除
			else
				context.m_p3DListener->StageRemove();	// 全ステージ削除
		}
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdStage::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_p3DListener){
		if (context.m_p3DListener->IsBusy()){
			// stateを変えない。ロードが終わったら再び実行されるように。
			return BUSY;
		}
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdStage::TagName() const
{
	return "CmdStage";
}

//-----------------------------------------------------------------------------------------------
// CmdEvent
//-----------------------------------------------------------------------------------------------
bool CmdEvent::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	int id = 0;
	const char *file = 0;
	const char *action = 0;
	int method = 0;
	float x=0.0f,y=0.0f,z=0.0f;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::id:
			id = pAttr->value;
			break;
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::action:
			action = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::method:
			method = pAttr->value;
			break;
		case sink::attribute::x:
			x = pAttr->fvalue;
			break;
		case sink::attribute::y:
			y = pAttr->fvalue;
			break;
		case sink::attribute::z:
			z = pAttr->fvalue;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::EVENT, pAttr->name);
		}
	}
	if (context.m_p3DListener){
		context.m_p3DListener->Event(id, file, action, method,x,y,z);
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdEvent::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_p3DListener){
		if (context.m_p3DListener->IsBusy()){
			context.SetState_WaitClick();
			return BUSY;
		}
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdEvent::TagName() const
{
	return "CmdEvent";
}

//-----------------------------------------------------------------------------------------------
// CmdCamera
//-----------------------------------------------------------------------------------------------
bool CmdCamera::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char *file = 0;
	const char *name = 0;
	const char *set = 0;
	float near = 0;
	float far = 0;
	float fov = 0;
	int loop = 0;
	int frame = 0;
	float x=0,y=0,z=0,rot=0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::name:
			name = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::set:
			set = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::near:
			near = pAttr->fvalue;
			break;
		case sink::attribute::far:
			far = pAttr->fvalue;
			break;
		case sink::attribute::fov:
			fov = pAttr->fvalue;
			break;
		case sink::attribute::loop:
			loop = pAttr->value;
			break;
		case sink::attribute::frame:
			frame = pAttr->value;
			break;
		case sink::attribute::x:
			x = pAttr->fvalue;
			break;
		case sink::attribute::y:
			y = pAttr->fvalue;
			break;
		case sink::attribute::z:
			z = pAttr->fvalue;
			break;
		case sink::attribute::rot:
			rot = pAttr->fvalue;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::CAMERA, pAttr->name);
		}
	}
	if (context.m_p3DListener){
		context.m_p3DListener->Camera(file, name, near, far, fov, loop, frame, set, x, y, z, rot);
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdCamera::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_p3DListener){
		if (context.m_p3DListener->IsBusy()){
			// stateを変えない。ロードが終わったら再び実行されるように。
			return BUSY;
		}
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdCamera::TagName() const
{
	return "CmdCamera";
}

//-----------------------------------------------------------------------------------------------
// CmdLight
//-----------------------------------------------------------------------------------------------
bool CmdLight::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	int id = 0;
	const char *file = 0;
	const char *name = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::id:
			id = pAttr->value;
			break;
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::name:
			name = &context.m_ScriptBuf[pAttr->value];
			break;
		default:
			context.ErrorInvalidAttr(sink::element::LIGHT, pAttr->name);
		}
	}
	if (context.m_p3DListener){
		context.m_p3DListener->Light(id, file, name);
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdLight::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_p3DListener){
		if (context.m_p3DListener->IsBusy()){
			// stateを変えない。ロードが終わったら再び実行されるように。
			return BUSY;
		}
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdLight::TagName() const
{
	return "CmdLight";
}

//-----------------------------------------------------------------------------------------------
// CmdEffect
//-----------------------------------------------------------------------------------------------
bool CmdEffect::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char* file = 0;
	const char* actor = 0;
	const char* joint = 0;
	float x=0,y=0,z=0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::name:
			actor = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::joint:
			joint = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::x:
			x = pAttr->fvalue;
			break;
		case sink::attribute::y:
			y = pAttr->fvalue;
			break;
		case sink::attribute::z:
			z = pAttr->fvalue;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::EFFECT, pAttr->name);
		}
	}
	if (context.m_p3DListener){
		context.m_p3DListener->Effect(file, actor, joint, x, y, z);
	}
	return false;	// 子要素を持たない
}

const char* CmdEffect::TagName() const
{
	return "CmdEffect";
}

//-----------------------------------------------------------------------------------------------
// CmdSync
//-----------------------------------------------------------------------------------------------
bool CmdSync::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	int id = 0;
	int frame = -1;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::id:
			id = pAttr->value;
			break;
		case sink::attribute::frame:
			frame = pAttr->value;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::SYNC, pAttr->name);
		}
	}
	if (context.m_p3DListener){
		context.m_p3DListener->Sync(id, frame);
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdSync::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetState_WaitClick();
	return BUSY;
}

const char* CmdSync::TagName() const
{
	return "CmdSync";
}

//-----------------------------------------------------------------------------------------------
// CmdDrawSync
//-----------------------------------------------------------------------------------------------
void CmdDrawSync::TagProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	switch (pElement->name){
	case sink::element::IF:
		context.SetState_CmdIf();
		break;
	case sink::element::CG:
		context.SetState_CmdCg();
		break;
	case sink::element::ACTOR:
		context.SetState_CmdActor();
		break;
	case sink::element::STAGE:
		context.SetState_CmdStage();
		break;
	case sink::element::EVENT:
		context.SetState_CmdEvent();
		break;
	case sink::element::CAMERA:
		context.SetState_CmdCamera();
		break;
	default:	// 該当しない子要素
		context.ErrorInvalidElement(sink::element::DRAWSYNC, pElement->name);
		context.SetState_Invalid();
	}
}

bool CmdDrawSync::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		context.ErrorInvalidAttr(sink::element::DRAWSYNC, pAttr->name);
	}
	if (context.m_p3DListener){
		context.m_p3DListener->DrawSync(true);	// drawを止める
	}
	return true;	// 子要素を持つ
}

RunningStatus CmdDrawSync::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	if (context.m_p3DListener){
		context.m_p3DListener->DrawSync(false);	// draw再開
	}
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdDrawSync::TagName() const
{
	return "CmdDrawSync";
}

//-----------------------------------------------------------------------------------------------
// CmdSe
//-----------------------------------------------------------------------------------------------
bool CmdSe::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	int id = 0;
	const char* file = 0;
	float volume = 1.f;
	float pan = 0.f;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::id:
			id = pAttr->value;
			break;
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::volume:
			volume = pAttr->fvalue;
			break;
		case sink::attribute::pan:
			pan = pAttr->fvalue;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::SE, pAttr->name);
		}
	}
	if (context.m_pSoundListener){
		context.m_pSoundListener->Se(id, file, volume, pan);
	}
	return false;	// 子要素を持たない
}

const char* CmdSe::TagName() const
{
	return "CmdSe";
}

//-----------------------------------------------------------------------------------------------
// CmdBgm
//-----------------------------------------------------------------------------------------------
bool CmdBgm::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char* file = 0;
	const char* action = 0;
	float volume = 1.f;
	float fadeTime = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::action:
			action = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::volume:
			volume = pAttr->fvalue;
			break;
		case sink::attribute::time:
			fadeTime = pAttr->fvalue;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::BGM, pAttr->name);
		}
	}
	if (context.m_pSoundListener){
		context.m_pSoundListener->Bgm(file, action, volume, fadeTime);
	}
	return false;	// 子要素を持たない
}

const char* CmdBgm::TagName() const
{
	return "CmdBgm";
}

//-----------------------------------------------------------------------------------------------
// CmdStream
//-----------------------------------------------------------------------------------------------
bool CmdStream::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char* file = 0;
	float volume = 1.f;
	int	sync = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		case sink::attribute::volume:
			volume = pAttr->fvalue;
			break;
		case sink::attribute::sync:
			sync = pAttr->value;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::STREAM, pAttr->name);
		}
	}
	if (context.m_pSoundListener){
		context.m_pSoundListener->Stream(file, volume, sync);
	}
	return false;	// 子要素を持たない
}

const char* CmdStream::TagName() const
{
	return "CmdStream";
}

//-----------------------------------------------------------------------------------------------
// CmdMovie
//-----------------------------------------------------------------------------------------------
bool CmdMovie::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	const char* file = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::file:
			file = &context.m_ScriptBuf[pAttr->value];
			break;
		default:
			context.ErrorInvalidAttr(sink::element::MOVIE, pAttr->name);
		}
	}
	if (context.m_pSoundListener){
		context.m_pSoundListener->Movie(file);
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdMovie::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetState_WaitClick();
	return BUSY;
}

const char* CmdMovie::TagName() const
{
	return "CmdMovie";
}

//-----------------------------------------------------------------------------------------------
// CmdChild
//-----------------------------------------------------------------------------------------------
bool CmdChild::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	context.m_file = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::file:
			context.m_file = &context.m_ScriptBuf[pAttr->value];
			break;
		default:
			context.ErrorInvalidAttr(sink::element::CHILD, pAttr->name);
		}
	}
	if (context.m_file==0){
		if (context.m_pErrorMessageListener){
			context.m_pErrorMessageListener->ErrorRequiredAttr(sink::element::CHILD, sink::attribute::file);
		}
	}
	return false;
}

RunningStatus CmdChild::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetState_WaitClick();
	return CHILD_SCRIPT;
}

const char* CmdChild::TagName() const
{
	return "CmdChild";
}

//-----------------------------------------------------------------------------------------------
// CmdSlow
//-----------------------------------------------------------------------------------------------
bool CmdSlow::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	float time = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::time:
			time = pAttr->fvalue;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::SLOW, pAttr->name);
		}
	}
	if (context.m_p3DListener){
		context.m_p3DListener->Slow(time);
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdSlow::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdSlow::TagName() const
{
	return "CmdSlow";
}

//-----------------------------------------------------------------------------------------------
// CmdWindow
//-----------------------------------------------------------------------------------------------
bool CmdWindow::AttrProc(ScriptContext& context, const miku::ElementNode *pElement)
{
	float x = 0;
	float y = 0;

	const miku::Attr *pAttr = pElement->getAttrPtr();
	const miku::Attr *end = pAttr + pElement->getAttrNum();
	for ( ; pAttr != end; pAttr++){
		switch (pAttr->name){
		case sink::attribute::x:
			x = pAttr->fvalue;
			break;
		case sink::attribute::y:
			y = pAttr->fvalue;
			break;
		default:
			context.ErrorInvalidAttr(sink::element::WINDOW, pAttr->name);
		}
	}
	if (context.m_pTextListener){
		context.m_pTextListener->Window(x, y);
	}
	return false;	// 子要素を持たない
}

RunningStatus CmdWindow::TagTerminate(ScriptContext& context, const miku::ElementNode *pElement, int index)
{
	context.SetParentState(pElement);
	return CONTINUE;
}

const char* CmdWindow::TagName() const
{
	return "CmdWindow";
}