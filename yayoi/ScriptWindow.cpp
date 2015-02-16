#include "stdafx.h"
#include "ScriptWindow.h"
#include "XmlBin/miku.h"
#include "sstream"

using namespace yayoi;


ScriptWindow::ScriptWindow(void)
{
	InitializeComponent();
}

void ScriptWindow::MikuPrint(const char *pBuf, XmlEnum& xmls, const hashmap_t& hashmap)
{
	scriptView->Nodes->Clear();

	int idx = 0;
	int indent = 0;
	m_NodeList.Clear();
	m_IndexList.Clear();
	TreeNode^ root = gcnew TreeNode();
	TreeNode^ tree = root;
	do {
		const miku::Node *pNode = reinterpret_cast<const miku::Node *>(&pBuf[idx]);
		if (pNode->isElementType()){
			const miku::ElementNode *pElement = static_cast<const miku::ElementNode *>(pNode);

			std::stringstream ss;
			ss << '[' << idx << ']';
			ss << "<" << xmls.ElementName(pElement->name);
			const miku::Attr *pAttr = reinterpret_cast<const miku::Attr *>(pElement+1);
			for (int i=0; i<pElement->getAttrNum(); i++, pAttr++){
				ss << " " << xmls.AttributeName(pAttr->name) << "=\"";
				switch (xmls.Type(pAttr->name)){
				case XmlEnum::STRING:
					ss << &pBuf[pAttr->value];
					break;
				case XmlEnum::INT:
				case XmlEnum::SHORT:
				case XmlEnum::BYTE:
					ss << pAttr->value;
					break;
				case XmlEnum::FLOAT:
					{
					float f = *reinterpret_cast<const float *>(&pAttr->value);
					ss << f;
					}
					break;
				case XmlEnum::CRC32:
					{
					hashmap_t::const_iterator it = hashmap.find(pAttr->value);
					if (it != hashmap.end())	ss << it->second;
					}
					break;
				}
				ss << "\"";
			}
			if (pElement->getChildNode()){
				ss << ">";
				TreeNode^ t = gcnew TreeNode( gcnew String( ss.str().c_str() ) );
				tree->Nodes->Add(t);
				m_NodeList.Add(t);
				m_IndexList.Add(idx);
				tree = t;
				idx =  pElement->getChildNode();
				indent++;
				continue;
			} else {
				ss << "/>";
				TreeNode^ t = gcnew TreeNode( gcnew String( ss.str().c_str() ) );
				tree->Nodes->Add(t);
				m_NodeList.Add(t);
				m_IndexList.Add(idx);
			}
		} else {
			const miku::TextNode *pText = static_cast<const miku::TextNode *>(pNode);
			TreeNode^ t = gcnew TreeNode( gcnew String(&pBuf[pText->entity]) );
			tree->Nodes->Add(t);
			m_NodeList.Add(t);
			m_IndexList.Add(idx);
		}
		while (pNode->isTerminate()){
			// 次がなければ１階層のエレメントに戻る
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<const miku::Node *>(&pBuf[idx]);
			indent--;
			const miku::ElementNode *pElement = static_cast<const miku::ElementNode *>(pNode);
			tree = tree->Parent;

//			if (indent==0)	break;	// rootまで上がってきたので終了。returnでも良い
			if (idx==0)	break;	// rootまで上がってきたので終了。returnでも良い
		}
		idx = pNode->getBrotherNode();
	} while (indent);	// 階層がない場合のみここで抜ける。

#if 0
	scriptView->Nodes->Add(root->FirstNode);
#else
	// バグ：最初の行しか表示されない
	tree = root->FirstNode->FirstNode;
	while (tree){
		TreeNode^ t = tree->NextNode;
		scriptView->Nodes->Add(tree);
		tree = t;
	}
#endif
	scriptView->ExpandAll();

#if 0
	scriptBox->Clear();

	int idx = 0;
	int indent = 0;
	do {
		const miku::Node *pNode = reinterpret_cast<const miku::Node *>(&pBuf[idx]);
		if (pNode->isElementType()){
			const miku::ElementNode *pElement = static_cast<const miku::ElementNode *>(pNode);

			for (int i=0; i<indent; i++){
				scriptBox->Text += "  ";
			}
			scriptBox->Text += "<";
			String^ str = gcnew String(xmls.ElementName(pElement->name));
			scriptBox->Text += str;
			const miku::Attr *pAttr = reinterpret_cast<const miku::Attr *>(pElement+1);
			for (int i=0; i<pElement->getAttrNum(); i++, pAttr++){
				scriptBox->Text += " ";
				{
					String^  str = gcnew String(xmls.AttributeName(pAttr->name));
					scriptBox->Text += str;
				}
				scriptBox->Text += "=\"";
				{
					switch (xmls.Type(pAttr->name)){
					case XmlEnum::STRING:
						{
						String^ str = gcnew String(	&pBuf[pAttr->value] );
						scriptBox->Text += str;
						}
						break;
					case XmlEnum::INT:
					case XmlEnum::SHORT:
					case XmlEnum::BYTE:
						scriptBox->Text += pAttr->value;
						break;
					case XmlEnum::FLOAT:
						{
						float f = *reinterpret_cast<const float *>(&pAttr->value);
						scriptBox->Text += f;
						}
						break;
					}
				}
				scriptBox->Text += "\"";
			}
			if (pElement->getChildNode()){
				scriptBox->Text += ">\r\n";
				idx =  pElement->getChildNode();
				indent++;
				continue;
			} else {
				scriptBox->Text += "/>\r\n";
			}
		} else {
			const miku::TextNode *pText = static_cast<const miku::TextNode *>(pNode);

			for (int i=0; i<indent; i++){
				scriptBox->Text += "  ";
			}
			String^ str = gcnew String(&pBuf[pText->entity]);
			scriptBox->Text += str;
			scriptBox->Text += "\r\n";
		}
		while (pNode->isTerminate()){
			// 次がなければ１階層のエレメントに戻る
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<const miku::Node *>(&pBuf[idx]);
			indent--;
			const miku::ElementNode *pElement = static_cast<const miku::ElementNode *>(pNode);
			for (int i=0; i<indent; i++){
				scriptBox->Text += "  ";
			}
			scriptBox->Text += "</";
			String^ str = gcnew String(xmls.ElementName(pElement->name));
			scriptBox->Text += str;
			scriptBox->Text += ">\r\n";

//			if (indent==0)	break;	// rootまで上がってきたので終了。returnでも良い
			if (idx==0)	break;	// rootまで上がってきたので終了。returnでも良い
		}
		idx = pNode->getBrotherNode();
	} while (indent);	// 階層がない場合のみここで抜ける。
#endif
}

/**
 * 実行中の場所を選択中として強調
 */
void ScriptWindow::SelectedLine(int idx)
{
	int n = m_IndexList.IndexOf(idx);
	scriptView->SelectedNode = m_NodeList[n];
}

/**
 * 既読を色づけ
 */
void ScriptWindow::CheckLine(int idx)
{
	int n = m_IndexList.IndexOf(idx);
	m_NodeList[n]->ForeColor = System::Drawing::Color::Violet;
}

/**
 *	選択されているノードの実行インデックスを返す
 */
int ScriptWindow::GetSelectedIndex()
{
	TreeNode^ t = scriptView->SelectedNode;
	int n = m_NodeList.IndexOf(t);
	return m_IndexList[n];
}
