/*! @file miku.h */
/*!
	XMLバイナリーフォーマット　miku
 */
#pragma once


//! XMLバイナリーフォーマット
namespace miku
{
	typedef int Offset;

	enum {
		NODE_TYPE_ELEMENT,
		NODE_TYPE_TEXT = 0x40000000,	// テキスト
		NODE_TERMINATE = 0x80000000		// 弟ノードなし
	};

	//! miku構造体の共通分
	struct Node {
		Offset nextNode;	//!< 下位24ビット：次のノード（弟ノード）、または戻り先ノードの位置
							//!< 上位8ビット：タイプ

		bool isElementType() const { return (nextNode & NODE_TYPE_TEXT)==0; }
		bool isTextType() const { return (nextNode & NODE_TYPE_TEXT)!=0; }
		bool hasNextNode() const { return (nextNode & NODE_TERMINATE)==0; }
		bool isTerminate() const { return (nextNode & NODE_TERMINATE)!=0; }
		int getNextNode() const { return nextNode & 0xffffff; }
		int getBrotherNode() const { return isTerminate()? 0 : getNextNode(); }
	};

	//! ElementNodeに続いてnAttr個続く構造体
	struct Attr {
		Offset name;	//!< 属性の名前の位置、あるいは属性のenum
		union {
			Offset value;	//!< 属性の値の位置、あるいは値そのもの
			float fvalue;
		};
	};

	//! タイプが要素の場合 
	struct ElementNode : public Node {
		Offset childNode;	//!< 下位24ビット：最初の子ノードの位置
							//!< 上位8ビット：属性(Attribute)の数
		Offset name;		//!< 要素の名前の位置、あるいは要素のenum

		int getChildNode() const { return childNode & 0xffffff; }
		int getAttrNum() const { return childNode >> 24; }
		const Attr* getAttrPtr() const { return reinterpret_cast<const miku::Attr *>(this+1); }
	};

	//! タイプがテキストの場合
	struct TextNode : public Node {
		Offset entity;		//!< テキストの位置
	};

}
