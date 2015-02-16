
#include	<d3d9.h>

#include	"../Setup.h"
#include	"mymovie.h"

extern WindowsSetupData g_SetupData;

namespace movie {

void	ErrorMessage( HRESULT result )
{
#if	0
	switch( result ){
	default:
		MessageBox( NULL, L"不明のエラー", L"MyMovie", MB_OK|MB_ICONSTOP); break;
	}
#else
	MessageBox( NULL, L"不明のエラー", L"MyMovie", MB_OK|MB_ICONSTOP);
#endif
}


MyMovie	g_MyMovie;

MyMovie::MyMovie( void )
{
//	m_Filename[0] = '\0';
}

MyMovie::~MyMovie( void )
{
	if	( IsPlay()){ m_pMedia->Release(); }
	m_pGraph->Release();
	m_pVideo->Release();
	m_pEvent->Release();
	m_pBasic->Release();

	CoUninitialize();
}

bool	MyMovie::Init( HWND& hWnd )
{
	/* COM初期化 */
	CoInitialize( NULL );

	/* DilterGraphics生成 */
	CoCreateInstance( CLSID_FilterGraph,
					  NULL,
					  CLSCTX_INPROC,
					  IID_IGraphBuilder,
					  (LPVOID*)&m_pGraph
					);

	/* MediaControlインターフェイス取得 */
	m_pGraph->QueryInterface( IID_IMediaControl, (LPVOID*)&m_pMedia );
	m_pGraph->QueryInterface( IID_IMediaEvent, (LPVOID*)&m_pEvent );

	m_pGraph->QueryInterface( IID_IVideoWindow, (LPVOID*)&m_pVideo );
	m_pGraph->QueryInterface( IID_IBasicVideo, (LPVOID*)&m_pBasic );

	m_hWnd = &hWnd;

	return	true;
}

void	MyMovie::Read( const TCHAR* filename )
{
#if 0
	strcpy_s( m_Readname, filename );

	if	( strcmp( m_Filename, m_Readname ) == 0){
		return; 
	}

	if	( IsPlay()){
		m_pMedia->Stop();
		m_pMedia->Release();
		return;
	}

	char	name[512];

	if	( strchr( filename, ':' )){
		// フルパス受け取り
		sprintf_s( name, "%s.mpg", filename );
	} else
	{
		sprintf_s( name, "%s%s.mpg", g_SetupData.imagePath, filename );
	}

	TCHAR	name_t[512];

	memset( name_t, 0x0, sizeof(name_t) );
	MultiByteToWideChar( CP_OEMCP, MB_PRECOMPOSED, name, strlen(name), name_t, sizeof(name_t)/2 );

	strcpy_s( m_Filename, m_Readname );
	m_pMedia->RenderFile( name_t );
#else
	m_pGraph->RenderFile( filename, 0 );
#endif
	long	width, height;
	int		x, y;

	m_pBasic->get_VideoWidth( &width );
	m_pBasic->get_VideoHeight( &height );

	m_pVideo->put_Owner( (OAHWND)*m_hWnd );
	m_pVideo->put_WindowStyle( WS_CHILD|WS_CLIPSIBLINGS );

	if	( width > g_SetupData.width){
		x = 0;
		width = g_SetupData.width;
	} else
	{
		x = (g_SetupData.width - width)/2;
	}

	if	( height > g_SetupData.height){
		y = 0;
		height = g_SetupData.height;
	} else
	{
		y = (g_SetupData.height - height)/2;
	}

	m_pVideo->SetWindowPosition( x, y, width, height );

	m_pVideo->SetWindowForeground( OATRUE );
	m_pVideo->put_Visible( OATRUE );

	m_pMedia->Run();
}

void	MyMovie::Play( void )
{
	if	( !IsPlay()){
//		Read( m_Readname );
		return;
	}

	HRESULT	result;
	long	ev, p1, p2;

	if	( (result = m_pEvent->GetEvent( &ev, &p1, &p2, 10 )) != S_OK){
		return;
	}

	switch( ev ){
	case EC_COMPLETE:
		/* 再生終了 */
		m_pMedia->Stop();
		m_pMedia->Release();
//		m_Filename[0] = '\0';
//		m_Readname[0] = '\0';
		break;
	default:
		break;
	}
}

bool	MyMovie::IsPlay( void )
{
	HRESULT			result;
	OAFilterState	state;

	if	( (result = m_pMedia->GetState( 60, &state )) != S_OK){
		switch( result ){
		case VFW_S_STATE_INTERMEDIATE:
			/* 移行中なので成功扱い */
			return	(state==State_Running); break;
		case VFW_S_CANT_CUE:
		case E_FAIL:
			return	false; break;
		}
	}
	return	(state==State_Running);
}

bool	MyMovie::IsPause( void )
{
	HRESULT			result;
	OAFilterState	state;

	if	( (result = m_pMedia->GetState( 60, &state )) != S_OK){
		switch( result ){
		case VFW_S_STATE_INTERMEDIATE:
			/* 移行中なので成功扱い */
			return	(state==State_Paused); break;
		case VFW_S_CANT_CUE:
		case E_FAIL:
			return	false; break;
		}
	}
	return	(state==State_Paused);
}

bool	MovieInit( HWND& hWnd )
{
	return	g_MyMovie.Init( hWnd );
}

void	MovieUpdate( void )
{
	g_MyMovie.Play();
}

void	MoviePlay( const TCHAR* filename )
{
	g_MyMovie.Read( filename );
}

bool	MovieIsPlay( void )
{
	return	g_MyMovie.IsPlay();
}

}
