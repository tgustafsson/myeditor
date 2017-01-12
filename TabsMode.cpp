#include "TabsMode.h"
#include "Control.h"
#include "Debug.h"

using namespace std;

TabsMode::TabsMode(const KeyCords& keys, shared_ptr<Control> control, KeyCord::insert_func insert) : Mode(keys, control, insert) {
}

vector<shared_ptr<AttributedString>> TabsMode::syntax_highlight(vector<shared_ptr<AttributedString>> rows) {
   _debug << "TabsMode\n";
   intptr_t row_nr, col;
   m_control->get_cursor_pos(row_nr, col, Control::change_t::REAL);
   vector<shared_ptr<AttributedString>> ret;
   size_t r = 0;
   for (auto row : rows)
   {
      decltype(row) rcopy;
      size_t i = 0, j = 0;
      bool copy = false;
      while ( i < row->length() )
      {
         auto c = (*row)[i];
         if ( get<0>(c) == '\t' )
         {
            if ( !copy )
            {
               _debug << "Create copy\n";
               rcopy = make_shared<decltype(row)::element_type>(*row);
               copy = true;
            }
            _debug << "Erase " << get<0>((*rcopy)[j])<< "\n";
            rcopy->erase(j, 1);
            while ( ((j + 1) % 4) != 0 )
            {
               if ( row_nr == r )
               {
                  if ( j > col )
                  {
                     col++;
                  }
               }
               auto b = rcopy->begin();
               _debug << "insert at " << j << "\n";
               rcopy->insert(b + j++, ' ');
               auto np = (*rcopy)[j - 1];
               get<1>(np) = AttributedString::color::TAB;
            }
//          _debug << "TabsMode row: " << row->to_str() << "\n";
            _debug << "TabsMode rcopy: |" << rcopy->to_str()<< "|\n";
         }
         i++;
         j++;
         r++;
      }
      if ( copy )
      {
         ret.push_back(rcopy);
      } else
      {
         ret.push_back(row);
      }
   }
   m_control->change_cursor(row_nr, col, Control::change_t::REAL);
   return ret;
}
