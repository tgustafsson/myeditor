#include "Model.h"
#include <fstream>
#include <string>
#ifdef _MSC_VER
#include <codecvt>
#endif
#include <locale>
#include "Selection.h"
#include "Debug.h"

#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;

string wstring_to_string(const wstring& ws) {
   string ret;
   for ( auto c : ws )
   {
      ret += static_cast<char>(c);
   }
   return ret;
}

void Model::load() {
   if ( boost::filesystem::exists(boost::filesystem::path(m_path)) )
   {
   #ifndef _MSC_VER
      string temp = wstring_to_string(m_path);
   #else
      wstring temp = m_path;
   #endif
      std::wifstream wif(temp, ios_base::openmode::_S_bin);
   #ifdef _MSC_VER
      wif.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
      m_encoding = encoding::UTF8;
   #endif
      m_content.clear();
      while ( !wif.eof() && !wif.fail() )
      {
         auto line = make_shared<wstring>();
         wchar_t curr = 0;
         while ( !wif.eof() && !wif.eof() && curr != '\n' )
         {
            wif.read(&curr, 1);
            line->append(&curr);
         }

         //getline(wif, *line);
         auto astring = make_shared<AttributedString>(line);
         m_content.push_back(astring);
      }
      auto fail = wif.fail();
      auto bad = wif.bad();
      auto eof = wif.eof();
      auto good = wif.good();
      auto rdstate = wif.rdstate();
      if ( eof && !fail )
      {
         m_content.clear();
         std::wifstream wif2(temp);
         m_encoding = encoding::ASCII;
         while ( !wif2.eof() && !wif2.fail() )
         {
            auto line = make_shared<wstring>();
            getline(wif2, *line);
            auto astring = make_shared<AttributedString>(line);
            m_content.push_back(astring);
         }
         wif2.close();
      }
      wif.close();
   }
}

vector<shared_ptr<AttributedString>> Model::rows(intptr_t start, intptr_t stop) {
   if ( start == -1 )
   {
      start = 0;
   }
   if ( stop == -1 )
   {
      stop = m_content.size();
   }
   vector<shared_ptr<AttributedString>> ret;
   for ( intptr_t i = start; i < stop && i < m_content.size(); i++ )
   {
      ret.push_back(m_content[i]);
   }
   return ret;
}

size_t Model::number_of_lines() const {
   return m_content.size();
}

shared_ptr<AttributedString> Model::get_row(size_t row) {
   while ( m_content.size() <= row ) m_content.push_back(make_shared<AttributedString>(L""));
   return m_content[row];
}

void Model::insert(size_t row, const wstring& content) {
   while ( m_content.size() < row )
   {
      m_content.push_back(make_shared<AttributedString>(L""));
   }
   auto start = m_content.begin();
   m_content.insert(start + row, make_shared<AttributedString>(content));
}

void Model::erase(size_t row) {
   m_content.erase(m_content.begin() + row);
}

void Model::save() {
#ifndef _MSC_VER
   string temp = wstring_to_string(m_path);
#else
   wstring temp = m_path;
#endif
   std::wofstream wof(temp);
#ifdef _MSC_VER
   if ( m_encoding == encoding::UTF8 ) wof.imbue(locale(locale::empty(), new codecvt_utf8<wchar_t>));
#endif
   for ( auto r : m_content )
   {
      wof << r->to_str() << endl;
   }
   wof.flush();
   wof.close();
}

void Model::load(wstring path) {
   m_path = path;
   load();
}

Model::SearchPos Model::search(intptr_t row, intptr_t col, intptr_t view_row, intptr_t view_col, Model::SearchDirection dir, std::wstring search_term) {
   intptr_t i = row + view_row;
   intptr_t c = col + view_col;
   size_t j = 0;
   while ( j < number_of_lines() )
   {
      auto cs = get_row(i);
      auto find_pos = cs->to_str().find(search_term, c);
      if ( find_pos != wstring::npos )
      {
         Model::SearchPos sp;
         sp.delta_row = i - (row + view_row);
         sp.delta_col = find_pos - (col + view_col);
         sp.found = true;
         return sp;
      }
      i = (i + 1) % number_of_lines();
      c = 0;
      j++;
   }
   Model::SearchPos sp;
   sp.delta_row = 0;
   sp.delta_col = 0;
   sp.found = false;
   return sp;
}

void Model::set_start_selection(shared_ptr<Selection> s) {
   m_selections.push_back(s);
}

const vector<shared_ptr<Selection> >& Model::get_selections() {
   return m_selections;
}

void Model::remove_selections() {
   m_selections.clear();
}

std::wstring Model::get_path() const {
   return m_path;
}

std::wstring Model::get_filename() const {
   namespace fs = boost::filesystem;
   boost::filesystem::path p = fs::canonical(fs::path(m_path));
   return p.parent_path().wstring();
}

std::wstring Model::get_extension() const {
   namespace fs = boost::filesystem;
   fs::path p = fs::path(m_path);
   auto ret = p.extension().wstring(); 
   return ret;
}
