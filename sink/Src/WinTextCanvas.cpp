/*! @file WinTextCanvas.cpp */
#include <d3dx9tex.h>

#pragma warning( disable : 4996 ) // disable deprecated warning 
#include <strsafe.h>
#pragma warning( default : 4996 ) 

#include "WinTextCanvas.h"
#include "Setup.h"


void WinTextCanvas::draw(int x, int y, const char *str, int n)
{
	RECT	rc;
	wchar_t	str_t[512];
	int		textMax = strlen(str);

	SetRect(&rc, x, y, 0, 0);
	memset(str_t, 0x0, sizeof(str_t));
	if (textMax < n) n = textMax;
	MultiByteToWideChar( CP_OEMCP, MB_PRECOMPOSED, str, n, str_t, sizeof(str_t)/2 );
	pTextFont->DrawText( NULL, str_t, n, &rc, DT_NOCLIP, D3DXCOLOR( 1.0f, 1.0f, 1.0f, 1.0f ));

}

void WinTextCanvas::se()
{
}
