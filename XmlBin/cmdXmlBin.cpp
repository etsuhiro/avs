#include "XmlBin.h"

#include <cstdio>
#include <vector>

#include "common/cFileWrite.h"
#include "common/Strbank.h"
#include "tinyxml/tinyxml.h"
#include "miku.h"
#include "XmlEnum/XmlEnum.h"

bool debugMode = false;
bool epackMode = false;
bool bigEndian = false;
XmlEnum xmls;

void mikuPrint(char *pBuf, int idx, int indent)
{
#if 0
	do {
		miku::Node *pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
		if (pNode->type==0){
			miku::ElementNode *pElement = reinterpret_cast<miku::ElementNode *>(pNode);

			for (int i=0; i<indent; i++)
				printf("  ");
			if (epackMode)
				printf("<%s", xmls.ElementName(pElement->name));
			else
				printf("<%s(%d)", &pBuf[pElement->name], pElement->name);
			miku::Attr *pAttr = reinterpret_cast<miku::Attr *>(pElement+1);
			for (int i=0; i<pElement->nAttr; i++, pAttr++){
				if (epackMode){
					printf(" %s=\"", xmls.AttributeName(pAttr->name));
					switch(xmls.Type(pAttr->name)){
					case XmlEnum::STRING:
						printf("%s\"", &pBuf[pAttr->value]);
						break;
					case XmlEnum::FLOAT:
						printf("%f\"", *reinterpret_cast<float *>(&pAttr->value));
						break;
					default:
						printf("%d\"", pAttr->value);
					}
				} else
					printf(" %s=\"%s\"", &pBuf[pAttr->name], &pBuf[pAttr->value]);
			}
			if (pElement->childNode){
				printf(">\n");
				mikuPrint(pBuf, pElement->childNode, indent+1);
				for (int i=0; i<indent; i++)
					printf("  ");
				printf("</%s>\n", &pBuf[pElement->name]);
			} else {
				printf("/>\n");
			}
		} else {
			miku::TextNode *pText = reinterpret_cast<miku::TextNode *>(pNode);

			for (int i=0; i<indent; i++)
				printf("  ");
			printf("%s\n", &pBuf[pText->entity]);
		}
		idx = pNode->nextNode;
	} while (idx);
#else
	do {
		miku::Node *pNode = reinterpret_cast<miku::Node *>(&pBuf[idx]);
		if (pNode->isElementType()){
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);

			for (int i=0; i<indent; i++)
				printf("  ");
			printf("<%s", &pBuf[pElement->name]);
			miku::Attr *pAttr = reinterpret_cast<miku::Attr *>(pElement+1);
			for (int i=0; i<pElement->getAttrNum(); i++, pAttr++){
				printf(" %s=\"%s\"", &pBuf[pAttr->name], &pBuf[pAttr->value]);
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
			printf("</%s>\n", &pBuf[pElement->name]);

//			if (indent==0)	break;	// rootまで上がってきたので終了。returnでも良い
			if (idx==0)	break;	// rootまで上がってきたので終了。returnでも良い
		}
		idx = pNode->getBrotherNode();
	} while (indent);	// 階層がない場合のみここで抜ける。
#endif
}

// デバッグ用ダンプ
void mikuDump(char *pBuf, int idx, int ret)
{
	do {
		miku::Node *pNode = reinterpret_cast<miku::Node *>(pBuf+idx);

		printf("[%d]\n", idx);
		printf("nextNode: %d", pNode->getNextNode());
		if (pNode->isTerminate())	printf("[return]\n");
		else						printf("\n");
//		printf("returnNode: %d\n", pNode->returnNode);
		if (pNode->isElementType()){
			miku::ElementNode *pElement = static_cast<miku::ElementNode *>(pNode);
			printf("childNode: %d\n", pElement->getChildNode());
			printf("<%s>\n", &pBuf[pElement->name]);
			if (pElement->getChildNode()){
				mikuDump(pBuf, pElement->getChildNode(), idx);
			}
		} else {
			miku::TextNode *pText = static_cast<miku::TextNode *>(pNode);
			printf("%s\n", &pBuf[pText->entity]);
		}
		idx = pNode->getBrotherNode();
	} while(idx);
}

void usage(void)
{
	fprintf(stderr, "Usage: xmlbin xmlfile [-d] [-b] [-e schemafile] [-o outfile]\n");
	exit(0);
}

int main(int argc, char *argv[])
{
	const char *outFile = 0;

	if (argc==1)	usage();

	TiXmlDocument xml;
	if (xml.LoadFile(argv[1])){
		for (int i=2; i<argc; i++){
			if (argv[i][0] == '-'){
				switch(argv[i][1]){
				case 'e':
					if (++i>=argc)	usage();
					{
						TiXmlDocument schema;
						if (schema.LoadFile(argv[i])){
							epackMode = true;
							xmls.ReadSchema(schema.RootElement());
//							xmls.Print(argv[i]);
						}
					}
					break;
				case 'd':
					debugMode = true;
					break;
				case 'b':
					bigEndian = true;
					break;
				case 'o':
					if (++i>=argc)	usage();
					outFile = argv[i];
					break;
				default:
					usage();
				}
			} else
				usage();
		}

		std::vector<char> bxbuf;
		hashmap_t	hashmap;
		int nodeSize = XmlBin().Conv(bxbuf, hashmap, xml, (epackMode)? &xmls:0 );

		if (debugMode){
			mikuPrint(&bxbuf[0], 0, 0);
			mikuDump(&bxbuf[0], 0, 0);
		} else {
			if (bigEndian){
				for (int i=0; i<nodeSize; i+=4){
					char ex[4];
					for (int j=0; j<4; j++){
						ex[3-j] = bxbuf[i+j];
					}
					for (int j=0; j<4; j++){
						bxbuf[i+j] = ex[j];
					}
				}
			}
			if (outFile==0){
				std::string outfile(argv[1]);
				std::string::size_type pos = outfile.find_last_of('.');
				if (pos!=std::string::npos){
					outfile.erase(pos);	// 拡張子を消す
				}
				outfile += ".bin";
				FileWrite(outfile.c_str()).write(&bxbuf[0], bxbuf.size());
			} else {
				FileWrite(outFile).write(&bxbuf[0], bxbuf.size());
			}
		}
	} else {
		fprintf(stderr, "file read error %s\n", argv[1]);
	}
}
