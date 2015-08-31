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
			// �����Ȃ���΂P�K�w�̃G�������g�ɖ߂�
			idx = pNode->getNextNode();
			pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
			indent--;
			//			if (indent==0)	break;	// root�܂ŏオ���Ă����̂ŏI���Breturn�ł��ǂ�
			if (idx == 0)	break;	// root�܂ŏオ���Ă����̂ŏI���Breturn�ł��ǂ�
		}
		idx = pNode->getBrotherNode();
	} while (indent);	// �K�w���Ȃ��ꍇ�݂̂����Ŕ�����B
}
