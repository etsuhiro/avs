// iona.cpp : �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
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
	// �X�L�[�}��ǂ�
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
	// �X�N���v�g��ǂ�
	if (!xml.LoadFile(path)){
		printf("file read error %s\n", path);
		exit(0);
	}

	// �o�C�i���ɕϊ�
	hashmap_t hashmap;
	XmlBin().Conv(scriptbuf, hashmap, xml, &xmls);
}

void mikuPrint(char *pBuf, XmlEnum& xmls)
{
	int idx = 0;
	int indent = 0;
	do {
		miku::Node *pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
		if (pNode->isElementType()){
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);

			for (int i = 0; i<indent; i++)
				printfDx("  ");
			printfDx("<%s", xmls.ElementName(pElement->name));
			miku::Attr *pAttr = reinterpret_cast<miku::Attr *>(pElement + 1);
			for (int i = 0; i<pElement->getAttrNum(); i++, pAttr++){
				printfDx(" %s=???", xmls.AttributeName(pAttr->name));
			}
			if (pElement->getChildNode()){
				printfDx(">\n");
				idx = pElement->getChildNode();
				indent++;
				continue;
			}
			else {
				printfDx("/>\n");
			}
		}
		else {
			miku::TextNode *pText = static_cast<miku::TextNode *>(pNode);

			for (int i = 0; i<indent; i++)
				printfDx("  ");
			printfDx("%s\n", &pBuf[pText->entity]);
		}
		while (pNode->isTerminate()){
			// �����Ȃ���΂P�K�w�̃G�������g�ɖ߂�
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
			indent--;
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);
			for (int i = 0; i<indent; i++)
				printfDx("  ");
			printfDx("</%s>\n", xmls.ElementName(pElement->name));

			//			if (indent==0)	break;	// root�܂ŏオ���Ă����̂ŏI���Breturn�ł��ǂ�
			if (idx == 0)	break;	// root�܂ŏオ���Ă����̂ŏI���Breturn�ł��ǂ�
		}
		idx = pNode->getBrotherNode();
	} while (indent);	// �K�w���Ȃ��ꍇ�݂̂����Ŕ�����B
	ScreenFlip();
}

class ScriptTreeView : public ITreeViewControl {

	virtual void Setup(HWND hTree) override
	{
		TreeViewItem(hTree, TEXT("�e�P"));
		{
			TreeViewItem tree(hTree, TEXT("�e�Q"));
			TreeViewItem(hTree, TEXT("�q�P"));
			TreeViewItem(hTree, TEXT("�q�Q"));
		}
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

			// �㏑���ۑ��̃��j���[��I���ɂ���
			HMENU hMenu = GetMenu(hWnd);
			EnableMenuItem(hMenu, IDM_OVERWRITE, MF_ENABLED);


			ImageTreeView* treeView = new ImageTreeView();
			HWND hwndTreeView = treeView->Create(hInst, IDD_DIALOG1, hWnd);
			ShowWindow(hwndTreeView, SW_SHOW);
		}
	}

	void SaveScriptFile(HWND hWnd)
	{
		pao::FileDialog fileDialog(hWnd);
		fileDialog.SetTitle(L"���O��t���ĕۑ�");
		fileDialog.SetFilter(L"script(*.xml)\0*.xml\0");

		if (fileDialog.DialogBoxSave() == TRUE){
		}
	}
}

//using BaseClass = pao::FrameworkDX11;
using BaseClass = pao::FrameworkWindows;

class MyFramework : public BaseClass {
		static const int MAX_LOADSTRING = 100;
	TCHAR szTitle[MAX_LOADSTRING];					// �^�C�g�� �o�[�̃e�L�X�g
	TCHAR szWindowClass[MAX_LOADSTRING];			// ���C�� �E�B���h�E �N���X��
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
	// �A�v���P�[�V���� ���j���[�̏���
	virtual LRESULT OnCommand(HWND hWnd, WPARAM wParam, LPARAM lParam) override
	{
		int wmId = LOWORD(wParam);
		int wmEvent = HIWORD(wParam);
		// �I�����ꂽ���j���[�̉��:
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
