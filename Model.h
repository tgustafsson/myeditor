#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string>
#include <memory>
#include "AttributedString.h"

class Selection;

class Model
{
public:
   enum encoding
   {
      UTF8,
      ASCII
   };
   struct SearchPos {
      intptr_t delta_row, delta_col;
      bool found;
   };
   enum SearchDirection
   {
      FORWARD,
      BACKWARD
   };
protected:
   std::vector<std::shared_ptr<AttributedString>> m_content;
   std::wstring m_path;
   encoding m_encoding;
   std::vector<std::shared_ptr<Selection>> m_selections;
protected:
   virtual void load();
public:
   Model() : m_path(L""), m_encoding(encoding::UTF8) {
   }
   Model(std::wstring path) : m_path(path), m_encoding(encoding::UTF8)
   {
      load();
   }
   std::wstring get_path() const;
   std::wstring get_filename() const;
   std::wstring get_extension() const;
   virtual std::vector<std::shared_ptr<AttributedString>> rows(intptr_t start = -1, intptr_t stop = -1);
   virtual size_t number_of_lines() const;
   virtual std::shared_ptr<AttributedString> get_row(size_t row);
   virtual void insert(size_t row, const std::wstring& content);
   virtual void erase(size_t row);
   virtual void save();
   virtual void load(std::wstring path);
   virtual SearchPos search(intptr_t row, intptr_t col, intptr_t view_row, intptr_t view_col, SearchDirection dir, std::wstring search_term);
   virtual void set_start_selection(std::shared_ptr<Selection> s);
   virtual const std::vector<std::shared_ptr<Selection> >& get_selections();
   virtual void remove_selections();
};

#endif
