#pragma once

#include "AvsCore/ScriptEngine.h"
#include "AvsCore/ErrorMessageListener.h"
#include <vcclr.h>
#include "LogWindow.h"

namespace yayoi {

	class YayoiScriptEngine : public avs::ScriptEngine, public avs::ErrorMessageListener {
	public:
		YayoiScriptEngine(XmlEnum&, LogWindow^);
		int GetMsgIndex() { return m_MsgIndex; }

	private:
		virtual void ErrorInvalidElement(int cmd, int element);
		virtual void ErrorInvalidAttr(int cmd, int attr);
		virtual void ErrorrequiredAttr(int cmd, int attr);
		virtual void ErrorInvalidText(const char *tag, const char* str);
		virtual void ErrorUnfoundLabel(unsigned int label);
		virtual void MsgIdx(int idx);
		virtual void ScriptLoad(const char *filename);

		XmlEnum&	m_mikuEnum;
		int			m_MsgIndex;
		const char*	m_ReqLoadFileName;
		gcroot<LogWindow^>	m_LogWindow;
	};

}
