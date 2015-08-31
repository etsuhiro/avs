#include "AvsVariable.h"
#include "common/crc32.h"
#include "../sinkenum.h"
#include "XmlBin/miku.h"

using namespace avs;

void AvsVariable::Set(const char* name, int val)
{
	unsigned int hash = getCRC32(name);
	std::map<unsigned int, int>::iterator p = m_var.find(hash);
	if (p != m_var.end()){
		p->second = val;
	}
}

int AvsVariable::Get(const char* name)
{
	unsigned int hash = getCRC32(name);
	std::map<unsigned int, int>::iterator p = m_var.find(hash);
	return (p != m_var.end()) ? p->second : 0;
}

void AvsVariable::Init(char *pBuf)
{
	int idx = 0;
	int indent = 0;
	do {
		miku::Node *pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
		if (pNode->isElementType()){
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);
			if (pElement->name == sink::element::SET){
				const miku::Attr *pAttr = pElement->getAttrPtr();
				for (int i = 0; i<pElement->getAttrNum(); i++, pAttr++){
					if (pAttr->name == sink::attribute::var){
						unsigned int hash = getCRC32(&pBuf[pAttr->value]);
						m_var[hash] = 0;
						break;	// for
					}
				}
			}
			if (pElement->getChildNode()){
				idx = pElement->getChildNode();
				indent++;
				continue;	// do
			}
		}
		while (pNode->isTerminate()){
			// Ÿ‚ª‚È‚¯‚ê‚Î‚PŠK‘w‚ÌƒGƒŒƒƒ“ƒg‚É–ß‚é
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
			indent--;
			//			if (indent==0)	break;	// root‚Ü‚Åã‚ª‚Á‚Ä‚«‚½‚Ì‚ÅI—¹Breturn‚Å‚à—Ç‚¢
			if (idx == 0)	break;	// root‚Ü‚Åã‚ª‚Á‚Ä‚«‚½‚Ì‚ÅI—¹Breturn‚Å‚à—Ç‚¢
		}
		idx = pNode->getBrotherNode();
	} while (indent);	// ŠK‘w‚ª‚È‚¢ê‡‚Ì‚İ‚±‚±‚Å”²‚¯‚éB
}
