#include "SoftwrapControl.h"
#include "Macro.h"
#include "Debug.h"
#include <limits>

using namespace std;

SoftwrapControl::SoftwrapControl(std::shared_ptr<Model> m, std::shared_ptr<View> v)
   : CursesControl(m) {
   m_visual_row = 0;
   m_visual_col = 0;
   m_visual_view_row = 0;
   intptr_t temp1, temp2;
   v->get_win_prop(temp1, temp2);
   m_width = temp1;
   m_width -= 2;
   m_height = temp2;
   m_view_row_add = 0;
   wrap_content();
}

//void SoftwrapControl::change(intptr_t delta_row, Control::change_t row_change, intptr_t delta_col, Control::change_t col_change, shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
//   intptr_t row, col, width, height, view_row, view_col;
//   get_cursor_pos(row, col, row_change);
//   view->get_win_prop(width, height);
//   get_view(view_row, view_col, col_change);
//   intptr_t new_row = row + delta_row;
//   intptr_t new_col = col + delta_col;
//   if ( new_row < 0 )
//   {
//      view_row += new_row;
//      row = 0;
//      if ( view_row < 0 )
//      {
//         view_row = 0;
//      }
//   } else
//   {
//      if ( new_row >= height )
//      {
//         view_row += new_row - height + 1;
//         row = height - 1;
//      } else
//      {
//         row = new_row;
//      }
//   }
//   if ( new_col < 0 )
//   {
//      view_col += new_col;
//      if ( view_col < 0 )
//      {
//         change(-1, row_change, 0, col_change, model, view, control);
//         my_end_line(model, view, control);
//         return;
//      }
//      col = 0;
//   } else
//   {
//      if ( new_col >= width )
//      {
//         change_cursor(row, width, row_change);
//         view_col += new_col - width;
//         change_view(view_row, view_col, model->number_of_lines(), col_change);
//         return;
//      } else
//      {
//         col = new_col;
//      }
//   }
//   change_cursor(row, col, row_change);
//   change_view(view_row, view_col, model->number_of_lines(), col_change);
//}

shared_ptr<AttributedString> SoftwrapControl::get_row(change_t t, intptr_t delta) {
   if ( t == Control::REAL ) return Control::get_row(t, delta);
   else if ( t == Control::VISUAL )
   {
      return m_softwrap[m_visual_row + m_visual_view_row];
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

vector<intptr_t> SoftwrapControl::get_real_rows_being_visual(shared_ptr<View> view)
{
   vector<intptr_t> ret;
   intptr_t width, height;
   view->get_win_prop(width, height);
   for ( intptr_t i = m_visual_view_row; i < m_visual_view_row + height && m_softwrap.size(); i++ )
   {
      intptr_t crow = get<0>(m_map_soft_to_real[i + m_visual_view_row]) - (m_view_row + m_view_row_add); 
      if ( ret.size() > 0 )
      {
         if ( ret[ret.size() - 1] < crow )
         {
            ret.push_back(crow);
         }
      }
      else
      {
         ret.push_back(crow);
      }
   }
   return ret;
}

void SoftwrapControl::change_cursor(intptr_t row, intptr_t col, change_t t) {
   if ( t == Control::REAL )
   {
      m_real_row = row;
      m_real_col = col;
      m_visual_row = m_map_real_to_soft[row + (m_view_row + m_view_row_add)] + (m_view_col + col) / m_width - m_visual_view_row;
      m_visual_col = col % m_width;
   } else if ( t == Control::VISUAL )
   {
      if ( col >= static_cast<intptr_t>(m_width) ) col = m_width - 1;
      m_real_row = get<0>(m_map_soft_to_real[row + m_visual_view_row]) - (m_view_row + m_view_row_add);
      m_real_col = get<1>(m_map_soft_to_real[row + m_visual_view_row]) * m_width + col;
      m_visual_row = row;
      m_visual_col = col;
   }
}

void SoftwrapControl::change_view(intptr_t row, intptr_t col, size_t number_of_lines, Control::change_t t) {
   if ( t == Control::change_t::REAL )
   {
      if ( col >= 0 )
      {
         m_real_col += col;
         m_visual_row = m_map_real_to_soft[m_real_row + (m_view_row + m_view_row_add)] + (m_view_col + m_real_col) / m_width - m_visual_view_row;
         m_visual_col = m_real_col % m_width;
      }
      if ( row >= 0 && row < static_cast<intptr_t>(number_of_lines) 
          && (m_view_row + m_view_row_add) != row)
      {
//       m_view_row = get<0>(m_map_soft_to_real[m_visual_view_row]);
//       m_view_row_add = get<1>(m_map_soft_to_real[m_visual_view_row]);
         m_view_row = row;
         m_view_row_add = 0;
         m_visual_view_row = m_map_real_to_soft[row + (m_view_row + m_view_row_add)];
      }
      return;
   }
   else if ( t == Control::change_t::VISUAL )
   {
      m_visual_view_row = row;
      m_view_col = col; 
      if ( m_map_soft_to_real.find(m_visual_view_row) != m_map_soft_to_real.end() )
      {
         m_view_row = get<0>(m_map_soft_to_real[m_visual_view_row]); 
         m_view_row_add = get<1>(m_map_soft_to_real[m_visual_view_row]); 
      }
      else
      {
         m_view_row = 0;
      }
      return;

   }
   throw (exception());
}

void SoftwrapControl::get_view(intptr_t& row, intptr_t& col, Control::change_t t) {
   if ( t == Control::change_t::REAL )
   {
      row = m_view_row + m_view_row_add;
      col = m_view_col;
      return;
   }
   else if ( t == Control::change_t::VISUAL )
   {
      row = m_visual_view_row;
      col = 0;
      return;
   }
   throw (exception());
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
      return m_real_row +  m_view_row + m_view_row_add;
   } else if ( t == Control::VISUAL )
   {
      return m_visual_row + m_visual_view_row;
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

