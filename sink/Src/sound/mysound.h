#pragma	once

#include	<dsound.h>
#include	<vorbisfile.h>

namespace sound {

class MySound {
private:
	LPDIRECTSOUND			m_pDSound;

public:
	bool	Init( HWND& hWnd );
	void	Update( void );
	LPDIRECTSOUND&	DirectSnd(){ return	m_pDSound; }

public:
	MySound( void );
	~MySound( void );
};

class MySoundInd {
private:
	OggVorbis_File	m_Ogg;					// 開かれてるファイル格納
	char			m_Filename[512];		// 開かれてるファイル名
	char			m_Readname[512];		// 開こうとしているファイル名
	bool			m_PlayFlg;				// 再生フラグ
	bool			m_LoopFlg;				// ループフラグ
	int				m_NowBuf;				// ダブルバッファ系
	int				m_Volume;				// 音声量

	WAVEFORMATEX			m_WaveFmt;		// DirectSound
	DSBUFFERDESC			m_Desc;			// DirectSound
	IDirectSoundBuffer8*	m_DSBuf;		// DirectSound

	struct {
		int		fadeTime;		// フェードタイム
		long	startTime;		// 開始タイム
		int		volume;			// ボリューム
	} m_FadeParam;

	enum {
		IDLE,	// 待ち
		READ,	// 読み込み
		PLAY,	// 再生中
		PAUSE,	// 一時停止中
		STOP,	// 停止
		FOUT,	// フェードアウト
	} m_Mode;

private:
	void	Read( void );
	void	Decode( char* buffer, int bufsize );
	void	FadeoutProc( void );

public:
	void	Update( void );

	/*
		サウンドリクエスト
		name : ファイル名
		loop : ループフラグ[true/有 false/無]
	*/
	void	Req( const char* name, bool loop=false );

	/*
		再生チェック
		bool : 再生フラグ[ture/再生中 false/(一時)停止中]
	*/
	bool	IsPlay( void );

	/*
		一時停止
		再生再開時は停止した場所から
	*/
	void	Pause( void );

	/*
		停止
		再生再開時は初始めから
	*/
	void	Stop( void );

	/*
		再生再開
		読み込み音源がない場合はなにもしない
	*/
	void	Play( void );

	/*
		音量設定
		vol : 音量[0~127 0/無音 127/最大]
	*/
	void	SetVol( int vol );

	/*
		音量取得
		int : 音量[0~127 0/無音 127/最大]
	*/
	int		GetVol( void );

	/*
		フェードアウト
		fadeTime : フェードタイム[単位はms 1000で１秒]
	*/
	void	Fadeout( int fadeTime );

	/*
		パン設定
		pan : 偏り[-0x4000/左 0/中央 0x4000/右]
	*/
	void	SetPan( signed short pan );

public:
	MySoundInd();
	~ MySoundInd();
};

bool	SoundInit( HWND& hWnd );			// サウンド関連初期化
void	SoundUpdate( void );				// サウンド関連毎フレーム根っこで呼び出し

void	SoundStop( void );					// 全サウンド停止
void	SoundPause( void );					// 全サウンド一時停止
void	SoundPlay( void );					// 全サウンド再生

extern MySoundInd	Bgm;			// 背景音系
extern MySoundInd	Voice;			// 声系

}
