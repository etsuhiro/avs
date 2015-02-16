/*! @file textCanvas.cpp */

#include "TextCanvas.h"
#include "SetupData.h"

using namespace avs;

TextCanvas::TextCanvas(avs::SetupData& setup)
	: m_Setup(setup)
{
	m_pLinebuf = new unsigned short*[m_Setup.text.row];
	m_nChrLine = new int[m_Setup.text.row];
	m_pBuffer = new unsigned short[m_Setup.text.clum * m_Setup.text.row];
}

TextCanvas::~TextCanvas()
{
	delete[] m_pLinebuf;
	delete[] m_nChrLine;
	delete[] m_pBuffer;
}

void TextCanvas::Clear()
{
	m_nLine = 0;
	m_nChr = 0;
	for (int i=0; i<m_Setup.text.row; i++){
		m_pLinebuf[i] = m_pBuffer;
		m_nChrLine[i] = 0;
	}
	m_chrCnt = 0;
	m_pool = 0;
}

void TextCanvas::Put(const char *msg)
{
	if (m_nLine == m_Setup.text.row)	return;
	bool lineTop = false;	// 禁則処理用
	while(*msg){
		unsigned short c = (unsigned char)*msg++;
		if (c!='\n'){
			if (c & 0x80){	// SJIS第1バイト
				c <<= 8;
				c |= (unsigned char)*msg++;
			}
			*m_pCurrent++ = c;	// ワイド文字
			++m_nChrLine[m_nLine];	// この行の文字数カウントアップ
			++m_nChr;				// トータル文字数カウントアップ
			if (lineTop){
				lineTop = false;
				while(m_nChrLine[m_nLine-1]>1){	// 前行が１文字以下なら、さすがに持ってこない
					switch(m_pLinebuf[m_nLine][0]){	// 行頭文字
					case 0x8141:	// 、
					case 0x8142:	// 。
					case 0x8143:	// ，
					case 0x8144:	// ．
					case 0x8145:	// ・
					case 0x8148:	// ？
					case 0x8149:	// ！
						--m_nChrLine[m_nLine-1];	// 前行から１文字削る
						++m_nChrLine[m_nLine];		// この行を１文字増やす
						--m_pLinebuf[m_nLine];		// この行のポインタを１文字前へ
						continue;
						break;
					}
					break;	// whileに対するbreak
				}
			}
		}
		if (c=='\n' || m_nChrLine[m_nLine] == m_Setup.text.clum){
			++m_nLine;	// 行数カウントアップ
			if (m_nLine == m_Setup.text.row)	return;
			m_pLinebuf[m_nLine] = m_pCurrent;
			if (c!='\n'){	// 強制改行時は禁則処理は行わない
				lineTop = true;	// 次は行頭なので禁則処理チェック
			}
		}
	}
}

void TextCanvas::Update(int msecElapseTime)
{
	// ししおどし
	m_pool += msecElapseTime;
	if (m_pool >= m_ChrSpeed){
		m_pool -= m_ChrSpeed;
		++m_chrCnt;
	}
}

void TextCanvas::Draw()
{
	int nChr = m_chrCnt;
	for (int i=0; i<m_nLine; i++){
		int n = (nChr < m_nChrLine[i])? nChr : m_nChrLine[i];

		draw(m_Setup.window.text.x, m_Setup.window.text.y+i*m_Setup.window.linefeed, m_pLinebuf[i], n);

		if (nChr <= m_nChrLine[i])	break;
		nChr -= m_nChrLine[i];
	}
}

