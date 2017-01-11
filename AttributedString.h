#pragma once

#include <string>
#include <vector>
#include <tuple>
#include <memory>

class AttributedString
{
public:
   enum color
   {
      STRING,
      TRIPLE_QUOTE_STRING,
      LONG_COMMENT,
      LINE_COMMENT,
      NORMAL,
      KEYWORD,
      DATATYPE,
      CERROR,
      INC_SEARCH,
      HILIGHT
   };
protected:
   std::shared_ptr<std::wstring> m_content;
   std::vector<color> m_color;
   bool m_valid_syntax_coloring;
   wchar_t m_line_feed;
public:
   typedef std::wstring::iterator iterator;
   typedef std::wstring::const_iterator const_iterator;
   AttributedString(std::shared_ptr<std::wstring> s);
   AttributedString(const std::wstring& s);
   AttributedString(const AttributedString& as);
   virtual const std::wstring& to_str();
   virtual std::tuple<const wchar_t&, color&> operator [](size_t index);
   void invalidate_syntax_coloring();
   bool valid_syntax_coloring() const;
   void set_valid_syntax_coloring();
   virtual size_t size() const;
   virtual size_t length() const;
   virtual const_iterator begin();
   virtual const_iterator end();
   virtual wchar_t at(size_t pos) const;
   virtual void append(size_t pos, wchar_t wc);
   virtual void append(const std::wstring& ws);
   virtual void insert(const_iterator pos, wchar_t wc);
   virtual void insert(const_iterator pos, std::wstring::iterator begin, std::wstring::iterator end);
   virtual void insert(const_iterator pos, std::wstring::const_iterator begin, std::wstring::const_iterator end);
   virtual void erase(size_t off, size_t count);
   virtual std::wstring substr(size_t off = 0, size_t len = std::string::npos) const;
   AttributedString& operator=(const std::wstring& rhs);
   AttributedString& operator=(const AttributedString& rhs);
};
