
#include	<d3d9.h>

#if	0
#pragma	comment( lib, "../ogg_lib/libogg_static" )
#pragma	comment( lib, "../ogg_lib/libvorbis_static" )
#pragma	comment( lib, "../ogg_lib/libvorbisfile_static" )
#endif

#include	"../Setup.h"
#include	"mysound.h"

extern WindowsSetupData g_SetupData;

namespace sound {

MySoundInd	Bgm;
MySoundInd	Voice;

void	ErrorMessage( HRESULT result )
{
	switch( result ){
	case DS_NO_VIRTUALIZATION:
		MessageBox( NULL, L"バッファ作成は成功したが他の３Ｄアルゴリズムが代用されました", L"MySound", MB_OK|MB_ICONSTOP); break;
#if	0
	case DS_INCOMPLETE:
		MessageBox( NULL, L"メソッドは成功したが、得られていないオプションエフェクトもあります", L"MySound", MB_OK|MB_ICONSTOP); break;
#endif
	case DSERR_ACCESSDENIED:
		MessageBox( NULL, L"アクセスが拒否されました", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_ALLOCATED:
		MessageBox( NULL, L"優先レベルなどのリソースが他の呼び出しによって使用中なため失敗しました", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_ALREADYINITIALIZED:
		MessageBox( NULL, L"オブジェクトはすでに初期化済みです", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_BADFORMAT:
		MessageBox( NULL, L"指定したWAVEフォーマットはサポートされていません", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_BADSENDBUFFERGUID:
		MessageBox( NULL, L"オーディオパスファイルで指定されているGUIDが有効なミックスインバッファと一致しません", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_BUFFERLOST:
		MessageBox( NULL, L"バッファメモリが失われました、復元してください", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_BUFFERTOOSMALL:
		MessageBox( NULL, L"バッファサイズが不十分で、エフェクト処理が実行できません", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_CONTROLUNAVAIL:
		MessageBox( NULL, L"要求されたバッファコントロールが利用不可です", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_DS8_REQUIRED:
		MessageBox( NULL, L"要求された機能にはCLSID_SirectSound8クラス以降のオブジェクトが必要です", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_FXUNAVAILABLE :
		MessageBox( NULL, L"要求されたエフェクトはシステムで見つからなかったもしくは順序が間違っています", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_GENERIC:
		MessageBox( NULL, L"DirectSoundサブシステム内部で原因不明のエラーが発生しました", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_INVALIDCALL:
		MessageBox( NULL, L"オブジェクトの現在の状態に対して有効ではありません", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_INVALIDPARAM:
		MessageBox( NULL, L"戻り関数に無効なパラメータが渡されました", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_NOAGGREGATION:
		MessageBox( NULL, L"オブジェクトは集計をサポートしていません", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_NODRIVER:
		MessageBox( NULL, L"使用可能なサウンドドライバーがありません", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_NOINTERFACE:
		MessageBox( NULL, L"要求されたCOMインターフェイスは使用できません", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_OBJECTNOTFOUND:
		MessageBox( NULL, L"要求されたオブジェクトが見つかりません", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_OTHERAPPHASPRIO:
		MessageBox( NULL, L"他の高優先度アプリケーションが実行しているため、この呼び出しを実行できません", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_OUTOFMEMORY:
		MessageBox( NULL, L"DirectSoundサブシステムは要求を完了するだけの十分なメモリーを割りあてられませんでした", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_PRIOLEVELNEEDED:
		MessageBox( NULL, L"DSSCL_PRIORITYと同等以上の強調レベルが必要です", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_SENDLOOP :
		MessageBox( NULL, L"IDirectSound8::Initializeメソッド呼び出し前です", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_UNSUPPORTED:
		MessageBox( NULL, L"呼び出された関数はこの時点ではサポートされていません", L"MySound", MB_OK|MB_ICONSTOP); break;
	default:
		MessageBox( NULL, L"不明のエラー", L"MySound", MB_OK|MB_ICONSTOP); break;
	}
}

MySound	g_MySound;

MySound::MySound( void )
{
}

MySound::~MySound( void )
{
	m_pDSound->Release();
}

bool	MySound::Init( HWND& hWnd )
{
	HRESULT	r;

	if	( (r = DirectSoundCreate( NULL, &m_pDSound, NULL )) != DS_OK){
		ErrorMessage( r );
		return	false;
	}
	m_pDSound->SetCooperativeLevel( hWnd, DSSCL_PRIORITY );
	return	true;
}


void	MySound::Update( void )
{
	Bgm.Update();
	Voice.Update();
}


MySoundInd::MySoundInd()
	: m_PlayFlg(false)
	, m_LoopFlg(false)
	, m_NowBuf(1)
	, m_Volume(127)
	, m_Mode(IDLE)
{
	m_Filename[0] = '\0';
	m_Readname[0] = '\0';
}

MySoundInd::~MySoundInd()
{
	if	( m_PlayFlg){
		ov_clear( &m_Ogg );
	}
}

void	MySoundInd::Read( void )
{
	char	name[512];

	if	( strchr( m_Readname, ':' )){
		// フルパス受け取り
		sprintf_s( name, "%s.ogg", m_Readname );
	} else
	{
		sprintf_s( name, "%s%s.ogg", g_SetupData.imagePath, m_Readname );
	}

	int		err = ov_fopen( name, &m_Ogg );
	
	if	( err != 0){
		switch( err ){
		case OV_EREAD:
		case OV_ENOTVORBIS:
		case OV_EVERSION:
		case OV_EBADHEADER:
		case OV_EFAULT:
			break;
		}
		return;
	}

	vorbis_info*	info = ov_info( &m_Ogg, -1 );
	int				playTime = 1;		// ○秒分

	// WAVE情報
	m_WaveFmt.wFormatTag		= WAVE_FORMAT_PCM;
	m_WaveFmt.nChannels			= info->channels;
	m_WaveFmt.nSamplesPerSec	= 44100;				// サンプリングレート
	m_WaveFmt.wBitsPerSample	= 16;					// ビットレート
	m_WaveFmt.nBlockAlign		= info->channels*16/8;
	m_WaveFmt.nAvgBytesPerSec	= m_WaveFmt.nSamplesPerSec*m_WaveFmt.nBlockAlign;
	m_WaveFmt.cbSize			= 0;

	// DirectSoundBuffer情報
	m_Desc.dwSize			= sizeof(DSBUFFERDESC);
	/*
		DSBCAPS_STICKYFOCUS : 他アプリケーションがアクティブになっても音は鳴らし続ける
		DSBCAPS_CTRLVOLUME  : ボリュームコントロール能力を持つ
		DSBCAPS_CTRLPAN     : パンコントロール能力を持つ
	*/
	m_Desc.dwFlags			= DSBCAPS_STICKYFOCUS|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN;
	m_Desc.dwBufferBytes	= m_WaveFmt.nAvgBytesPerSec*playTime;
	m_Desc.dwReserved		= 0;
	m_Desc.lpwfxFormat		= &m_WaveFmt;
	m_Desc.guid3DAlgorithm	= GUID_NULL;

	// セカンダリバッファ作成
	IDirectSoundBuffer*		ptmpBuf = NULL;
	HRESULT	res;

	if	( (res = g_MySound.DirectSnd()->CreateSoundBuffer( &m_Desc, &ptmpBuf, NULL )) != DS_OK){
		ErrorMessage( res );
		ov_clear( &m_Ogg );
		return;
	}
	ptmpBuf->QueryInterface( IID_IDirectSoundBuffer, (void**)&m_DSBuf );
	ptmpBuf->Release();

	void*	pA1 = NULL, *pA2 = NULL;
	DWORD	dA1 = 0, dA2 = 0;

	if	( (res = m_DSBuf->Lock( 0, 0, &pA1, &dA1, &pA2, &dA2, DSBLOCK_ENTIREBUFFER )) != DS_OK){
		ErrorMessage( res );
		ov_clear( &m_Ogg );
		return;
	}
	Decode( (char*)pA1, dA1 );
	m_DSBuf->Unlock( pA1, dA1, pA2, dA2 );
	m_DSBuf->Play( 0, 0, DSBPLAY_LOOPING );
	SetVol( m_Volume );
	m_PlayFlg = true;
	strcpy_s( m_Filename, m_Readname );
	m_Readname[0] = '\0';
	m_NowBuf = 0;
	m_Mode = PLAY;
}

void	MySoundInd::Decode( char* buffer, int bufsize )
{
	int		reqsize = 4096;
	int		readsize;
	int		bitstream;
	int		comsize = 0;
	bool	adjust = false;

	memset( buffer, 0x0, bufsize );
	if	( bufsize < reqsize){
		reqsize = bufsize;
		adjust = true;
	}
	for	( int i = 0; i < 10000; i++){
		readsize = ov_read( &m_Ogg,					// オープンファイル
							(char*)(buffer + comsize),	// バッファ
							reqsize,			// バッファサイズ
							0, 					// エンディアン		0:リトル(Windows) 1:ビッグ
							2,					// WORDサイズ		1:8bit 2:16bit(Windows)
							1,					// ＰＣＭ音声符号	0:無 1:有(通常)
							&bitstream			// ストリーム再生位置(ポインタ)
							);
		if	( readsize == 0){
			/* ループ */
			if	( m_LoopFlg){
				ov_time_seek( &m_Ogg, 0.0 );
			} else
			{
				return;
			}
		}
		comsize += readsize;
		if	( comsize >= bufsize){
			return;
		}
		if	( bufsize - comsize < 4096){
			adjust = true;
			reqsize = bufsize - comsize;
		}
	}
}

void	MySoundInd::FadeoutProc( void )
{
	long	now = timeGetTime();
	long	diff = now - m_FadeParam.startTime;

	if	( diff > m_FadeParam.fadeTime){
		Stop();
		return;
	}

	int		vol = m_FadeParam.volume - m_FadeParam.volume*(int)diff/m_FadeParam.fadeTime;

	if	( vol < 0){ Stop(); return; }
	if	( vol > 127){ vol = 127; }

	long	db = vol*DSBVOLUME_MAX/127;
	HRESULT	res;

	if	( (res = m_DSBuf->SetVolume( db )) != DS_OK){
		ErrorMessage( res );
	}
}

void	MySoundInd::Update( void )
{
	switch( m_Mode ){
	case IDLE:
		if	( strcmp( m_Readname, "" ) == 0){ break; }
		m_Mode = READ; break;
		break;

	case READ:
		if	( IsPlay()){
			Stop();
			m_Mode = READ; break;
		}
		Read(); break;

	case PLAY:
		if	( !IsPlay()){
			Stop(); break;
		}
		if	( strcmp( m_Readname, "" ) == 0 || strcmp( m_Filename, m_Readname ) == 0){
			m_Readname[0] = '\0';
			u_int	size = m_Desc.dwBufferBytes/2;
			void*	pA1 = NULL, *pA2 = NULL;
			DWORD	point, dA1 = 0, dA2 = 0;
			HRESULT	res;
		
			if	( (res = m_DSBuf->GetCurrentPosition( &point, 0 )) != DS_OK){
				ErrorMessage( res );
				return;
			}
			if	( m_NowBuf == 0 && point >= size){
				if	( (res = m_DSBuf->Lock( 0, size, &pA1, &dA1, &pA2, &dA2, 0 )) != DS_OK){
					ErrorMessage( res );
					return;
				}
				Decode( (char*)pA1, dA1 );
				m_DSBuf->Unlock( pA1, dA1, pA2, dA2 );
				m_NowBuf ^= 1;
			} else
			if	( m_NowBuf == 1 && point < size){
				if	( (res = m_DSBuf->Lock( size, size*2, &pA1, &dA1, &pA2, &dA2, 0 )) != DS_OK){
					ErrorMessage( res );
					return;
				}
				Decode( (char*)pA1, dA1 );
				m_DSBuf->Unlock( pA1, dA1, pA2, dA2 );
				m_NowBuf ^= 1;
			}
			break;
		}
		m_Mode = READ; break;

	case PAUSE:
		break;

	case STOP:
		if	( !IsPlay()){
			ov_clear( &m_Ogg );
			m_Mode = IDLE;
		} break;

	case FOUT:
		FadeoutProc();
		break;
	}
#if	0
	if	( IsPlay()){
		if	( strcmp( m_Readname, "" ) == 0 || strcmp( m_Filename, m_Readname ) == 0){
			m_Readname[0] = '\0';
			u_int	size = m_Desc.dwBufferBytes/2;
			void*	pA1 = NULL, *pA2 = NULL;
			DWORD	point, dA1 = 0, dA2 = 0;
			HRESULT	res;
		
			if	( (res = m_DSBuf->GetCurrentPosition( &point, 0 )) != DS_OK){
				ErrorMessage( res );
				return;
			}
			if	( m_NowBuf == 0 && point >= size){
				if	( (res = m_DSBuf->Lock( 0, size, &pA1, &dA1, &pA2, &dA2, 0 )) != DS_OK){
					ErrorMessage( res );
					return;
				}
				Decode( (char*)pA1, dA1 );
				m_DSBuf->Unlock( pA1, dA1, pA2, dA2 );
				m_NowBuf ^= 1;
			} else
			if	( m_NowBuf == 1 && point < size){
				if	( (res = m_DSBuf->Lock( size, size*2, &pA1, &dA1, &pA2, &dA2, 0 )) != DS_OK){
					ErrorMessage( res );
					return;
				}
				Decode( (char*)pA1, dA1 );
				m_DSBuf->Unlock( pA1, dA1, pA2, dA2 );
				m_NowBuf ^= 1;
			}
			return;
		}
	}

	if	( m_PlayFlg){
		m_DSBuf->Stop();
		ov_clear( &m_Ogg );
		m_PlayFlg = false;
		return;
	}

	if	( strcmp( m_Readname, "" ) == 0){ return; }
	Read();
#endif
}

void	MySoundInd::Req( const char* name, bool loop )
{
	strcpy_s( m_Readname, name );
	m_LoopFlg = loop;
}

bool	MySoundInd::IsPlay( void )
{
	if	( !m_DSBuf){ return	false; }

	HRESULT	res;
	DWORD	sts;

	if	( (res = m_DSBuf->GetStatus( &sts )) != DS_OK){
		ErrorMessage( res );
		return	false;
	}
	return	(sts&DSBSTATUS_PLAYING);
}

void	MySoundInd::Pause( void )
{
	if	( !m_DSBuf){ return; }

	HRESULT	res;

	switch( m_Mode ){
	case PLAY:
		if	( (res = m_DSBuf->Stop()) != DS_OK){
			ErrorMessage( res ); break;
		}
		m_Mode = PAUSE; break;
	}
}

void	MySoundInd::Stop( void )
{
	if	( !m_DSBuf){ return; }

	HRESULT	res;

	switch( m_Mode ){
	case READ:
	case PLAY:
	case FOUT:
		if	( (res = m_DSBuf->Stop()) != DS_OK){
			ErrorMessage( res ); break;
		}
		m_Mode = STOP; break;
	}
}

void	MySoundInd::Play( void )
{
	if	( !m_DSBuf){ return; }

	switch( m_Mode ){
	case PAUSE:
		m_DSBuf->Play( 0, 0, DSBPLAY_LOOPING );
		m_Mode = PLAY; break;
	case IDLE:
	case STOP:
		strcpy_s( m_Readname, m_Filename );
		m_Mode = READ; break;
#if	0
		{
			void*	pA1 = NULL, *pA2 = NULL;
			DWORD	dA1 = 0, dA2 = 0;

			ov_time_seek( &m_Ogg, 0.0 );
			if	( (res = m_DSBuf->Lock( 0, 0, &pA1, &dA1, &pA2, &dA2, DSBLOCK_ENTIREBUFFER )) != DS_OK){
				ErrorMessage( res );
				ov_clear( &m_Ogg );
				return;
			}
			Decode( (char*)pA1, dA1 );
			m_DSBuf->Unlock( pA1, dA1, pA2, dA2 );
			m_DSBuf->Play( 0, 0, DSBPLAY_LOOPING );
			m_NowBuf = 0;
			m_Mode = PLAY;
		} break;
#endif
	}
}

void	MySoundInd::SetVol( int vol )
{
	if	( !m_DSBuf){ return; }

	if	( vol <    0){ vol =   0; }
	if	( vol >= 128){ vol = 127; }

	if	( m_Mode != FOUT){
		long	db = (127 - vol)*DSBVOLUME_MIN/127;
		HRESULT	res;
	
		if	( (res = m_DSBuf->SetVolume( db )) != DS_OK){
			ErrorMessage( res );
			return;
		}
	}
	m_Volume = vol;
}

int		MySoundInd::GetVol( void )
{
	if	( !m_DSBuf){ return	0; }

	HRESULT	res;
	long	db;

	if	( (res = m_DSBuf->GetVolume( &db )) != DS_OK){
		ErrorMessage( res );
		return	0;
	}
	/*
		DSBVOLUME_MAX : 0
		DSBVOLUME_MIN : -10000
		減衰なので０↓
	*/
	db = db - DSBVOLUME_MIN;
	return	db*127/10000;
}

void	MySoundInd::Fadeout( int fadeTime )
{
	if	( m_DSBuf){ return; }
	if	( m_Mode != PLAY){ return; }

	m_FadeParam.fadeTime = fadeTime;
	m_FadeParam.startTime = timeGetTime();
	m_FadeParam.volume = m_Volume;

	m_Mode = FOUT;
}

void	MySoundInd::SetPan( signed short pan )
{
	if	( !m_DSBuf){ return; }

	if	( pan < -0x4000){ pan = -0x4000; }
	if	( pan >  0x4000){ pan =  0x4000; }

	signed long	lp = pan*DSBPAN_RIGHT/0x4000;
	HRESULT	res;

	if	( (res = m_DSBuf->SetPan( lp )) != DS_OK){
		ErrorMessage( res );
	}
}



bool	SoundInit( HWND& hWnd )
{
	return	g_MySound.Init( hWnd );
}

void	SoundUpdate( void )
{
	g_MySound.Update();
}

void	SoundStop( void )
{
}

void	SoundPause( void )
{
}

}
