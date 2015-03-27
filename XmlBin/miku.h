/*! @file miku.h */
/*!
	XML�o�C�i���[�t�H�[�}�b�g�@miku
 */
#pragma once


//! XML�o�C�i���[�t�H�[�}�b�g
namespace miku
{
	typedef int Offset;

	enum {
		NODE_TYPE_ELEMENT,
		NODE_TYPE_TEXT = 0x40000000,	// �e�L�X�g
		NODE_TERMINATE = 0x80000000		// ��m�[�h�Ȃ�
	};

	//! miku�\���̂̋��ʕ�
	struct Node {
		Offset nextNode;	//!< ����24�r�b�g�F���̃m�[�h�i��m�[�h�j�A�܂��͖߂��m�[�h�̈ʒu
							//!< ���8�r�b�g�F�^�C�v

		bool isElementType() const { return (nextNode & NODE_TYPE_TEXT)==0; }
		bool isTextType() const { return (nextNode & NODE_TYPE_TEXT)!=0; }
		bool hasNextNode() const { return (nextNode & NODE_TERMINATE)==0; }
		bool isTerminate() const { return (nextNode & NODE_TERMINATE)!=0; }
		int getNextNode() const { return nextNode & 0xffffff; }
		int getBrotherNode() const { return isTerminate()? 0 : getNextNode(); }
	};

	//! ElementNode�ɑ�����nAttr�����\����
	struct Attr {
		Offset name;	//!< �����̖��O�̈ʒu�A���邢�͑�����enum
		union {
			Offset value;	//!< �����̒l�̈ʒu�A���邢�͒l���̂���
			float fvalue;
		};
	};

	//! �^�C�v���v�f�̏ꍇ 
	struct ElementNode : public Node {
		Offset childNode;	//!< ����24�r�b�g�F�ŏ��̎q�m�[�h�̈ʒu
							//!< ���8�r�b�g�F����(Attribute)�̐�
		Offset name;		//!< �v�f�̖��O�̈ʒu�A���邢�͗v�f��enum

		int getChildNode() const { return childNode & 0xffffff; }
		int getAttrNum() const { return childNode >> 24; }
		const Attr* getAttrPtr() const { return reinterpret_cast<const miku::Attr *>(this+1); }
	};

	//! �^�C�v���e�L�X�g�̏ꍇ
	struct TextNode : public Node {
		Offset entity;		//!< �e�L�X�g�̈ʒu
	};

}
