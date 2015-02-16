/*! @file ScriptData.h */

#pragma once

class ScriptData {
private:
	char*	m_Buffer;
	char	m_File[512];
	char	m_LoadName[512];
	bool	m_LoadFlg;

public:
	void	LoadFile( const char* filename );
	void	LoadFile( const TCHAR* filename );
	void	Proc( void );

	const char*		MultiBuffer( void );
	const TCHAR*	WideBuffer( void ){ return	 NULL; }
	bool	CheckLoad( void ){ return	m_LoadFlg; }
	void	BreakLoadFlg( void ){ m_LoadFlg = false; }

public:
	ScriptData( void );
	~ScriptData( void );

};
