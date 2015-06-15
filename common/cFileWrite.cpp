//
// $Id: cFileWrite.cc,v 1.3 2004/04/25 17:27:35 wada Exp $

#include "cFileWrite.h"

using namespace std;

// �R���X�g���N�^
FileWrite::FileWrite(const char *fname) throw(badFileIO)
{
	if (fname==NULL)
		throw( badFileIO("FileWrite:: File name ptr is NULL", "") );
	if ((fopen_s(&ofp, fname, "wb")) != 0){
		throw( badFileIO("FileWrite:: Can't Open File ", fname) );
	}
}

FileWrite::~FileWrite()
{
	fclose(ofp);
}

void FileWrite::write(char data)
{
	fwrite(&data, 1, 1, ofp );
}

void FileWrite::write(short data)
{
	fwrite(&data, 2, 1, ofp );
}

void FileWrite::write(int data)
{
	fwrite(&data, 4, 1, ofp );
}

void FileWrite::write(const void *pData, int nData)
{
	fwrite(pData, 1, nData, ofp );
}

void FileWrite::seek(int offset, int pos)
{
	fseek(ofp, offset, pos);
}



// �t�@�C�����J���A�K�v�T�C�Y�̃������[���m�ۂ��ēǂݍ���
FileRead::FileRead(const char *filename) throw(badFileIO)
{
	FILE *fp;

	if (filename==NULL)
		throw( badFileIO("FileRead:: File name ptr is NULL", "") );
	if (fopen_s(&fp, filename, "rb") != 0){
		throw( badFileIO("FileRead:: Can't Open File ", filename) );
	}
	// �t�@�C���T�C�Y�����߂�̂�DOS�n�ˑ��ł͂Ȃ����n�I�ȕ��@��p����
	fseek(fp, 0, SEEK_END);		/* �Ō���܂ŃV�[�N */
	size = ftell(fp);		/* ���݂̃|�V�V���������߃t�@�C���T�C�Y�𓾂� */
	rewind(fp);				/* �擪�ɖ߂� */
	buf = new char[size];

	fread(buf, 1, size, fp);	/* �o�b�t�@�Ƀt�@�C����ǂݍ��� */
	fclose(fp);
}

FileRead::~FileRead()
{
	delete [] buf;
}

