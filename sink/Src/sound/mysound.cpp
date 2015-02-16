
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
		MessageBox( NULL, L"�o�b�t�@�쐬�͐������������̂R�c�A���S���Y������p����܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
#if	0
	case DS_INCOMPLETE:
		MessageBox( NULL, L"���\�b�h�͐����������A�����Ă��Ȃ��I�v�V�����G�t�F�N�g������܂�", L"MySound", MB_OK|MB_ICONSTOP); break;
#endif
	case DSERR_ACCESSDENIED:
		MessageBox( NULL, L"�A�N�Z�X�����ۂ���܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_ALLOCATED:
		MessageBox( NULL, L"�D�惌�x���Ȃǂ̃��\�[�X�����̌Ăяo���ɂ���Ďg�p���Ȃ��ߎ��s���܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_ALREADYINITIALIZED:
		MessageBox( NULL, L"�I�u�W�F�N�g�͂��łɏ������ς݂ł�", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_BADFORMAT:
		MessageBox( NULL, L"�w�肵��WAVE�t�H�[�}�b�g�̓T�|�[�g����Ă��܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_BADSENDBUFFERGUID:
		MessageBox( NULL, L"�I�[�f�B�I�p�X�t�@�C���Ŏw�肳��Ă���GUID���L���ȃ~�b�N�X�C���o�b�t�@�ƈ�v���܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_BUFFERLOST:
		MessageBox( NULL, L"�o�b�t�@�������������܂����A�������Ă�������", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_BUFFERTOOSMALL:
		MessageBox( NULL, L"�o�b�t�@�T�C�Y���s�\���ŁA�G�t�F�N�g���������s�ł��܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_CONTROLUNAVAIL:
		MessageBox( NULL, L"�v�����ꂽ�o�b�t�@�R���g���[�������p�s�ł�", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_DS8_REQUIRED:
		MessageBox( NULL, L"�v�����ꂽ�@�\�ɂ�CLSID_SirectSound8�N���X�ȍ~�̃I�u�W�F�N�g���K�v�ł�", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_FXUNAVAILABLE :
		MessageBox( NULL, L"�v�����ꂽ�G�t�F�N�g�̓V�X�e���Ō�����Ȃ������������͏������Ԉ���Ă��܂�", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_GENERIC:
		MessageBox( NULL, L"DirectSound�T�u�V�X�e�������Ō����s���̃G���[���������܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_INVALIDCALL:
		MessageBox( NULL, L"�I�u�W�F�N�g�̌��݂̏�Ԃɑ΂��ėL���ł͂���܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_INVALIDPARAM:
		MessageBox( NULL, L"�߂�֐��ɖ����ȃp�����[�^���n����܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_NOAGGREGATION:
		MessageBox( NULL, L"�I�u�W�F�N�g�͏W�v���T�|�[�g���Ă��܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_NODRIVER:
		MessageBox( NULL, L"�g�p�\�ȃT�E���h�h���C�o�[������܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_NOINTERFACE:
		MessageBox( NULL, L"�v�����ꂽCOM�C���^�[�t�F�C�X�͎g�p�ł��܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_OBJECTNOTFOUND:
		MessageBox( NULL, L"�v�����ꂽ�I�u�W�F�N�g��������܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_OTHERAPPHASPRIO:
		MessageBox( NULL, L"���̍��D��x�A�v���P�[�V���������s���Ă��邽�߁A���̌Ăяo�������s�ł��܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_OUTOFMEMORY:
		MessageBox( NULL, L"DirectSound�T�u�V�X�e���͗v�����������邾���̏\���ȃ������[�����肠�Ă��܂���ł���", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_PRIOLEVELNEEDED:
		MessageBox( NULL, L"DSSCL_PRIORITY�Ɠ����ȏ�̋������x�����K�v�ł�", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_SENDLOOP :
		MessageBox( NULL, L"IDirectSound8::Initialize���\�b�h�Ăяo���O�ł�", L"MySound", MB_OK|MB_ICONSTOP); break;
	case DSERR_UNSUPPORTED:
		MessageBox( NULL, L"�Ăяo���ꂽ�֐��͂��̎��_�ł̓T�|�[�g����Ă��܂���", L"MySound", MB_OK|MB_ICONSTOP); break;
	default:
		MessageBox( NULL, L"�s���̃G���[", L"MySound", MB_OK|MB_ICONSTOP); break;
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
		// �t���p�X�󂯎��
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
	int				playTime = 1;		// ���b��

	// WAVE���
	m_WaveFmt.wFormatTag		= WAVE_FORMAT_PCM;
	m_WaveFmt.nChannels			= info->channels;
	m_WaveFmt.nSamplesPerSec	= 44100;				// �T���v�����O���[�g
	m_WaveFmt.wBitsPerSample	= 16;					// �r�b�g���[�g
	m_WaveFmt.nBlockAlign		= info->channels*16/8;
	m_WaveFmt.nAvgBytesPerSec	= m_WaveFmt.nSamplesPerSec*m_WaveFmt.nBlockAlign;
	m_WaveFmt.cbSize			= 0;

	// DirectSoundBuffer���
	m_Desc.dwSize			= sizeof(DSBUFFERDESC);
	/*
		DSBCAPS_STICKYFOCUS : ���A�v���P�[�V�������A�N�e�B�u�ɂȂ��Ă����͖炵������
		DSBCAPS_CTRLVOLUME  : �{�����[���R���g���[���\�͂�����
		DSBCAPS_CTRLPAN     : �p���R���g���[���\�͂�����
	*/
	m_Desc.dwFlags			= DSBCAPS_STICKYFOCUS|DSBCAPS_CTRLVOLUME|DSBCAPS_CTRLPAN;
	m_Desc.dwBufferBytes	= m_WaveFmt.nAvgBytesPerSec*playTime;
	m_Desc.dwReserved		= 0;
	m_Desc.lpwfxFormat		= &m_WaveFmt;
	m_Desc.guid3DAlgorithm	= GUID_NULL;

	// �Z�J���_���o�b�t�@�쐬
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
		readsize = ov_read( &m_Ogg,					// �I�[�v���t�@�C��
							(char*)(buffer + comsize),	// �o�b�t�@
							reqsize,			// �o�b�t�@�T�C�Y
							0, 					// �G���f�B�A��		0:���g��(Windows) 1:�r�b�O
							2,					// WORD�T�C�Y		1:8bit 2:16bit(Windows)
							1,					// �o�b�l��������	0:�� 1:�L(�ʏ�)
							&bitstream			// �X�g���[���Đ��ʒu(�|�C���^)
							);
		if	( readsize == 0){
			/* ���[�v */
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
		�����Ȃ̂łO��
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
