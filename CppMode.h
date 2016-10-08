#pragma once
#include "Mode.h"
#include <string>
#include <regex>

class CppMode : public Mode
{
	/*		STRING,
		TRIPLE_QUOTE_STRING,
		LONG_COMMENT,
		LINE_COMMENT,
		NORMAL,
		KEYWORD,
		DATATYPE,
		CERROR*/
private:
	std::wregex m_keywords;
	std::wregex m_line_comment;
	std::wregex m_long_comment;
	std::wregex m_datatype;
	std::wregex m_string;
public:
	CppMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert);
	virtual std::vector<std::shared_ptr<AttributedString>> syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows);
};