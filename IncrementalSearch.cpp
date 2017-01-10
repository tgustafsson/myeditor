#include "IncrementalSearch.h"
#include "IncrementalSearchMode.h"
#include "Utilities.h"

using namespace std;

KeyCord::command_return_t IncrementalSearch::my_incremental_search(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control, std::shared_ptr<Control> incremental_search_control, std::shared_ptr<Model> incremental_search_model, std::shared_ptr<View> incremental_search_view) {
   intptr_t row, col, width, height, view_row, view_col;
   control->get_cursor_pos(row, col, Control::REAL);
   view->get_win_prop(width, height);
   control->get_view(view_row, view_col);
   m_start_row = row;
   m_start_col = col;
   m_start_view_row = view_row;
   m_start_view_col = view_col;
   m_view = view;
   while ( m_run_loop )
   {
      loop(incremental_search_model, incremental_search_view, incremental_search_control);
      //incremental_search_control->loop();
      if ( !m_quitting )
      {
         auto search_string = incremental_search_model->get_row(0)->to_str();
         intptr_t row, col, width, height, view_row, view_col;
         control->change(m_delta_row, Control::REAL, m_delta_col, Control::REAL, model, view, control);
         control->get_cursor_pos(row, col, Control::REAL);
         view->get_win_prop(width, height);
         control->get_view(view_row, view_col);
         auto delta_pos = model->search(row, col, view_row, view_col, m_dir, search_string);
         if ( !delta_pos.found && m_wrap )
         {
            if ( m_dir == Model::SearchDirection::FORWARD ) delta_pos = model->search(0, 0, 0, 0, m_dir, search_string);
            else if ( m_dir == Model::SearchDirection::BACKWARD )
            {

               //delta_pos = model->search(0, , m_model->number_of_lines(), 0, m_dir, search_string);
            }
            if ( delta_pos.found )
            {
               delta_pos.delta_row -= row;
               delta_pos.delta_col -= col;
               delta_pos.found = true;
            }
         }
         control->change(delta_pos.delta_row, Control::REAL, delta_pos.delta_col, Control::REAL, model, view, control);
         m_delta_row = 0;
         m_delta_col = 0;
         control->get_view(view_row, view_col);
         control->get_cursor_pos(row, col, Control::VISUAL);
         vector<shared_ptr<AttributedString>> _rows;
         if ( m_start_row <= row && row <= m_start_row + height ) _rows = control->rows(Control::change_t::VISUAL, m_start_view_row, m_start_view_row + height);
         else
         {
            //control->change_view(row - height / 2, view_col, height);
            _rows = control->rows(Control::change_t::VISUAL, row, row + height);
         }
         IncrementalSearchMode& ism = dynamic_cast<IncrementalSearchMode&>(*(control->get_modes()[1]));
         ism.set_search(search_string); 
         for (auto current_mode : control->get_modes() )
         {
            _rows = current_mode->syntax_highlight(_rows);
         }
         view->update(_rows, view_col);
      }
      incremental_search_control->set_execute();

   }
   m_run_loop = true;
   m_quitting = false;
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t IncrementalSearch::my_incremental_search_insert(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control, wchar_t wc) {
   shared_ptr<AttributedString> s = control->get_row(Control::REAL, 0);
   intptr_t start = control->get_col(Control::REAL);
   if ( start >= 0 )
   {
      while ( s->length() < start ) s->append(1, L' ');
      auto it = s->begin();
      s->insert(it + start, wc);
      control->change(0, Control::REAL, 1, Control::REAL, model, view, control);
      control->exit();
   }
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t IncrementalSearch::my_incremental_search_enter(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   control->exit();
   m_run_loop = false;
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t IncrementalSearch::my_incremental_search_again(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   intptr_t row, col;
   m_delta_col = 1;
   control->exit();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t IncrementalSearch::my_incremental_search_quit(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   control->change_cursor(m_start_row, m_start_col, Control::REAL);
   control->change_view(m_start_view_row, m_start_view_col, model->number_of_lines());
   m_quitting = true;
   m_run_loop = false;
   control->exit();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t IncrementalSearch::my_incremental_search_backspace(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   return make_tuple(&my_empty_undo, false);
}
