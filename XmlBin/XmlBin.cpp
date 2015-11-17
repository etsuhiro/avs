#include "XmlBin.h"

#include <cstdio>

#include "common/Strbank.h"
#include "common/crc32.h"
#include "tinyxml/tinyxml.h"
#include "miku.h"
#include "XmlEnum/XmlEnum.h"

namespace
{
	class myStrbank : public Strbank {
		std::vector<char>& m_buf;
	public:
		myStrbank(std::vector<char>& buf) :m_buf(buf) {}
	private:
		void output(const char *str, int size)
		{
			while(size--)
				m_buf.push_back(*str++);
		}
	};

	class xmlPerse {
		virtual void elementName(const char*) {}
		virtual void attrProc(const TiXmlAttribute *) {}
		virtual void textProc(const char *) {}	// �e�L�X�g
		virtual void attrNum(int, int) {}
	protected:
		~xmlPerse(){}

		std::vector<char>& m_buf;	// ���������o�C�i���X�N���v�g���i�[�����
		int m_ofs;
		XmlEnum* m_enum;	// enum pack�p�̃X�L�[�}

		xmlPerse(std::vector<char>& buf, XmlEnum* xenum) : m_buf(buf), m_ofs(0), m_enum(xenum) {}

		void Element(const TiXmlElement* e)
		{
			int etop = m_ofs;
			elementName( e->Value() );
			m_ofs += sizeof(miku::ElementNode);

			// �A�g���r���[�g�̏���
			int n=0;
			for (const TiXmlAttribute* attr = e->FirstAttribute(); attr; attr = attr->Next())
			{
				attrProc( attr );
				m_ofs += sizeof(miku::Attr);
				n++;
			}
			attrNum(n, etop);	// �A�g���r���[�g�̐����Z�b�g

			int next = etop + sizeof(miku::Node);	// ElementNode::childNode�̈ʒu���w��

			for (const TiXmlNode* child = e->FirstChild(); child; child = child->NextSibling())
			{
				const TiXmlText* childText = child->ToText();
				if ( childText ) {
					miku::Offset *pOffset = reinterpret_cast<miku::Offset *>(&m_buf[next]);
					*pOffset |= m_ofs;
					next = m_ofs;	// child��Node::nextNode�̈ʒu���w���i���͖̂��o�^�Ȃ̂Ń|�C���^�ł͎��ĂȂ��j

					textProc( childText->Value() );	// �e�L�X�g
					m_ofs += sizeof(miku::TextNode);
				}
		
				const TiXmlElement* childElement = child->ToElement();
				if (childElement){
					miku::Offset *pOffset = reinterpret_cast<miku::Offset *>(&m_buf[next]);
					*pOffset |= m_ofs;
					next = m_ofs;	// child��Node::nextNode�̈ʒu���w���i���͖̂��o�^�Ȃ̂Ń|�C���^�ł͎��ĂȂ��j

					Element(childElement);
				}
			}
		}
	};

	class pass1 : public xmlPerse {
		myStrbank& m_mystr;
	public:
		pass1(std::vector<char>& buf, myStrbank& mystr, XmlEnum* xenum)
		 : xmlPerse(buf, xenum), m_mystr(mystr) {}

		virtual ~pass1(){}

		void Elements(const TiXmlElement* e)
		{
			Element(e);
			m_mystr.Calc(m_ofs);
		}

	private:
		void elementName(const char *p)
		{
			// �o�b�t�@�ɗv�f�����m��
			for (int i=0; i<sizeof(miku::ElementNode); i++)
				m_buf.push_back(0);

			if (m_enum==0)
				m_mystr.Add( p );	// �v�f��
		}

		void attrProc(const TiXmlAttribute *attr)
		{
			// �o�b�t�@�ɑ��������m��
			for (int i=0; i<sizeof(miku::Attr); i++)
				m_buf.push_back(0);

			if (m_enum==0){
				m_mystr.Add( attr->Name() );	// ������
				m_mystr.Add( attr->Value() );
			} else {
				if (m_enum->Type(m_enum->Attribute(attr->Name()))==XmlEnum::STRING){
					m_mystr.Add( attr->Value() );	// �����l
				}
			}
		}

		void attrNum(int n, int etop)
		{
			miku::ElementNode *pElement = reinterpret_cast<miku::ElementNode *>(&m_buf[etop]);
//			pElement->nAttr = n;
			pElement->childNode = n<<24;
		}

		void textProc(const char *p)
		{
			miku::TextNode t;
			t.nextNode = miku::NODE_TYPE_TEXT;

			char *q = reinterpret_cast<char *>(&t);
			for (int i=0; i<sizeof(miku::TextNode); i++)
				m_buf.push_back(*q++);

			m_mystr.Add( p );	// �e�L�X�g�o�^
		}
	};

