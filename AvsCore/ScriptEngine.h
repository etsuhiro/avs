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

//! スクリプトエンジン
class ScriptEngine {
public:
	ScriptEngine();
	virtual ~ScriptEngine();

	//! 実行
	/*! スクリプトを進めるたびに呼ぶ
	 * @retval FINISH スクリプトが終了しました。
	 * @retval CLICK クリック待ち。再びRun()を呼ぶと続行する。
	 * @retval SELECT　選択肢。GotoOption()で選択する。
	 * @retval LABEL_JUMP　他のスクリプトへジャンプ。ファイル読み込みをし、SetScript()で知らせる。
	 */
	RunningStatus Run();

	void Init();
	void SetScript(const char *script);

	//! 指定した選択肢へジャンプ
	void GotoOption(int n);

	//! 実行開始のインデックスをセットする
	/*! ロードして再開、バックジャンプ等で使用 */
	void SetIndex(int idx){ m_idx = idx; }

	//! 実行インデックスを得る
	/*! この値をセーブし、ロード再開時に使用する */
	int GetIndex() const { return m_idx; }

	//! イベントリスナーを登録する
	/*! ただし登録できるのは1種類だけ */
	void AddTextListener(avsTextListener* pListener);

	//! 変数リスナーを登録する
	void AddVariableListener(VariableListener* pListener);

	//! エラーメッセージリスナーを登録する
	void AddErrorMessageListener(ErrorMessageListener* pListener);

	//! イメージリスナーを登録する
	void AddImageListener(avsImageListener* pListener);

	//! サウンドリスナーを登録する
	void AddSoundListener(avsSoundListener* pListener);

	//! 3Dリスナーを登録する
	void Add3DListener(avs3DListener* pListener);

	const char *getHeadline() const;

	//! ジャンプファイル名を得る
	const char* getJumpFile() const;

	//! 待ち時間を得る
	float getWaitTime() const;

	//! 通知イベントを得る
	int getEvent() const { return m_event; }

	//! 表示メッセージが発生したらスクリプトのインデックスを渡す
	/*! バックログ及び既読フラグセット用 */
	virtual void MsgIdx(int) {}

	/**
	 * スクリプトファイル読み込みリクエスト
	 *
	 * 他のスクリプトの読み込みリクエストが発生すると呼ばれるコールバック
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
