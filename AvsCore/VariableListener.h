/*! @file VariablListener.h */
/*!
	@brief 変数インターフェイス
	
	ScriptEngineにテキストの出力が発生するとコールバックされる。
 */

namespace avs
{

class VariableListener {
protected:
	~VariableListener(){}
public:

	//! 変数に値をセットする
	virtual void Set(const char* name, int val) = 0;
	//! 変数の値を得る
	virtual int Get(const char* name) = 0;
	//! キー情報を得る
	virtual int KeyOn(int key) = 0;
};

}
