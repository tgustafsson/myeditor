#include "Utilities.h"
#include "Macro.h"

using namespace std;


void loop(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   while ( control->get_execute() )
   {
      intptr_t width, height;
      view->get_win_prop(width, height);
      intptr_t view_row, view_col;
      control->get_view(view_row, view_col);
      auto _rows = control->rows(Control::VISUAL, view_row, view_row + height);
      for ( auto current_mode : control->get_modes() )
      {
         _rows = current_mode->syntax_highlight(_rows);
      }
      view->update(_rows, view_col);
      intptr_t row, col;
      control->get_cursor_pos(row, col, Control::REAL);
      intptr_t row_copy = row, col_copy = col, view_row_copy = view_row, view_col_copy = view_col;
      control->get_cursor_pos(row, col, Control::VISUAL); 
      view->change_visual_cursor(row, col);
      view->refresh();
      auto func = control->get_modes()[0]->get_key();
      auto hist = bind(func, model, view, control);
      auto undo = func(model, view, control);
      control->wrap_content();
      auto f = get<0>(undo);
      if ( get<1>(undo) ) control->get_command_history().add_command(hist, bind(f, model, view, control, row_copy, col_copy, view_row_copy, view_col_copy));
      if ( _macro.is_recording() ) _macro.add_function(hist);
   }
}