	// pass2��Offset���m�肳����
	class pass2 : public xmlPerse {
		myStrbank& m_mystr;
		hashmap_t& m_hashmap;
	public:
		pass2(std::vector<char>& buf, myStrbank& mystr, XmlEnum* xenum, hashmap_t& hashmap)
		 : xmlPerse(buf, xenum), m_mystr(mystr), m_hashmap(hashmap) {}

		virtual ~pass2(){}

		void Elements(const TiXmlElement* e)
		{
			Element(e);
			m_mystr.Dump();
		}

	private:
		void elementName(const char *p)
		{
			miku::ElementNode *pElement = reinterpret_cast<miku::ElementNode *>(&m_buf[m_ofs]);
			if (m_enum)
				pElement->name = m_enum->Element(p);
			else
				pElement->name = m_mystr.Get(p);
		}

		void attrProc(const TiXmlAttribute *attr)
		{
			miku::Attr *pAttr = reinterpret_cast<miku::Attr *>(&m_buf[m_ofs]);
			if (m_enum){
				pAttr->name = m_enum->Attribute(attr->Name());
				switch (m_enum->Type(pAttr->name)){
				case XmlEnum::STRING:
					pAttr->value = m_mystr.Get(attr->Value());
					break;
				case XmlEnum::FLOAT:
					*reinterpret_cast<float *>(&pAttr->value) = (float)atof(attr->Value());
					break;
				case XmlEnum::CRC32:
					pAttr->value = getCRC32(attr->Value());
					m_hashmap.insert(std::pair<unsigned int, std::string>(pAttr->value, attr->Value()));
					break;
				default:
					pAttr->value = atoi(attr->Value());
				}
			} else {
				pAttr->name = m_mystr.Get(attr->Name());
				pAttr->value = m_mystr.Get(attr->Value());
			}
		}

		void textProc(const char *p)
		{
			miku::TextNode *pText = reinterpret_cast<miku::TextNode *>(&m_buf[m_ofs]);
			pText->entity = m_mystr.Get(p);
		}
	};

	// �߂��i�e�j�m�[�h���Z�b�g����
	void returnSet(char *pBuf, int idx, int ret)
	{
		do {
			miku::Node *pNode = reinterpret_cast<miku::Node *>(pBuf+idx);
			if (pNode->getNextNode()==0){
				pNode->nextNode |= ret | miku::NODE_TERMINATE;
			}
			if (pNode->isElementType()){
				miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);
				if (pElement->getChildNode()){
					returnSet(pBuf, pElement->getChildNode(), idx);
				}
			}
			idx = pNode->getBrotherNode();
		} while(idx);
	}

}

int XmlBin::Conv(std::vector<char>& outbuf, hashmap_t& hashmap, const TiXmlDocument& xml, XmlEnum* xenum)
{
	outbuf.clear();
	myStrbank mystr(outbuf);
	pass1(outbuf, mystr, xenum).Elements(xml.RootElement());
	int nodeSize = outbuf.size();
	pass2(outbuf, mystr, xenum, hashmap).Elements(xml.RootElement());
	returnSet(&outbuf[0], 0, 0);
	return nodeSize;
}

void mikuPrint(char *pBuf, XmlEnum& xmls, IMikuPrint& print)
{
	int idx = 0;
	int indent = 0;
	do {
		miku::Node *pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
		if (pNode->isElementType()){
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);
			print.TagName(xmls.ElementName(pElement->name), indent);

			miku::Attr *pAttr = reinterpret_cast<miku::Attr *>(pElement + 1);
			for (int i = 0; i<pElement->getAttrNum(); i++, pAttr++){
				print.AttrName(xmls.AttributeName(pAttr->name));
			}
			if (pElement->getChildNode()){
				print.TagClose(xmls.ElementName(pElement->name), indent);
				idx = pElement->getChildNode();
				indent++;
				continue;
			}
			else {
				print.EmptyElementTagClose(xmls.ElementName(pElement->name), indent);
			}
		}
		else {
			miku::TextNode *pText = static_cast<miku::TextNode *>(pNode);
			print.Text(&pBuf[pText->entity], indent);
		}
		while (pNode->isTerminate()){
			// �����Ȃ���΂P�K�w�̃G�������g�ɖ߂�
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
			indent--;
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);
			print.EndTag(xmls.ElementName(pElement->name), indent);

			//			if (indent==0)	break;	// root�܂ŏオ���Ă����̂ŏI���Breturn�ł��ǂ�
			if (idx == 0)	break;	// root�܂ŏオ���Ă����̂ŏI���Breturn�ł��ǂ�
		}
		idx = pNode->getBrotherNode();
	} while (indent);	// �K�w���Ȃ��ꍇ�݂̂����Ŕ�����B
}
