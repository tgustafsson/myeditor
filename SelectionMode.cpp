#include "SelectionMode.h"
#include "Selection.h"
#include <algorithm>
#include "Control.h"
#include "Debug.h"

using namespace std;

SelectionMode::SelectionMode(const KeyCords& keys, std::shared_ptr<Control> control, shared_ptr<Model> model, shared_ptr<View> view, KeyCord::insert_func insert) :
   Mode(keys, control, insert), m_model(model), m_view(view) {
}

void selection_background(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control, shared_ptr<Selection> selection, vector<shared_ptr<AttributedString>> rows, AttributedString::color active_background, AttributedString::color inactive_background) {
   intptr_t width, height, srow, scol;
   control->get_view(srow, scol, Control::REAL);
   view->get_win_prop(width, height); 
   Selection::selection_t first;
   Selection::selection_t second;
   if ( selection->has_end() )
   {
      if ( selection->get_start(Control::change_t::REAL, control) <= selection->get_end(Control::change_t::REAL, control) )
      {
         first = selection->get_start(Control::change_t::REAL, control);
         second = selection->get_end(Control::change_t::REAL, control);
      } else
      {
         first = selection->get_end(Control::change_t::REAL, control);
         second = selection->get_start(Control::change_t::REAL, control);
      }
   } else
   {
      Selection::selection_t cursor = make_tuple(control->get_row_no(Control::REAL), control->get_col(Control::REAL), Control::change_t::REAL);
      if ( selection->get_start(Control::change_t::REAL, control) <= cursor )
      {
         first = selection->get_start(Control::change_t::REAL, control);
         second = cursor;
      } else
      {
         first = cursor;
         second = selection->get_start(Control::change_t::REAL, control);
      }
   }
   size_t i = 0;
   for (; i < rows.size(); i++ )
   {
      for ( size_t j = 0; j < rows[i]->length(); j++ )
      {
         Selection::selection_t pos = make_tuple(i + srow, j, Control::change_t::REAL);
         auto temp = (*rows[i])[j];
         if ( first <= pos && pos < second )
         {
            get<1>(temp) = active_background;
            rows[i]->invalidate_syntax_coloring();
         } else if ( get<1>(temp) == active_background )
         {
            get<1>(temp) = inactive_background;
            rows[i]->invalidate_syntax_coloring(); 
         }
      }
   }
}

vector<std::shared_ptr<AttributedString>> SelectionMode::syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows) {
   auto selections = m_model->get_selections();
   if ( selections.size() > 0 )
   {
      for ( auto selection : selections )
      {
         selection_background(m_model, m_view, m_control, selection, rows, AttributedString::color::SELECTION, AttributedString::color::NORMAL);
      }
   }
   return rows;
}
