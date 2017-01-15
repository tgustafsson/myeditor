#include "SoftwrapAttributedString.h"
#include <algorithm>
#include "Debug.h"

using namespace std;

SoftwrapAttributedString::SoftwrapAttributedString(std::shared_ptr<AttributedString> s, size_t offset, size_t width) : AttributedString(*s) {
   m_as = s;
   m_string = L"";
   m_offset = offset;
   m_width = width;
   _debug << "SoftwrapAttributedString: content " << *(this->m_content) << ", offset " << m_offset << " m_width" << width << "\n";
}

const std::wstring& SoftwrapAttributedString::to_str() {
   m_string = m_content->substr(m_offset, m_width);
   return static_cast<const wstring&>(m_string);
}

std::tuple<const wchar_t&, AttributedString::color&> SoftwrapAttributedString::operator[](size_t index)
{
   return AttributedString::operator[](index + m_offset);
}

size_t SoftwrapAttributedString::size() const {
   return min(m_content->length() - m_offset, m_width);
}

size_t SoftwrapAttributedString::length() const {
   return size();
}

AttributedString::const_iterator SoftwrapAttributedString::begin() {
   return AttributedString::begin() + m_offset;
}

AttributedString::const_iterator SoftwrapAttributedString::end() {
   if ( m_offset + m_width > m_content->length() ) return m_content->end();
   else return m_content->begin() + m_offset + m_width;
}

wchar_t SoftwrapAttributedString::at(size_t pos) const {
   return AttributedString::at(pos + m_offset);
}

void SoftwrapAttributedString::append(size_t pos, wchar_t wc) {
   m_as->append(pos + m_offset, wc);
   m_as->invalidate_syntax_coloring();
}

void SoftwrapAttributedString::append(const std::wstring& ws) {
   m_as->insert(m_as->begin() + min(m_width, size()), ws.begin(), ws.end()); //  (m_as->begin() + min(m_width, size()), ws.begin(), ws.end());
   m_as->invalidate_syntax_coloring();
}

void SoftwrapAttributedString::insert(AttributedString::const_iterator pos, wchar_t wc) {
   m_as->insert(pos, wc);
}

void SoftwrapAttributedString::insert(AttributedString::const_iterator pos, std::wstring::iterator begin, std::wstring::iterator end) {
   m_as->insert(pos, begin, end);
}

void SoftwrapAttributedString::erase(size_t off, size_t count) {
   m_as->erase(off + m_offset, count);
}

std::wstring SoftwrapAttributedString::substr(size_t off, size_t len) const {
   return m_as->substr(off + m_offset, len);
}
