#pragma	once

#include	<dshow.h>

namespace movie {

class MyMovie {
private:
	IGraphBuilder*	m_pGraph;
	IMediaControl*	m_pMedia;
	IMediaEventEx*	m_pEvent;
	IVideoWindow*	m_pVideo;
	IBasicVideo*	m_pBasic;
	HWND*			m_hWnd;

public:
	bool	Init( HWND& hWnd );
	void	Read( const TCHAR* filename );
	void	Play( void );
	bool	IsPlay( void );
	bool	IsPause( void );

public:
	MyMovie( void );
	~MyMovie( void );
};

bool	MovieInit( HWND& hWnd );
void	MovieUpdate( void );

void	MoviePlay( const TCHAR* filename );
bool	MovieIsPlay( void );

}
