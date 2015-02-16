/*! @file TextWrite.cpp */
#include "TextWrite.h"

void TextWrite(ID3DXFont* pFont, int x, int y, const wchar_t *wstr)
{
	RECT rc;
	SetRect( &rc, x-1, y, 0, 0 );
	pFont->DrawText( NULL, wstr, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ));
	SetRect( &rc, x, y-1, 0, 0 );
	pFont->DrawText( NULL, wstr, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ));
	SetRect( &rc, x+1, y, 0, 0 );
	pFont->DrawText( NULL, wstr, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ));
	SetRect( &rc, x, y+1, 0, 0 );
	pFont->DrawText( NULL, wstr, -1, &rc, DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 0.0f, 1.0f ));
	SetRect( &rc, x, y, 0, 0 );
	pFont->DrawText( NULL, wstr, -1, &rc, DT_NOCLIP, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ));
}
