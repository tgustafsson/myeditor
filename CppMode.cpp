#include "CppMode.h"
#include <regex>

using namespace std;

CppMode::CppMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert) : Mode(keys, control, insert)
, m_keywords{L"\\b(if|while|do|for|void|else|struct|class|enum|virtual)\\b"}
, m_line_comment{L"(//.*)"}
, m_long_comment{L"(/\\*|\\*/)"}
, m_datatype{L"\\b(int|short|long|char|wchar_t|size_t|intptr_t)\\b"}
, m_string{L"(\\\".*\\\")"}
{
}

std::vector<std::shared_ptr<AttributedString>> CppMode::syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows)
{
	vector<shared_ptr<AttributedString>> ret;
	for (auto r : rows)
	{
		shared_ptr<AttributedString> s = r;
		if (r->size() > 0 && !r->valid_syntax_coloring())
		{
			auto len = r->size();
			for (size_t i = 0; i < len; i++)
			{
				auto temp = (*r)[i];
				get<1>(temp) = AttributedString::color::NORMAL;
			}
			auto wsi = wsregex_iterator(r->begin(), r->end(), m_keywords);
			auto wsi_end = wsregex_iterator();
			for (auto it = wsi; it != wsi_end; it++)
			{
				auto match = *it;
				for (size_t j = match.position(); j < match.position() + match.str().length(); j++)
				{
					auto temp = (*s)[j];
					get<1>(temp) = AttributedString::color::KEYWORD;
				}
			}
			wsi = wsregex_iterator(r->begin(), r->end(), m_string);
			wsi_end = wsregex_iterator();
			for (auto it = wsi; it != wsi_end; it++)
			{
				auto match = *it;
				for (size_t j = match.position(); j < match.position() + match.str().length(); j++)
				{
					auto temp = (*s)[j];
					get<1>(temp) = AttributedString::color::STRING;
				}
			}
			wsi = wsregex_iterator(r->begin(), r->end(), m_line_comment);
			wsi_end = wsregex_iterator();
			for (auto it = wsi; it != wsi_end; it++)
			{
				auto match = *it;
				for (size_t j = match.position(); j < match.position() + match.str().length(); j++)
				{
					auto temp = (*s)[j];
					get<1>(temp) = AttributedString::color::LINE_COMMENT;
				}
			}
			wsi = wsregex_iterator(r->begin(), r->end(), m_datatype);
			wsi_end = wsregex_iterator();
			for (auto it = wsi; it != wsi_end; it++)
			{
				auto match = *it;
				for (size_t j = match.position(); j < match.position() + match.str().length(); j++)
				{
					auto temp = (*s)[j];
					get<1>(temp) = AttributedString::color::DATATYPE;
				}
			}
			//wsi = wsregex_iterator(r->begin(), r->end(), m_long_comment);
			//wsi_end = wsregex_iterator();
			//for (auto it = wsi; it != wsi_end; it++)
			//{
			//	auto match = *it;
			//	if (match.str() == L"/*")
			//	{
			//		color_long_comment()
			//	}
			//}
			r->set_valid_syntax_coloring();
		}
		ret.push_back(s);
	}
	return ret;
}