#ifndef C_FILE_WRITE_H
#define C_FILE_WRITE_H

#include <cstdio>
#include <string>

class badFileIO {
public:
	badFileIO(const char *p, const char *s) : msg(p) { msg+=s; }
	std::string msg;
};

class FileWrite {
public:
	FileWrite(const char *) throw(badFileIO);	// �o�̓t�@�C����
	~FileWrite();
	void write(char);
	void write(short);
	void write(int);
	void write(const void *, int);
	void seek(int ofset, int pos=0);
private:
	FileWrite(const FileWrite &);	// �R�s�[�R���X�g���N�^�͕s�v
	FileWrite & operator=(const FileWrite &);	// ������Z���s�v
	std::FILE *ofp;
};

class FileRead {
public:
	FileRead(const char *filename) throw(badFileIO);
	~FileRead();
	const void *Buf() const { return buf; };
	const void *Buf(int ofs) const { return buf+ofs; };
	std::size_t Size() const { return size; };
private:
	FileRead(const FileRead &);	// �R�s�[�R���X�g���N�^�͕s�v
	FileRead& operator=(const FileRead &);	// ������Z���s�v

	char *buf;
	std::size_t size;
};


#endif	// C_FILE_WRITE_H

