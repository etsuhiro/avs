#pragma once

#include <windows.h>
#include <commdlg.h>

namespace pao {
	class FileDialog {
		TCHAR m_fullPath[MAX_PATH];   // ファイル名(フルパス)を受け取る領域
		OPENFILENAME m_ofn;

	public:
		FileDialog(HWND hWnd);

		// ファイルの種類
		void SetFilter(LPCWSTR lpstrFilter);

		// フラグ指定
		void SetFlags(DWORD flags);

		// コモンダイアログのキャプション
		void SetTitle(LPCWSTR lpstrTitle);

		// デフォルトのファイルの種類
		void SetDefExt(LPCWSTR lpstrDefExt);

		// 初期フォルダの指定
		void SetInitialDir(LPCWSTR lpstrInitialDir);

		// ファイルを開くコモンダイアログを作成
		BOOL DialogBoxOpen();

		// ファイルを保存するコモンダイアログを作成
		BOOL FileDialog::DialogBoxSave();

		TCHAR* GetFullPath();
	};
}