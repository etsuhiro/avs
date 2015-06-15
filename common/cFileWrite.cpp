//
// $Id: cFileWrite.cc,v 1.3 2004/04/25 17:27:35 wada Exp $

#include "cFileWrite.h"

using namespace std;

// コンストラクタ
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



// ファイルを開き、必要サイズのメモリーを確保して読み込む
FileRead::FileRead(const char *filename) throw(badFileIO)
{
	FILE *fp;

	if (filename==NULL)
		throw( badFileIO("FileRead:: File name ptr is NULL", "") );
	if (fopen_s(&fp, filename, "rb") != 0){
		throw( badFileIO("FileRead:: Can't Open File ", filename) );
	}
	// ファイルサイズを求めるのにDOS系依存ではなく原始的な方法を用いる
	fseek(fp, 0, SEEK_END);		/* 最後尾までシーク */
	size = ftell(fp);		/* 現在のポシションを求めファイルサイズを得る */
	rewind(fp);				/* 先頭に戻す */
	buf = new char[size];

	fread(buf, 1, size, fp);	/* バッファにファイルを読み込む */
	fclose(fp);
}

FileRead::~FileRead()
{
	delete [] buf;
}

