#include "Utilities.h"
#include "Macro.h"
#include "Cords.h"
#include "LatexMode.h"
#include "CppMode.h"
#include "FundamentalMode.h"
#include "Debug.h"
#include "TabsMode.h"
#include <type_traits>

using namespace std;

vector<intptr_t> get_real_rows_being_visual(shared_ptr<View> view, shared_ptr<Control> control)
{
   intptr_t width, height;
   view->get_win_prop(width, height); 
   intptr_t view_row, view_col;
   control->get_view(view_row, view_col); 
   intptr_t row_copy_, col_copy_;
   control->get_cursor_pos(row_copy_, col_copy_, Control::change_t::REAL); 
   vector<intptr_t> real_rows_being_visual;
   for ( int i = 0; i < height; i++ )
   {
      control->change_cursor(i, 0, Control::change_t::VISUAL);
      intptr_t crow = control->get_row_no(Control::change_t::REAL);
      if ( real_rows_being_visual.size() > 0 )
      {
         if ( real_rows_being_visual[real_rows_being_visual.size() - 1] < crow )
         {
            real_rows_being_visual.push_back(crow);
         }
      } else
      {
         real_rows_being_visual.push_back(crow);
      }
   }
   control->change_cursor(row_copy_, col_copy_, Control::REAL); 
   return real_rows_being_visual;
}

void update_view(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   shared_ptr<TabsMode> tabsmode = nullptr; 
   try
   {
      shared_ptr<Mode> t = control->get_mode(Control::TABS_MODE);
      tabsmode = dynamic_pointer_cast<TabsMode>(t); 
   }
   catch ( exception e )
   {

   }
   
   intptr_t width, height;
   view->get_win_prop(width, height);
   intptr_t view_row, view_col;
   control->get_view(view_row, view_col);

   auto real_rows_being_visual = get_real_rows_being_visual(view, control);

   auto _rows = control->rows(Control::REAL, real_rows_being_visual[0], real_rows_being_visual[real_rows_being_visual.size() - 1]);
   for ( auto current_mode : control->get_modes() )
   {
      _rows = current_mode->syntax_highlight(_rows);
   }
   if ( real_rows_being_visual.size() > 0 )
   {
      control->insert_visual_rows(_rows, real_rows_being_visual[0]);
   }
   control->wrap_content();
   _rows = control->rows(Control::VISUAL, view_row, view_row + height);
   view->update(_rows, view_col);
   intptr_t row, col;
   control->get_cursor_pos(row, col, Control::REAL);
   intptr_t row_copy = row, col_copy = col, view_row_copy = view_row, view_col_copy = view_col;
   intptr_t deltacol = 0;
   intptr_t new_current_row, new_current_col;
   if ( tabsmode != nullptr )
   {
      intptr_t current_visual_row, current_visual_col;
      control->get_cursor_pos(current_visual_row, current_visual_col, Control::VISUAL);
      deltacol = tabsmode->get_delta_col();
      control->change_cursor(row, col + deltacol, Control::REAL);
      control->get_cursor_pos(new_current_row, new_current_col, Control::VISUAL);
      control->change_cursor(current_visual_row, current_visual_col, Control::VISUAL);
   } else
   {
      control->get_cursor_pos(new_current_row, new_current_col, Control::VISUAL);
   }
   view->change_visual_cursor(new_current_row, new_current_col);
   view->refresh();
}

void loop(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   while ( control->get_execute() )
   {
      intptr_t view_row, view_col;
      control->get_view(view_row, view_col); 
      intptr_t row, col;
      control->get_cursor_pos(row, col, Control::REAL);
      intptr_t row_copy = row, col_copy = col, view_row_copy = view_row, view_col_copy = view_col; 
      update_view(model, view, control);
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
   if ( extension == L".cpp" || extension == L".cc" || extension == L".h" || extension == L".hh" )
   {
      control->add_mode(Control::MAJOR_MODE, make_shared<CppMode>(_main_cords, control, &my_insert));
   } else if ( extension == L".tex" )
   {
      control->add_mode(Control::MAJOR_MODE, make_shared<LatexMode>(_main_cords, control, &my_insert));
   } else
   {
      control->add_mode(Control::MAJOR_MODE, make_shared<FundamentalMode>(_main_cords, control, &my_insert));
   }
}
