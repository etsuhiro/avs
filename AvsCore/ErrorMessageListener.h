/*! @file ErrorMessage.h */

namespace avs
{
	/*!
		@brief ScriptEngineのエラーメッセージリスナー
		
		ScriptEngineにエラーが発生した場合、メッセージをコールバックする。
	 */
	class ErrorMessageListener {
	public:
		virtual ~ErrorMessageListener() {}

		//! スクリプトエラー 無効な子要素
		virtual void ErrorInvalidElement(int cmd, int element) = 0;
		//! スクリプトエラー 無効な属性
		virtual void ErrorInvalidAttr(int cmd, int attr) = 0;
		//! スクリプトエラー 必須の属性がない
		virtual void ErrorRequiredAttr(int cmd, int attr) = 0;
		//! スクリプトエラー 無効な文字データ
		virtual void ErrorInvalidText(const char *tag, const char* str) = 0;
		//! スクリプトエラー ジャンプ先のラベルが見つからない
		virtual void ErrorUnfoundLabel(unsigned int label) = 0;
	};
}
