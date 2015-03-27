/*! @file XmlEnum.h */
#pragma once

#include <vector>
#include <string>
#include <map>
#include "tinyxml/tinyxml.h"

/*!
 * XML�t�@�C���̗v�f���Ƒ��������X�L�[�}�t�@�C���ɏ]����enum�̃C���f�b�N�X�ɕϊ����܂��B
 * �o�^����ĂȂ��v�f���⑮�����͒ǉ��o�^���܂��B
 */
class XmlEnum {
	struct AttrSet {
		std::string	name;	//!< ����
		int			type;	//!< �^�C�v
	};

	std::vector<std::string> m_element;
	std::vector<AttrSet> m_attribute;
public:
	enum {
		STRING,	//!< ������
		INT,	//!< �S�o�C�g����
		SHORT,	//!< �Q�o�C�g����
		BYTE,	//!< �P�o�C�g����
		FLOAT,	//!< �S�o�C�g���������_
		CRC32,	//!< CRC32�n�b�V���l
	};
	/// �X�L�[�}��ǂݍ��݂܂�
	void ReadSchema(const TiXmlElement* e);

	/// enum�̒�`��W���o�͂ɏo�͂��܂�
	/*!
	 @param[in] name name��namespace���Ƃ��ďo�͂��܂��B
	*/
	void PrintEnum(const char *name);

	/// �v�f����enum�l�i�C���f�b�N�X�j��Ԃ��A�o�^����ĂȂ��ꍇ�͓o�^���܂�
	/*!
	 @param[in] p �v�f��
	 @return enum�l
	*/
	int Element(const char *p);

	/// ��������o�^���A�C���f�b�N�X��Ԃ��܂�
	/*!
	 @param[in] p ������
	 @return enum�l
	*/
	int Attribute(const char *p);

	/// �C���f�b�N�X����^�C�v�����߂܂��B
	int Type(int idx) const { return m_attribute[idx].type; }

	/// �C���f�b�N�X����v�f�������߂܂�
	const char *ElementName(int idx) const { return m_element[idx].c_str(); }

	/// �C���f�b�N�X���瑮���������߂܂�
	const char *AttributeName(int idx) const { return m_attribute[idx].name.c_str(); }
};
