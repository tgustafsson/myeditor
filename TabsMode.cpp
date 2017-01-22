#include "TabsMode.h"
#include "Control.h"
#include "Debug.h"
#include <memory>
#include <cassert>

using namespace std;

TabsMode::TabsMode(const KeyCords& keys, shared_ptr<Control> control, KeyCord::insert_func insert): Mode(keys, control, insert), m_tabsize(4)
{
}

vector<shared_ptr<AttributedString>> TabsMode::syntax_highlight(vector<shared_ptr<AttributedString>> rows) {
   intptr_t row_nr, col;
   intptr_t new_col = 0;
   m_control->get_cursor_pos(row_nr, col, Control::change_t::REAL);
   intptr_t view_row, view_col;
   m_control->get_view(view_row, view_col);
   vector<shared_ptr<AttributedString>> ret;
   intptr_t r = 0;
   for (auto row : rows)
   {
      shared_ptr<AttributedString> rcopy;
      intptr_t i = 0, j = 0;
      bool copy = false;
      while ( i < static_cast<intptr_t>(row->length()) )
      {
         auto c = (*row)[i];
         if ( get<0>(c) == '\t' )
         {
            if ( !copy )
            {
               rcopy = row->deep_copy();
               copy = true;
            }
            auto what_is_j = (*rcopy)[j];
            assert(get<0>(what_is_j) == '\t');
            rcopy->erase(j, 1);
            for ( intptr_t length = m_tabsize - (j % m_tabsize); length > 0; length-- )
            {
               if ( (row_nr - view_row) == r )
               {
                  if ( col > i)
                  {
                     new_col++;
                  }
               }
               auto b = rcopy->begin();
               rcopy->insert(b + j++, ' ');
               auto np = (*rcopy)[j - 1];
               get<1>(np) = AttributedString::color::TAB;
            }
         }
         else
         {
            j++;
         }
         i++;
      }
      if ( copy )
      {
         ret.push_back(rcopy);
      } else
      {
         ret.push_back(row);
      }
      r++; 
   }
   if ( new_col > 0 )
   {
      m_delta_cursor_col = new_col - 1;
   }
   else
   {
      m_delta_cursor_col = 0;
   }
   return ret;
}

intptr_t TabsMode::get_delta_col() const {
   return m_delta_cursor_col;
}

void TabsMode::set_tabsize(char tabsize) {
   m_tabsize = tabsize;
}

char TabsMode::get_tabsize() const {
   return m_tabsize;
}
