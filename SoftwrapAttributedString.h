#pragma once

#include "AttributedString.h"

class SoftwrapAttributedString : public AttributedString
{
private:
	size_t m_width;
	size_t m_offset;
	std::wstring m_string;
	std::shared_ptr<AttributedString> m_as;
public:
	SoftwrapAttributedString(std::shared_ptr<AttributedString> s, size_t offset, size_t width);
	virtual const std::wstring& to_str();
	virtual std::tuple<wchar_t&, color&> operator[](size_t index);
	virtual size_t size() const;
	virtual size_t length() const;
	virtual AttributedString::iterator begin();
	virtual AttributedString::iterator end();
	virtual wchar_t at(size_t pos) const;
	virtual void append(size_t pos, wchar_t wc);
	virtual void append(const std::wstring& ws);
	virtual void insert(AttributedString::iterator pos, wchar_t wc);
	virtual void insert(AttributedString::iterator pos, std::wstring::iterator begin, std::wstring::iterator end);
	virtual void erase(size_t off, size_t count);
	virtual std::wstring substr(size_t off = 0, size_t len = std::string::npos) const;
};