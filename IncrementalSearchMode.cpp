#include "IncrementalSearchMode.h"
#include "Control.h"
#include "Debug.h"
#include <tuple>

using namespace std;

IncrementalSearchMode::IncrementalSearchMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert) : Mode(keys, control, insert) {
}

void incremental_search_background(shared_ptr<Control> control, vector<shared_ptr<AttributedString>> rows, const wstring& search, AttributedString::color active_color, AttributedString::color matched_color) {
   intptr_t row, col;
   control->get_cursor_pos(row, col, Control::REAL);
   intptr_t vrow = 0;
   for ( auto r : rows )
   {
      size_t pos = 0;
      while ( pos != wstring::npos )
      {
         auto s_to_s = r->to_str();
         pos = s_to_s.find(search, pos);
         if ( pos != wstring::npos )
         {
            auto color = matched_color;
            if ( (vrow == row) && (col == static_cast<intptr_t>(pos)) ) color = active_color;
            for ( size_t i = 0; i < search.length(); i++ )
            {
               auto temp = (*r)[pos];
               get<1>(temp) = color;
               pos++;
            }
            r->invalidate_syntax_coloring();
         }
      }
      vrow++;
   }
}

vector<shared_ptr<AttributedString>> IncrementalSearchMode::syntax_highlight(vector<shared_ptr<AttributedString>> rows) {
   _debug << "IncrementalSearchMode::syntax_highlight. rows.size() = " << rows.size() << "\n";
   _debug << "m_search = " << m_search << "\n";
   if ( m_search != L"" )
   {
      incremental_search_background(m_control, rows, m_search, AttributedString::color::HILIGHT, AttributedString::color::INC_SEARCH);
   }
   return rows;
}

void IncrementalSearchMode::set_search(wstring search) {
   m_search = search;
}

wstring IncrementalSearchMode::get_search() const {
   return m_search;
}
