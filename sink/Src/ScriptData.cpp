/*! @file ScriptData.cpp */
#include	<d3d9.h>
#include	<d3dx9tex.h>
#include	<commctrl.h>
#pragma warning( disable : 4996 ) // disable deprecated warning 
#include	<strsafe.h>
#pragma warning( default : 4996 ) 

#include	"ScriptData.h"
#include	"tinyxml.h"

ScriptData::ScriptData( void )
	: m_Buffer(NULL)
	, m_LoadFlg(false)
{
	m_File[0] = '\0';
	m_LoadName[0] = '\0';
}

ScriptData::~ScriptData( void )
{
	if	( m_Buffer){ delete[] m_Buffer; }
	m_Buffer = NULL;
}

void	ScriptData::LoadFile( const char* filename )
{
	strcpy_s( m_LoadName, filename );
	m_LoadFlg = false;
	m_File[0] = '\0';
}

void	ScriptData::LoadFile( const TCHAR* filename )
{
	char	buff[512];

	memset( buff, 0x0, sizeof(buff) );
	WideCharToMultiByte( CP_OEMCP, 0, filename, wcslen( filename ), buff, sizeof(buff), NULL, NULL );
	strcpy_s( m_LoadName, buff );
	m_LoadFlg = false;
	m_File[0] = '\0';
}

void	ScriptData::Proc( void )
{
	if	( strcmp( m_LoadName, "" ) == 0){ return; }
	if	( strcmp( m_LoadName, m_File ) == 0){ return; }

	FILE*	fp;

//	if	( fopen_s( &fp, m_LoadName, "rt" ) != 0){
	if	( fopen_s( &fp, m_LoadName, "rb" ) != 0){
		m_LoadName[0] = '\0';
		MessageBox( NULL, L"ファイルが存在しません。", L"LoadScript", MB_OK|MB_ICONSTOP);
		return;
	}

	int		length;

	fseek( fp, 0, SEEK_END );
	length = ftell( fp );
	fseek( fp, 0, SEEK_SET );

	if	( length > 0 ){
#if	1
		char*	buff = new char[length + 1];

		memset( buff, 0x0, sizeof(char)*(length + 1) );
		if	( fread( buff, length, 1, fp ) >= 1){
			/* 改行コード変換 CR CR+LF → LF */
			TIXML_STRING	str;
			char*	pl = buff, *pb = buff;
	
			str.reserve( length );
			for	( int i = 0; i < INT_MAX; i++, pb++){
				if	( !(*pb)){ break; }
#if	0
				if	( *pb == 0x0a){		// 文字コード0x0a = LF
					str.append( pl, pb - pl + 1 );
					pl = pb;
				} else
				if	( *pb == 0x0d){		// 文字コード0x0d = CR
					if	( pb - pl > 0){ str.append( pl, pb - pl ); }
//					str += (char)0x0a;		// LF
					if	( *(pb + 1) == 0x0a){ pb++; }		// CR+LF
					pl = pb;
				}
#else
				if	( *pb == 0x0a){		// 文字コード0x0a = LF
					if	( pb - pl > 0){ str.append( pl, pb - pl ); }
					str += 0x0d;		// CR
					str += 0x0a;		// LF
					pl = pb;
				} else
				if	( *pb == 0x0d){		// 文字コード0x0d = CR
					if	( *(pb + 1) != 0x0a){
						str.append( pl, pb - pl + 1 );
						str += 0x0a;
					} else
					{
						// CR+LF
						pb++;
						str.append( pl, pb - pl );
					}
					pl = pb;
				}
#endif
			}
			str.append( pl, pb - pl );
			if	( m_Buffer){ delete[]	m_Buffer; }
			m_Buffer = new char[str.size() + 1];
			memset( m_Buffer, 0x0, sizeof(char)*(str.size() + 1) );
	
			strcpy_s( m_Buffer, str.size() + 1, str.c_str() );
		}
		delete[]	buff;
#else
		if	( m_Buffer){ delete[]	m_Buffer; }
		m_Buffer = new char[length + 1];
		memset( m_Buffer, 0x0, sizeof(char)*(length + 1) );

		fread( m_Buffer, length, 1, fp );
#endif
		strcpy_s( m_File, m_LoadName );
		m_LoadFlg = true;
	}
	fclose( fp );
}

const char*	ScriptData::MultiBuffer( void )
{
	return	m_Buffer;
}
