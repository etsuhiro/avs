#pragma once

namespace avs {
	//! 実行中の状態
	enum RunningStatus {
		CONTINUE = 0,	//!< 継続
		FINISH,			//!< スクリプトが終了
		WAIT,			//!< 時間待ち
		LABEL_JUMP,		//!< ラベルジャンプ
		SELECT,			//!< 選択待ち
		BUSY,			//!< なんらかの処理待ち
		CHILD_SCRIPT,	//!< マルチスクリプト
		WHILE,			//!< 繰り返し
		EVENT,			//!< イベント
		CLICK,
	};
}