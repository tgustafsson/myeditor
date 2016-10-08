#include "AttributedString.h"
#include <algorithm>
#include "Debug.h"

using namespace std;

AttributedString::AttributedString(shared_ptr<wstring> s)
{
	m_valid_syntax_coloring = false;
	m_content = s;
	m_color.resize(s->length());
	for (size_t i = 0; i < m_color.size(); i++)
	{
		m_color[i] = color::NORMAL;
	}
}

AttributedString::AttributedString(const std::wstring& s)
{
	m_valid_syntax_coloring = false;
	m_content = make_shared<wstring>(s);
	m_color.resize(s.length());
	for (size_t i = 0; i < m_color.size(); i++)
	{
		m_color[i] = color::NORMAL;
	}
}

AttributedString::AttributedString(const AttributedString& as)
{
	m_valid_syntax_coloring = as.m_valid_syntax_coloring;
	this->m_content = as.m_content;
	this->m_color = as.m_color;
}

const wstring& AttributedString::to_str()
{
	return *m_content;
}

tuple<wchar_t&, AttributedString::color&> AttributedString::operator[](size_t index)
{
	return make_tuple(ref(m_content->at(index)), ref(m_color[index]));
}

void AttributedString::invalidate_syntax_coloring()
{
	m_valid_syntax_coloring = false;
}

size_t AttributedString::size() const
{
	return m_content->length();
}

AttributedString::iterator AttributedString::begin()
{
	return m_content->begin();
}

AttributedString::iterator AttributedString::end()
{
	return m_content->end();
}

size_t AttributedString::length() const
{
	return size();
}

wchar_t AttributedString::at(size_t pos) const
{
	return m_content->at(pos);
}

void AttributedString::append(size_t pos, wchar_t wc)
{
	m_content->append(pos, wc);
	m_color.resize(m_content->size());
	m_valid_syntax_coloring = false;
}

void AttributedString::append(const std::wstring& ws)
{
	m_content->insert(m_content->length(), ws);
	m_color.resize(m_content->size());
	m_valid_syntax_coloring = false;
}

void AttributedString::insert(iterator pos, wchar_t wc)
{
	m_content->insert(pos, wc);
	m_color.resize(m_content->size());
	m_valid_syntax_coloring = false;
}

void AttributedString::insert(iterator pos, wstring::iterator begin, wstring::iterator end)
{
	m_content->insert(pos, begin, end);
	m_color.resize(m_content->length());
	m_valid_syntax_coloring = false;
}

void AttributedString::insert(iterator pos, std::wstring::const_iterator begin, std::wstring::const_iterator end)
{
	m_content->insert(pos, begin, end);
	m_color.resize(m_content->length());
	m_valid_syntax_coloring = false;
}

bool AttributedString::valid_syntax_coloring() const
{
	return m_valid_syntax_coloring;
}

void AttributedString::set_valid_syntax_coloring()
{
	m_valid_syntax_coloring = true;
}

void AttributedString::erase(size_t off, size_t count)
{
	m_content->erase(off, count);
	m_color.resize(m_content->size());
	m_valid_syntax_coloring = false;
}

wstring AttributedString::substr(size_t off, size_t len) const
{
	return m_content->substr(off, len);
}

AttributedString& AttributedString::operator=(const wstring& rhs)
{
	*(this->m_content) = rhs;
	m_color.resize(m_content->length());
	m_valid_syntax_coloring = false;
	return *this;
}

AttributedString& AttributedString::operator=(const AttributedString& rhs)
{
	m_content = rhs.m_content;
	m_color = rhs.m_color;
	m_valid_syntax_coloring = rhs.m_valid_syntax_coloring;
	return *this;
}