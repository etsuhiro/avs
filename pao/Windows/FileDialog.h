#pragma once

#include <windows.h>
#include <commdlg.h>

namespace pao {
	class FileDialog {
		TCHAR m_fullPath[MAX_PATH];   // �t�@�C����(�t���p�X)���󂯎��̈�
		OPENFILENAME m_ofn;

	public:
		FileDialog(HWND hWnd);

		// �t�@�C���̎��
		void SetFilter(LPCWSTR lpstrFilter);

		// �t���O�w��
		void SetFlags(DWORD flags);

		// �R�����_�C�A���O�̃L���v�V����
		void SetTitle(LPCWSTR lpstrTitle);

		// �f�t�H���g�̃t�@�C���̎��
		void SetDefExt(LPCWSTR lpstrDefExt);

		// �����t�H���_�̎w��
		void SetInitialDir(LPCWSTR lpstrInitialDir);

		// �t�@�C�����J���R�����_�C�A���O���쐬
		BOOL DialogBoxOpen();

		// �t�@�C����ۑ�����R�����_�C�A���O���쐬
		BOOL FileDialog::DialogBoxSave();

		TCHAR* GetFullPath();
	};
}