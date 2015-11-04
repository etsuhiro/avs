#include "FileDialog.h"

using namespace pao;

FileDialog::FileDialog(HWND hWnd)
{
	m_fullPath[0] = 0;
	ZeroMemory(&m_ofn, sizeof(m_ofn));				// 最初にゼロクリアしておく
	m_ofn.lStructSize = sizeof(m_ofn);				// 構造体のサイズ
	m_ofn.hwndOwner = hWnd;						// コモンダイアログの親ウィンドウハンドル
	m_ofn.lpstrFilter = L"all(*.*)\0*.*\0\0";	// ファイルの種類
	m_ofn.lpstrFile = m_fullPath;				// 選択されたファイル名(フルパス)を受け取る変数のアドレス
	m_ofn.nMaxFile = MAX_PATH;		// lpstrFileに指定した変数のサイズ
	m_ofn.Flags = OFN_FILEMUSTEXIST;		// フラグ指定
	m_ofn.lpstrTitle = L"ファイルを開く";		// コモンダイアログのキャプション
}

// ファイルの種類
void FileDialog::SetFilter(LPCWSTR lpstrFilter)
{
	m_ofn.lpstrFilter = lpstrFilter;
}

// フラグ指定
void FileDialog::SetFlags(DWORD flags)
{
	m_ofn.Flags = flags;
}

// コモンダイアログのキャプション
void FileDialog::SetTitle(LPCWSTR lpstrTitle)
{
	m_ofn.lpstrTitle = lpstrTitle;
}

// デフォルトのファイルの種類
void FileDialog::SetDefExt(LPCWSTR lpstrDefExt)
{
	m_ofn.lpstrDefExt = lpstrDefExt;
}

// 初期フォルダの指定
void FileDialog::SetInitialDir(LPCWSTR lpstrInitialDir)
{
	m_ofn.lpstrInitialDir = lpstrInitialDir;
}

// ファイルを開くコモンダイアログを作成
BOOL FileDialog::DialogBoxOpen()
{
	return GetOpenFileName(&m_ofn);
}

TCHAR* FileDialog::GetFullPath()
{
	return m_fullPath;
}

// ファイルを保存するコモンダイアログを作成
BOOL FileDialog::DialogBoxSave()
{
	return GetSaveFileName(&m_ofn);
}

