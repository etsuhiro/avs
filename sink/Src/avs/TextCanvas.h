/*! @file TextCanvas.h */
/*!
	テキストキャンバス
 */

#ifndef avs_text_canvas_h_
#define avs_text_canvas_h_

namespace avs
{
	struct SetupData;

	enum Status {
		EMPTY,
		BUSY,
		READY
	};

	class TextCanvas {
	public:
		TextCanvas(avs::SetupData& );
		~TextCanvas(void);

		void Put(const char *);
		void Update(int msec_elapse_time);
		void Draw();
		void Clear();
		void Flush();

		Status GetStatus(){ return tcStatus; }

	private:
		virtual void draw(int x, int y, const unsigned short *str, int n){}
		virtual void se(){}

		Status	tcStatus;

		int					m_nLine;	//!< アクティブなテキストの行数
		int					m_nChr;		//!< アクティブなテキストの文字数
		unsigned short*		m_pCurrent;	//!< 現在のバッファポインタ
		unsigned short*		m_pBuffer;	//!< テキストバッファ
		unsigned short**	m_pLinebuf;	//!< 各行のテキストポインター
		int*				m_nChrLine;	//!< 各行アクティブなテキスト文字数
		int					m_chrCnt;	//!< 現在表示される文字数
		int					m_pool;		//!< 文字送り用時間プール（ししおどしの竹）

		int					m_ChrSpeed;	//!< 文字送りの時間
		avs::SetupData&		m_Setup;	//!< セットアップデータ
	};
}

#endif
