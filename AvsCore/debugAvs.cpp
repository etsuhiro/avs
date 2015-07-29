#include <cstdio>
#include <vector>
#include <string>
#include <map>

#include "avs.h"

XmlEnum xmls;	// ちょっとテスト

class AvsText : public avs::avsTextListener {
	void Put(const char* str, float time, int id)
	{
		if (!m_out){
			printf("「");
		}
		m_out = true;
		printf("%s", str);
	}

	void Crlf()
	{
		printf("\n");
	}

	void Attr(TEXT_ATTR attr)
	{
	}

	void Option(const char* str, int num)
	{
		if (m_out){
			printf("」\n");
			m_out = false;
		}
		printf(" %d:%s\n", num + 1, str);
	}

	void Clear()
	{
	}

	void Click()
	{
	}

	void Window(float x, float y)
	{
	}

	bool IsBusy()
	{
		return false;
	}

public:
	AvsText() : m_out(false) {}

	void Update()
	{
		if (m_out){
			printf("」\n");
			m_out = false;
		}
	}

private:
	bool m_out;
};

class Variable : public avs::VariableListener {
	void Set(const char* name, int val)
	{
		printf("set %s=%d\n", name, val);
		m_item[name] =val;
	}
	int Get(const char* name)
	{
		std::map<std::string, int>::iterator it = m_item.find(name);
		if (it==m_item.end()){
			printf("not found %s\n", name);
			return 0;
		}
		return m_item[name];
	}
	int KeyOn(int key)
	{
		return 0;
	}

	std::map<std::string, int> m_item;
};

struct JumpTo {
	const char *label;
	const char *file;
};

class MyScriptEngine : public avs::ScriptEngine {
	virtual void ErrorInvalidElement(int cmd, int element)
	{
		printf(" warning: <%s>タグ内では<%s>タグを子要素に出来ません。\n", xmls.ElementName(cmd), xmls.ElementName(element));
	}
	virtual void ErrorInvalidAttr(int cmd, int attr)
	{
		printf(" warning: <%s>タグに\"%s\"属性は無効です。\n", xmls.ElementName(cmd), xmls.AttributeName(attr));
	}
	virtual void ErrorrequiredAttr(int cmd, int attr)
	{
		printf(" warning: <%s>タグには\"s\"属性が必須です。\n", xmls.ElementName(cmd), xmls.AttributeName(attr));
	}
	virtual void ErrorInvalidText(const char *tag, const char* str)
	{
		printf(" warning: %s内で文字データは無効です。「%s」\n", tag, str);
	}
	virtual void setOptionJump(const char *label, const char *file, int )
	{
		JumpTo jumpTo = {label, file};
		m_option.push_back( jumpTo );
	}
	virtual void getOptionJump(int n)
	{
//		setJumpLabel(m_option[n].label);
//		setJumpFile(m_option[n].file);
		m_option.clear();
	}

	std::vector<JumpTo> m_option;
};

void mikuPrint(char *pBuf, XmlEnum& xmls)
{
	int idx = 0;
	int indent = 0;
	do {
		miku::Node *pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
		if (pNode->isElementType()){
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);

			for (int i=0; i<indent; i++)
				printf("  ");
			printf("<%s", xmls.ElementName(pElement->name));
			miku::Attr *pAttr = reinterpret_cast<miku::Attr *>(pElement+1);
			for (int i=0; i<pElement->getAttrNum(); i++, pAttr++){
				printf(" %s=???", xmls.AttributeName(pAttr->name));
			}
			if (pElement->getChildNode()){
				printf(">\n");
				idx =  pElement->getChildNode();
				indent++;
				continue;
			} else {
				printf("/>\n");
			}
		} else {
			miku::TextNode *pText = static_cast<miku::TextNode *>(pNode);

			for (int i=0; i<indent; i++)
				printf("  ");
			printf("%s\n", &pBuf[pText->entity]);
		}
		while (pNode->isTerminate()){
			// 次がなければ１階層のエレメントに戻る
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
			indent--;
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);
			for (int i=0; i<indent; i++)
				printf("  ");
			printf("</%s>\n", xmls.ElementName(pElement->name));

//			if (indent==0)	break;	// rootまで上がってきたので終了。returnでも良い
			if (idx==0)	break;	// rootまで上がってきたので終了。returnでも良い
		}
		idx = pNode->getBrotherNode();
	} while (indent);	// 階層がない場合のみここで抜ける。
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
	XmlBin().Conv(scriptbuf, hashmap, xml, &xmls );
//	mikuPrint(&scriptbuf[0], xmls);
}

void SelectOption(MyScriptEngine& script);

int main(int argc, char *argv[])
{
	if (argc!=2){
		printf("usage debugAvs script\n");
		exit(0);
	}
	std::string path;
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		_splitpath_s(argv[1], drive, _MAX_DRIVE, dir, _MAX_DIR, NULL, 0, NULL, 0);
		path = drive;
		path += dir;
	}

	TiXmlDocument schema;
	// スキーマを読む
	if (!schema.LoadFile("sink.xsd")){
//		printf("file read error (sink.xsd)\n");
		exit(0);
	}
//	XmlEnum xmls;
	xmls.ReadSchema(schema.RootElement());
//	xmls.PrintEnum("sink");

	std::vector<char> scriptBuf;
	LoadScript(scriptBuf, argv[1], xmls);

	MyScriptEngine script;
	script.SetScript(&scriptBuf[0]);
	AvsText avsText;
	script.AddTextListener(&avsText);
	Variable variable;
	script.AddVariableListener(&variable);

	int stat;
	do {
		stat = script.Run(0);
		avsText.Update();
		switch (stat){
		case avs::CLICK:
			getchar();
			break;
		case avs::SELECT:
			SelectOption(script);
			break;
		case avs::LABEL_JUMP:
			if (script.getJumpFile()){
				std::string file;
				file = path + script.getJumpFile() + ".xml";
				LoadScript(scriptBuf, file.c_str(), xmls);
				script.SetScript(&scriptBuf[0]);	// scriptBufが変わったので再セット
			}
			break;
		}
	} while(stat!=avs::FINISH);
	printf("  *** 終了しました。何かキーを押してください。");
	getchar();
}

void SelectOption(MyScriptEngine& script)
{
#if 0
	int num = script.getOptionNum();
	int c;
	do {
		// かなりいいかげんな入力ルーチンだけど、テスト用だから
		printf("enter 1-%d: ", num);
		c = getchar();
	} while(c<'1' ||  '0'+num<c);
	script.GotoOption(c-'1');
#endif
}
