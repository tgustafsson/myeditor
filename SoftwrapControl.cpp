#include "SoftwrapControl.h"
#include "Macro.h"
#include "Debug.h"
#include <limits>

using namespace std;

SoftwrapControl::SoftwrapControl(std::shared_ptr<Model> m, std::shared_ptr<View> v)
   : CursesControl(m) {
   m_visual_row = 0;
   m_visual_col = 0;
   intptr_t temp1, temp2;
   v->get_win_prop(temp1, temp2);
   m_width = temp1;
   m_width -= 2;
   wrap_content();
}

shared_ptr<AttributedString> SoftwrapControl::get_row(change_t t, intptr_t delta) {
   if ( t == Control::REAL ) return Control::get_row(t, delta);
   else if ( t == Control::VISUAL )
   {
      return m_softwrap[m_visual_row + m_view_row];
   }
   return make_shared<AttributedString>(wstring());
}

void SoftwrapControl::wrap_content() {
   size_t i = 0;
   size_t total = 0;
   m_softwrap.clear();
   m_map_soft_to_real.clear();
   m_map_real_to_soft.clear();
   auto _rows = rows(Control::WITH_INSERTED_VISUAL_LINES, 0, numeric_limits<intptr_t>::max());
   for (auto c : _rows)
   {
      size_t j = 0;
      do
      {
         m_map_soft_to_real[total] = make_tuple(i, j);
         if ( m_map_real_to_soft.find(i) == m_map_real_to_soft.end() ) m_map_real_to_soft[i] = total;
         m_softwrap.push_back(make_shared<SoftwrapAttributedString>(c, j * m_width, m_width));
         j++;
         total++;
      }
      while ( j * m_width < c->length() );
      i++;
   }
}

void SoftwrapControl::change_cursor(intptr_t row, intptr_t col, change_t t) {
   if ( t == Control::REAL )
   {
      m_real_row = row;
      m_real_col = col;
      m_visual_row = m_map_real_to_soft[row] + (m_view_col + col) / m_width;
      m_visual_col = col % m_width;
   } else if ( t == Control::VISUAL )
   {
      if ( col >= static_cast<intptr_t>(m_width) ) col = m_width - 1;
      m_real_row = get<0>(m_map_soft_to_real[row]);
      m_real_col = get<1>(m_map_soft_to_real[row]) * m_width + col;
      m_visual_row = row;
      m_visual_col = col;
   }
}

void SoftwrapControl::change_view(intptr_t row, intptr_t col, size_t number_of_lines) {
   if ( col >= 0 )
   {
      m_real_col += col;
      m_visual_row = m_map_real_to_soft[m_real_row] + (m_view_col + m_real_col) / m_width;
      m_visual_col = m_real_col % m_width;
   }
   if ( row >= 0 && row < static_cast<intptr_t>(number_of_lines) )
   {
      m_view_row = row;
   }

}

void SoftwrapControl::get_cursor_pos(intptr_t& row, intptr_t& col, change_t t) {
   if ( t == Control::REAL )
   {
      row = m_real_row;
      col = m_real_col;
   } else if ( t == Control::VISUAL )
   {
      row = m_visual_row;
      col = m_visual_col;
   }
}

size_t SoftwrapControl::get_row_no(Control::change_t t) {
   if ( t == Control::REAL )
   {
      return Control::get_row_no(t);
   } else if ( t == Control::VISUAL )
   {
      return m_visual_row + m_view_row;
   }
   throw(exception());
}

size_t SoftwrapControl::get_col(Control::change_t t) {
   if ( t == Control::REAL )
   {
      return Control::get_col(t);
   } else if ( t == Control::VISUAL )
   {
      return m_visual_col;
   }
   throw (exception());
}

vector<shared_ptr<AttributedString>> SoftwrapControl::rows(Control::change_t t, intptr_t start_row, intptr_t end_row) {
   if ( t == Control::REAL )
   {
      auto ret = Control::rows(t, start_row, end_row); 
      return ret;
   } else if ( t == Control::VISUAL )
   {
      vector<shared_ptr<AttributedString>> ret;
      for ( intptr_t i = start_row; i <= end_row && i < static_cast<intptr_t>(m_softwrap.size()); i++ )
      {
         ret.push_back(m_softwrap[i]);
      }
      return ret;
   }
   else if ( t == Control::WITH_INSERTED_VISUAL_LINES )
   {
      auto ret = Control::rows(t, start_row, end_row); 
      return ret;
   }
   throw (exception());
}

