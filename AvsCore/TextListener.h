/*! @file TextListener.h */
#pragma once

namespace avs
{
	/*!
		@brief ScriptEngineのテキスト用リスナー
		
		ScriptEngineにテキストの出力が発生するとコールバックされる。
	 */
	class avsTextListener {
	protected:
		~avsTextListener() {}
	public:
		enum TEXT_ATTR {
			DEFAULT		//!< 
		};

		//! テキスト出力
		virtual void Put(const char* str /**< テキストのポインタ */,
						 float time /**< テキストの表示時間 */,
						 int id) = 0;
		//! 改行を指定
		virtual void Crlf() = 0;
		//! 属性を指定
		virtual void Attr(TEXT_ATTR attr) = 0;
		//! 選択肢の出力
		virtual void Option(const char* str, /**< 選択肢テキストのポインタ */
							int num /**< 識別子（スクリプトインデックス）*/) = 0;
		//! テキストを消す
		virtual void Clear() = 0;
		//! クリック待ち要求
		virtual void Click() = 0;

		virtual void Window(float x, float y) = 0;

		virtual bool IsBusy() = 0;
	};
}
