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
	bool lineTop = false;	// �֑������p
	while(*msg){
		unsigned short c = (unsigned char)*msg++;
		if (c!='\n'){
			if (c & 0x80){	// SJIS��1�o�C�g
				c <<= 8;
				c |= (unsigned char)*msg++;
			}
			*m_pCurrent++ = c;	// ���C�h����
			++m_nChrLine[m_nLine];	// ���̍s�̕������J�E���g�A�b�v
			++m_nChr;				// �g�[�^���������J�E���g�A�b�v
			if (lineTop){
				lineTop = false;
				while(m_nChrLine[m_nLine-1]>1){	// �O�s���P�����ȉ��Ȃ�A�������Ɏ����Ă��Ȃ�
					switch(m_pLinebuf[m_nLine][0]){	// �s������
					case 0x8141:	// �A
					case 0x8142:	// �B
					case 0x8143:	// �C
					case 0x8144:	// �D
					case 0x8145:	// �E
					case 0x8148:	// �H
					case 0x8149:	// �I
						--m_nChrLine[m_nLine-1];	// �O�s����P�������
						++m_nChrLine[m_nLine];		// ���̍s���P�������₷
						--m_pLinebuf[m_nLine];		// ���̍s�̃|�C���^���P�����O��
						continue;
						break;
					}
					break;	// while�ɑ΂���break
				}
			}
		}
		if (c=='\n' || m_nChrLine[m_nLine] == m_Setup.text.clum){
			++m_nLine;	// �s���J�E���g�A�b�v
			if (m_nLine == m_Setup.text.row)	return;
			m_pLinebuf[m_nLine] = m_pCurrent;
			if (c!='\n'){	// �������s���֑͋������͍s��Ȃ�
				lineTop = true;	// ���͍s���Ȃ̂ŋ֑������`�F�b�N
			}
		}
	}
}

void TextCanvas::Update(int msecElapseTime)
{
	// �������ǂ�
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

