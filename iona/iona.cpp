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

namespace avsutil {
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
}

class TreePrint : public IMikuPrint {
	HWND hTree;
	HTREEITEM item;
	std::vector<HTREEITEM> tv;

public:
	TreePrint(HWND hTree) : hTree(hTree) {}

private:
	virtual void TagName(const char* name, int indent) override
	{
		TCHAR str[256];
		MultiByteToWideChar(CP_OEMCP, MB_PRECOMPOSED, name, -1, str, sizeof(str) / 2);
		HTREEITEM parent = (tv.empty()) ? NULL : tv.back();
		item = TreeViewAddItem(hTree, parent, str);
	}

	virtual void TagClose(const char* name, int indent) override
	{
		tv.push_back(item);
	}

	virtual void EmptyElementTagClose(const char* name, int indent) override
	{
	}

	virtual void EndTag(const char* name, int indent) override
	{
		tv.pop_back();
	}

	virtual void AttrName(const char* name) override
	{
	}

	virtual void Text(const char* text, int indent) override
	{
	}
};

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
		mikuPrint(pBuf, xmls, TreePrint(hTree));
#if 0
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
#endif
	}
};

namespace {
	std::vector<char> scriptBuf;

	void OpenScriptFile(HINSTANCE hInst, HWND hWnd, avs::ScriptEngine& script)
	{
		pao::FileDialog fileDialog(hWnd);
		fileDialog.SetFilter(L"script(*.xml)\0*.xml\0" L"all(*.*)\0*.*\0" L"\0");

		if (fileDialog.DialogBoxOpen() == TRUE){
			char fpath[MAX_PATH];
			WideCharToMultiByte(CP_OEMCP, 0, fileDialog.GetFullPath(), -1, fpath, MAX_PATH, NULL, NULL);
			avsutil::LoadScript(scriptBuf, fpath, sinkxsd);
			script.SetScript(&scriptBuf[0]);

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

class MyScriptEngine : public avs::ScriptEngine
{
};

using BaseClass = pao::FrameworkDX11;
//using BaseClass = pao::FrameworkWindows;

class MyFramework : public BaseClass {
		static const int MAX_LOADSTRING = 100;
	HACCEL hAccelTable;
	MyScriptEngine script;

public:
	MyFramework(HINSTANCE hInstance) : BaseClass(hInstance)
	{
		hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_IONA));
	}
	~MyFramework()
	{
	}

	void Init(HINSTANCE hInstance, int nCmdShow)
	{
		TCHAR szTitle[MAX_LOADSTRING];					// タイトル バーのテキスト
		LoadString(GetAppInstanceHandle(), IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		TCHAR szWindowClass[MAX_LOADSTRING];			// メイン ウィンドウ クラス名
		LoadString(hInstance, IDC_IONA, szWindowClass, MAX_LOADSTRING);
		HWND hWnd = Create(hInstance, szWindowClass, szTitle);
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);

		avsutil::LoadSchema();
	}

private:
	virtual void MakeWindow(WNDCLASSEX& wcex) override
	{
		HINSTANCE hInstance = GetAppInstanceHandle();

		SetIcon(LoadIcon(hInstance, MAKEINTRESOURCE(IDI_IONA)), LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SMALL)));
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		SetMenu(MAKEINTRESOURCE(IDC_IONA));
	}

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
			OpenScriptFile(GetAppInstanceHandle(), hWnd, script);
			break;
		case IDM_SAVE:
			SaveScriptFile(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, WM_COMMAND, wParam, lParam);
		}
		return 0;
	}

//	virtual void Render(ID3D11DeviceContext*) override
//	{
//		avs::RunningStatus stat = script.Run(16.f);
//	}
};


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	MyFramework myFrameWork(hInstance);
	myFrameWork.Init(hInstance, nCmdShow);
	return myFrameWork.Run();
}
