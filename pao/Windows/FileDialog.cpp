#include "FileDialog.h"

using namespace pao;

FileDialog::FileDialog(HWND hWnd)
{
	m_fullPath[0] = 0;
	ZeroMemory(&m_ofn, sizeof(m_ofn));				// �ŏ��Ƀ[���N���A���Ă���
	m_ofn.lStructSize = sizeof(m_ofn);				// �\���̂̃T�C�Y
	m_ofn.hwndOwner = hWnd;						// �R�����_�C�A���O�̐e�E�B���h�E�n���h��
	m_ofn.lpstrFilter = L"all(*.*)\0*.*\0\0";	// �t�@�C���̎��
	m_ofn.lpstrFile = m_fullPath;				// �I�����ꂽ�t�@�C����(�t���p�X)���󂯎��ϐ��̃A�h���X
	m_ofn.nMaxFile = MAX_PATH;		// lpstrFile�Ɏw�肵���ϐ��̃T�C�Y
	m_ofn.Flags = OFN_FILEMUSTEXIST;		// �t���O�w��
	m_ofn.lpstrTitle = L"�t�@�C�����J��";		// �R�����_�C�A���O�̃L���v�V����
}

// �t�@�C���̎��
void FileDialog::SetFilter(LPCWSTR lpstrFilter)
{
	m_ofn.lpstrFilter = lpstrFilter;
}

// �t���O�w��
void FileDialog::SetFlags(DWORD flags)
{
	m_ofn.Flags = flags;
}

// �R�����_�C�A���O�̃L���v�V����
void FileDialog::SetTitle(LPCWSTR lpstrTitle)
{
	m_ofn.lpstrTitle = lpstrTitle;
}

// �f�t�H���g�̃t�@�C���̎��
void FileDialog::SetDefExt(LPCWSTR lpstrDefExt)
{
	m_ofn.lpstrDefExt = lpstrDefExt;
}

// �����t�H���_�̎w��
void FileDialog::SetInitialDir(LPCWSTR lpstrInitialDir)
{
	m_ofn.lpstrInitialDir = lpstrInitialDir;
}

// �t�@�C�����J���R�����_�C�A���O���쐬
BOOL FileDialog::DialogBoxOpen()
{
	return GetOpenFileName(&m_ofn);
}

TCHAR* FileDialog::GetFullPath()
{
	return m_fullPath;
}

// �t�@�C����ۑ�����R�����_�C�A���O���쐬
BOOL FileDialog::DialogBoxSave()
{
	return GetSaveFileName(&m_ofn);
}

