/*! @file XmlEnum.h */
#pragma once

#include <vector>
#include <string>
#include <map>
#include "tinyxml/tinyxml.h"

/*!
 * XMLファイルの要素名と属性名をスキーマファイルに従ってenumのインデックスに変換します。
 * 登録されてない要素名や属性名は追加登録します。
 */
class XmlEnum {
	struct AttrSet {
		std::string	name;	//!< 名称
		int			type;	//!< タイプ
	};

	std::vector<std::string> m_element;
	std::vector<AttrSet> m_attribute;
public:
	enum {
		STRING,	//!< 文字列
		INT,	//!< ４バイト整数
		SHORT,	//!< ２バイト整数
		BYTE,	//!< １バイト整数
		FLOAT,	//!< ４バイト浮動小数点
		CRC32,	//!< CRC32ハッシュ値
	};
	/// スキーマを読み込みます
	void ReadSchema(const TiXmlElement* e);

	/// enumの定義を標準出力に出力します
	/*!
	 @param[in] name nameをnamespace名として出力します。
	*/
	void PrintEnum(const char *name);

	/// 要素名のenum値（インデックス）を返し、登録されてない場合は登録します
	/*!
	 @param[in] p 要素名
	 @return enum値
	*/
	int Element(const char *p);

	/// 属性名を登録し、インデックスを返します
	/*!
	 @param[in] p 属性名
	 @return enum値
	*/
	int Attribute(const char *p);

	/// インデックスからタイプを求めます。
	int Type(int idx) const { return m_attribute[idx].type; }

	/// インデックスから要素名を求めます
	const char *ElementName(int idx) const { return m_element[idx].c_str(); }

	/// インデックスから属性名を求めます
	const char *AttributeName(int idx) const { return m_attribute[idx].name.c_str(); }
};
