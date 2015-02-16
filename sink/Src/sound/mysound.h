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
	OggVorbis_File	m_Ogg;					// �J����Ă�t�@�C���i�[
	char			m_Filename[512];		// �J����Ă�t�@�C����
	char			m_Readname[512];		// �J�����Ƃ��Ă���t�@�C����
	bool			m_PlayFlg;				// �Đ��t���O
	bool			m_LoopFlg;				// ���[�v�t���O
	int				m_NowBuf;				// �_�u���o�b�t�@�n
	int				m_Volume;				// ������

	WAVEFORMATEX			m_WaveFmt;		// DirectSound
	DSBUFFERDESC			m_Desc;			// DirectSound
	IDirectSoundBuffer8*	m_DSBuf;		// DirectSound

	struct {
		int		fadeTime;		// �t�F�[�h�^�C��
		long	startTime;		// �J�n�^�C��
		int		volume;			// �{�����[��
	} m_FadeParam;

	enum {
		IDLE,	// �҂�
		READ,	// �ǂݍ���
		PLAY,	// �Đ���
		PAUSE,	// �ꎞ��~��
		STOP,	// ��~
		FOUT,	// �t�F�[�h�A�E�g
	} m_Mode;

private:
	void	Read( void );
	void	Decode( char* buffer, int bufsize );
	void	FadeoutProc( void );

public:
	void	Update( void );

	/*
		�T�E���h���N�G�X�g
		name : �t�@�C����
		loop : ���[�v�t���O[true/�L false/��]
	*/
	void	Req( const char* name, bool loop=false );

	/*
		�Đ��`�F�b�N
		bool : �Đ��t���O[ture/�Đ��� false/(�ꎞ)��~��]
	*/
	bool	IsPlay( void );

	/*
		�ꎞ��~
		�Đ��ĊJ���͒�~�����ꏊ����
	*/
	void	Pause( void );

	/*
		��~
		�Đ��ĊJ���͏��n�߂���
	*/
	void	Stop( void );

	/*
		�Đ��ĊJ
		�ǂݍ��݉������Ȃ��ꍇ�͂Ȃɂ����Ȃ�
	*/
	void	Play( void );

	/*
		���ʐݒ�
		vol : ����[0~127 0/���� 127/�ő�]
	*/
	void	SetVol( int vol );

	/*
		���ʎ擾
		int : ����[0~127 0/���� 127/�ő�]
	*/
	int		GetVol( void );

	/*
		�t�F�[�h�A�E�g
		fadeTime : �t�F�[�h�^�C��[�P�ʂ�ms 1000�łP�b]
	*/
	void	Fadeout( int fadeTime );

	/*
		�p���ݒ�
		pan : �΂�[-0x4000/�� 0/���� 0x4000/�E]
	*/
	void	SetPan( signed short pan );

public:
	MySoundInd();
	~ MySoundInd();
};

bool	SoundInit( HWND& hWnd );			// �T�E���h�֘A������
void	SoundUpdate( void );				// �T�E���h�֘A���t���[���������ŌĂяo��

void	SoundStop( void );					// �S�T�E���h��~
void	SoundPause( void );					// �S�T�E���h�ꎞ��~
void	SoundPlay( void );					// �S�T�E���h�Đ�

extern MySoundInd	Bgm;			// �w�i���n
extern MySoundInd	Voice;			// ���n

}
