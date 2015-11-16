// iona.cpp : アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "iona.h"
#include "avs.h"
#include "pao/DX11/FrameworkDX11.h"
#include "pao/Windows/FileDialog.h"
#include "ImageTreeView.h"
#include <string>

XmlEnum sinkxsd;

void LoadSchema()
{
	TiXmlDocument schema;
	// スキーマを読む
	if (!schema.LoadFile("sink.xsd")){
		printf("file read error (sink.xsd)\n");
		exit(0);
	}
	//	XmlEnum xmls;
	sinkxsd.ReadSchema(schema.RootElement());
}

void LoadScript(std::vector<char>& scriptbuf, const char* path, XmlEnum& xmls)
{
	TiXmlDocument xml;
	// スクリプトを読む
	if (!xml.LoadFile(path)){
		printf("file read error %s\n", path);
		exit(0);
	}

	// バイナリに変換
	hashmap_t hashmap;
	XmlBin().Conv(scriptbuf, hashmap, xml, &xmls);
}

class ScriptTreeView : public ITreeViewControl {
	char *pBuf;
	XmlEnum& xmls;

public:
	ScriptTreeView(char *pBuf, XmlEnum& xmls)
		: pBuf(pBuf)
		, xmls(xmls)
	{}

private:
	virtual void Setup(HWND hTree) override
	{
		int idx = 0;
		int indent = 0;
		std::vector<HTREEITEM> tv;
		do {
			miku::Node *pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
			if (pNode->isElementType()){
				miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);

				TCHAR str[256];
				MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, xmls.ElementName(pElement->name), -1, str, sizeof(str) / 2);
				HTREEITEM parent = (tv.empty())? NULL : tv.back();
				HTREEITEM item = TreeViewAddItem(hTree, parent, str);

				if (pElement->getChildNode()){
					idx = pElement->getChildNode();
					tv.push_back(item);
					indent++;
					continue;
				}
			}
			while (pNode->isTerminate()){
				// 次がなければ１階層のエレメントに戻る
				idx = pNode->getNextNode();
				pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
				tv.pop_back();
				indent--;
				//			if (indent==0)	break;	// rootまで上がってきたので終了。returnでも良い
				if (idx == 0)	break;	// rootまで上がってきたので終了。returnでも良い
			}
			idx = pNode->getBrotherNode();
		} while (indent);	// 階層がない場合のみここで抜ける。
	}
};

namespace {
	std::vector<char> scriptBuf;

	void OpenScriptFile(HINSTANCE hInst, HWND hWnd)
	{
		pao::FileDialog fileDialog(hWnd);
		fileDialog.SetFilter(L"script(*.xml)\0*.xml\0" L"all(*.*)\0*.*\0" L"\0");

		if (fileDialog.DialogBoxOpen() == TRUE){
			char fpath[MAX_PATH];
			WideCharToMultiByte(CP_OEMCP, 0, fileDialog.GetFullPath(), -1, fpath, MAX_PATH, NULL, NULL);
			LoadScript(scriptBuf, fpath, sinkxsd);

			// 上書き保存のメニューを選択可にする
			HMENU hMenu = GetMenu(hWnd);
			EnableMenuItem(hMenu, IDM_OVERWRITE, MF_ENABLED);


			ScriptTreeView* scriptTree = new ScriptTreeView(&scriptBuf[0], sinkxsd);
			ImageTreeView* treeView = new ImageTreeView(hInst, IDD_DIALOG1, hWnd);
			treeView->AddControl(scriptTree);
			treeView->SetTitle(fileDialog.GetFileName());
		}
	}

	void SaveScriptFile(HWND hWnd)
	{
		pao::FileDialog fileDialog(hWnd);
		fileDialog.SetTitle(L"名前を付けて保存");
		fileDialog.SetFilter(L"script(*.xml)\0*.xml\0");

		if (fileDialog.DialogBoxSave() == TRUE){
		}
	}
}

//using BaseClass = pao::FrameworkDX11;
using BaseClass = pao::FrameworkWindows;

class MyFramework : public BaseClass {
		static const int MAX_LOADSTRING = 100;
	TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
	TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
	HACCEL hAccelTable;

public:
	MyFramework(HINSTANCE hInstance) : BaseClass(hInstance)
	{
		LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		LoadString(hInstance, IDC_IONA, szWindowClass, MAX_LOADSTRING);

		SetIcon(LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IONA)), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		SetMenu(MAKEINTRESOURCE(IDC_IONA));
		SetClassName(szWindowClass);

		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IONA));

		LoadSchema();
	}
	~MyFramework()
	{
	}

	int Execute(int nCmdShow)
	{
		return FrameworkWindows::Execute(szTitle, nCmdShow);
	}

private:
	// アプリケーション メニューの処理
	virtual LRESULT OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) override
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// 選択されたメニューの解析:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(GetAppInstanceHandle(), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case IDM_OPEN:
			OpenScriptFile(GetAppInstanceHandle(), hWnd);
			break;
		case IDM_SAVE:
			SaveScriptFile(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
		}
		return 0;
	}
};


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyFramework myFrameWork(hInstance);
	return myFrameWork.Execute(nCmdShow);
}
