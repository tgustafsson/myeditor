#include "Utilities.h"
#include "Macro.h"
#include "Cords.h"
#include "LatexMode.h"
#include "CppMode.h"
#include "FundamentalMode.h"
#include "Debug.h"

using namespace std;


void loop(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   while ( control->get_execute() )
   {
      intptr_t width, height;
      view->get_win_prop(width, height);
      intptr_t view_row, view_col;
      control->get_view(view_row, view_col);
      intptr_t row_copy_, col_copy_;
      control->get_cursor_pos(row_copy_, col_copy_, Control::change_t::REAL);
      vector<intptr_t> real_rows_being_visual;
      for ( int i = 0; i < height; i++)
      {
         control->change_cursor(i, 0, Control::change_t::VISUAL);
         intptr_t crow = control->get_row_no(Control::change_t::REAL);
         if ( real_rows_being_visual.size() > 0 )
         {
            if ( real_rows_being_visual[real_rows_being_visual.size() - 1] != crow )
            {
               real_rows_being_visual.push_back(crow);
               _debug << "loop: inserting into real_rows_being_visual: " << crow << "\n";
            }
         }
         else
         {
            real_rows_being_visual.push_back(crow);
            _debug << "loop: inserting first into real_rows_being_visual: " << crow << "\n";
         }
      }
      auto _rows = control->rows(Control::REAL, real_rows_being_visual[0], real_rows_being_visual[real_rows_being_visual.size() - 1]);
      for ( auto current_mode : control->get_modes() )
      {
         _rows = current_mode->syntax_highlight(_rows);
      }
      if ( real_rows_being_visual.size() > 0 )
      {
         _debug << "loop: use insert_visual_rows\n";
         control->insert_visual_rows(_rows, real_rows_being_visual[0]); 
      }
      control->wrap_content();
      _debug << "loop: get visual rows\n";
      _rows = control->rows(Control::VISUAL, view_row, view_row + height);
      control->change_cursor(row_copy_, col_copy_, Control::REAL);
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

void assign_mode_based_on_extension(shared_ptr<Model> model, shared_ptr<Control> control)
{
   auto extension = model->get_extension();
   _debug << "assign_mode_based_on_extension: " << extension << "\n";
   if ( control->get_modes().size() == 0 )
   {
      control->get_modes().resize(1);
   }
   if ( extension == L".cpp" || extension == L".cc" || extension == L".h" )
   {
      control->get_modes()[0] = make_shared<CppMode>(_main_cords, control, &my_insert);
   } else if ( extension == L".tex" )
   {
      control->get_modes()[0] = make_shared<LatexMode>(_main_cords, control, &my_insert);
   } else
   {
      control->get_modes()[0] = make_shared<FundamentalMode>(_main_cords, control, &my_insert);
   }
}
