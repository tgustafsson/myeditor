#include "Control.h"
#include <vector>
#include <functional>
#include <tuple>
#include <fstream>
#include <limits>
#include "Debug.h"
#include <cstdlib>
#include "IncrementalSearchMode.h"
#include "IncrementalSearch.h"
#include "Clipboard.h"
#include "Macro.h"
#include "Selection.h"
#include "Cords.h"
#include "CppMode.h"
#include "LatexMode.h"
#include "FundamentalMode.h"
#include "Utilities.h"
#define BOOST_FILESYSTEM_NO_DEPRECATED

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;

size_t _number_key_presses = 2;

shared_ptr<AttributedString> Control::get_row(Control::change_t t, intptr_t delta) {
   return m_model->get_row(get_row_no(t) + delta);
}

size_t Control::get_row_no(Control::change_t t) {
   if ( t == Control::REAL ) return m_real_row + m_view_row;
   else if ( t == Control::VISUAL ) return m_real_row + m_view_row;
}

size_t Control::get_col(Control::change_t t) {
   if ( t == Control::REAL ) return m_real_col + m_view_col;
   else if ( t == Control::VISUAL ) return m_real_col + m_view_col;
}

Control::Control(shared_ptr<Model> m) : m_model(m), m_execute(true) {
   m_real_row = 0;
   m_real_col = 0;
   m_view_row = 0;
   m_view_col = 0;
}

Control::~Control() {
}

void Control::wrap_content() {

}

void Control::assign_mode_based_on_extension(shared_ptr<Control> control) {
   auto extension = m_model->get_extension();
   _debug << "assign_mode_based_on_extension: " << extension << "\n";
   if ( m_modes.size() == 0 )
   {
      m_modes.resize(1);
   }
   if ( extension == L".cpp" || extension == L".cc" || extension == L".h" )
   {
      m_modes[0] = make_shared<CppMode>(_main_cords, control, &my_insert);
   } else if ( extension == L".tex" )
   {
      m_modes[0] = make_shared<LatexMode>(_main_cords, control, &my_insert);
   } else
   {
      m_modes[0] = make_shared<FundamentalMode>(_main_cords, control, &my_insert);
   }
}

void Control::add_mode(shared_ptr<Mode> mode) {
   m_modes.push_back(mode);
}

void Control::exit() {
   m_execute = false;
}

void Control::set_execute() {
   m_execute = true;
}

bool Control::get_execute() {
   return m_execute;
}

CommandHistory& Control::get_command_history() {
   return m_commandhistory;
}

void Control::convert_to(Control::change_t from, size_t row, size_t col, Control::change_t to, intptr_t& _row, intptr_t& _col) {
   _row = row;
   _col = col;
}

void Control::change_cursor(intptr_t row, intptr_t col, Control::change_t t) {
   if ( t == Control::REAL )
   {
      m_real_row = row;
      m_real_col = col;
   } else if ( t == Control::VISUAL )
   {
      m_real_row = row;
      m_real_col = col;
   }
}

void Control::change_view(intptr_t row, intptr_t col, size_t number_of_lines) {
   if ( row >= 0 && row < number_of_lines )
   {
      m_view_row = row;
   }
   if ( col >= 0 )
   {
      m_view_col = col;
   }
}

void Control::get_cursor_pos(intptr_t& row, intptr_t& col, Control::change_t t) {
   if ( t == Control::REAL )
   {
      row = m_real_row;
      col = m_real_col;
   } else if ( t == Control::VISUAL )
   {
      row = m_real_row;
      col = m_real_col;
   }
}

void Control::get_view(intptr_t& row, intptr_t& col) {
   row = m_view_row;
   col = m_view_col;
}

vector<shared_ptr<AttributedString>> Control::rows(change_t t, size_t start_row, size_t end_row) {
   return m_model->rows(start_row, end_row);
}

void Control::change(intptr_t delta_row, Control::change_t row_change, intptr_t delta_col, Control::change_t col_change, shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   intptr_t row, col, width, height, view_row, view_col;
   get_cursor_pos(row, col, row_change);
   view->get_win_prop(width, height);
   get_view(view_row, view_col);
   intptr_t new_row = row + delta_row;
   intptr_t new_col = col + delta_col;
   if ( new_row < 0 )
   {
      view_row += new_row;
      row = 0;
      if ( view_row < 0 )
      {
         view_row = 0;
      }
   } else
   {
      if ( new_row >= height )
      {
         view_row += new_row - height + 1;
         row = height - 1;
      } else
      {
         row = new_row;
      }
   }
   if ( new_col < 0 )
   {
      view_col += new_col;
      if ( view_col < 0 )
      {
         change(-1, row_change, 0, col_change, model, view, control);
         my_end_line(model, view, control);
         return;
      }
      col = 0;
   } else
   {
      if ( new_col >= width )
      {
         change_cursor(row, width, row_change);
         view_col += new_col - width;
         change_view(view_row, view_col, model->number_of_lines());
         return;
      } else
      {
         col = new_col;
      }
   }
   change_cursor(row, col, row_change);
   change_view(view_row, view_col, model->number_of_lines());
}

vector<shared_ptr<Mode>>& Control::get_modes() {
   return m_modes;
}

KeyCord::command_return_t my_exit(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   control->exit();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_left(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   control->change(0, Control::change_t::VISUAL, -1, Control::change_t::VISUAL, model, view, control);
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t my_right(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   auto s = control->get_row(Control::change_t::VISUAL, 0);
   auto c = control->get_col(Control::change_t::VISUAL);
   intptr_t width, height;
   view->get_win_prop(width, height);
   if ( c + 1 >= s->length() || c + 1 >= 76 )
   {
      control->change(1, Control::change_t::VISUAL, 0, Control::change_t::VISUAL, model, view, control);
      my_begin_line(model, view, control);
   } else
   {
      control->change(0, Control::change_t::VISUAL, 1, Control::change_t::VISUAL, model, view, control);
   }
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t down(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   control->change(1, Control::change_t::VISUAL, 0, Control::change_t::VISUAL, model, view, control);
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t up(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   control->change(-1, Control::change_t::VISUAL, 0, Control::change_t::VISUAL, model, view, control);
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t word_right(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   shared_ptr<AttributedString> s = control->get_row(Control::REAL, 0);
   intptr_t start = control->get_col(Control::REAL);
   intptr_t col_change = 0;
   if ( static_cast<size_t>(start) >= s->length() )
   {
      control->change(1, Control::REAL, -start, Control::REAL, model, view, control);
      word_right(model, view, control);
      return make_tuple(&my_position_undo, true);
   }
   while ( static_cast<size_t>(start) < s->length() && isspace(s->at(start)) )
   {
      start++;
      col_change++;
   }
   while ( static_cast<size_t>(start) < s->length() && !isspace(s->at(start)) )
   {
      start++;
      col_change++;
   }
   control->change(0, Control::REAL, col_change, Control::REAL, model, view, control);
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t word_left(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control) {
   shared_ptr<AttributedString> s = control->get_row(Control::REAL, 0);
   intptr_t start = control->get_col(Control::REAL);
   intptr_t col_change = 0;
   if ( static_cast<size_t>(start) >= s->length() )
   {
      start = s->length() - 1;
      col_change = -static_cast<intptr_t>((control->get_col(Control::REAL) - s->length()));
   }
   while ( start >= 0 && isspace(s->at(start)) )
   {
      start--;
      col_change--;
   }
   if ( start <= 0 )
   {
      control->change(-1, Control::REAL, numeric_limits<intptr_t>::min(), Control::REAL, model, view, control);
      word_left(model, view, control);
      return make_tuple(&my_position_undo, true);
   } else
   {
      while ( start >= 0 && !isspace(s->at(start)) )
      {
         start--;
         col_change--;
      }
      if ( start < 0 )
      {
         col_change++;
      }
      control->change(0, Control::REAL, col_change, Control::REAL, model, view, control);
   }
   return make_tuple(&my_position_undo, true);
}

void my_empty_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col) {
}

void my_position_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col) {
   control->change_cursor(row, col, Control::REAL);
   control->change_view(view_row, view_col, model->number_of_lines());
}

void my_insert_character_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col) {
   control->change_cursor(row, col, Control::REAL);
   control->change_view(view_row, view_col, model->number_of_lines());
   my_delete(model, view, control);
}

void my_delete_character_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col, wchar_t wch) {
   if ( wch == '\n' )
   {

   } else
   {
      control->change_cursor(row, col - 1, Control::REAL);
      control->change_view(view_row, view_col, model->number_of_lines());
      my_insert(model, view, control, wch);
   }
}

KeyCord::command_return_t my_empty(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_begin_line(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   shared_ptr<AttributedString> s = control->get_row(Control::VISUAL, 0);
   intptr_t start = control->get_col(Control::VISUAL);
   intptr_t col_change = -start;
   while ( start + col_change < s->length() && isspace(s->at(start + col_change)) )
   {
      col_change++;
   }
   if ( col_change == 0 ) col_change = -start;
   control->change(0, Control::VISUAL, col_change, Control::VISUAL, model, view, control);
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t my_end_line(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   shared_ptr<AttributedString> s = control->get_row(Control::VISUAL, 0);
   size_t start = control->get_col(Control::VISUAL);
   intptr_t col_change = s->length() - start;
   control->change(0, Control::VISUAL, col_change, Control::VISUAL, model, view, control);
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t my_backspace(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   shared_ptr<AttributedString> s = control->get_row(Control::REAL, 0);
   intptr_t start = control->get_col(Control::REAL);
   wchar_t wch;
   if ( start > 0 )
   {
      if ( start <= s->length() && s->length() > 0 )
      {
         wch = s->at(start - 1);
         s->erase(start - 1, 1);
      }
      control->change(0, Control::REAL, -1, Control::REAL, model, view, control);
      auto f = bind(&my_delete_character_undo, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7, wch);
      return make_tuple(f, true);
   } else if ( control->get_row_no(Control::REAL) > 0 )
   {
      auto prev = control->get_row(Control::REAL, -1);
      auto l = prev->length();
      prev->append(s->to_str());
      model->erase(control->get_row_no(Control::REAL));
      control->wrap_content();
      control->change(-1, Control::REAL, l - start, Control::REAL, model, view, control);
   }
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_enter(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   shared_ptr<AttributedString> s = control->get_row(Control::REAL, 0);
   intptr_t start = control->get_col(Control::REAL);
   if ( start <= s->length() )
   {
      auto rest_of_line = s->substr(start);
      s->erase(start, s->length() - start);
      model->insert(control->get_row_no(Control::REAL) + 1, rest_of_line);
      control->wrap_content();
      control->change(1, Control::REAL, -start, Control::REAL, model, view, control);
   }
   return make_tuple(&my_insert_character_undo, true);
}

KeyCord::command_return_t my_delete(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   shared_ptr<AttributedString> s = control->get_row(Control::REAL, 0);
   intptr_t start = control->get_col(Control::REAL);
   if ( start < s->length() )
   {
      wchar_t undo_char = s->at(start);
      auto undo = bind(my_delete_character_undo, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7, undo_char);
      s->erase(start, 1);
      return make_tuple(undo, true);
   } else if ( start == s->length() )
   {
      auto next = control->get_row(Control::REAL, 1);
      auto l = next->length();
      s->append(next->to_str());
      model->erase(control->get_row_no(Control::REAL) + 1);
      control->wrap_content();
      auto undo = bind(my_delete_character_undo, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7, '\n');
      return make_tuple(undo, true);
      //control->change(-1, Control::REAL, l - start, Control::REAL, model, view, control);
   }
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_save(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   model->save();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_file_select_tab(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   namespace fs = boost::filesystem;
   auto path = model->get_row(0)->to_str();
   wstring filename = L"";
   fs::path full_path;
   if ( fs::exists(fs::path(path)) )
   {
      full_path = fs::canonical(fs::path(path));
   } else
   {
      full_path = fs::canonical(fs::path(path).parent_path());
      filename = fs::path(path).filename().generic_wstring();
   }
   if ( true )
   {
      fs::directory_iterator end;
      vector<wstring> possible_filenames;
      fs::directory_iterator it;
      if ( fs::is_directory(full_path) ) it = fs::directory_iterator(full_path);
      else it = fs::directory_iterator(full_path.parent_path());
      for (;
            it != end;
            it++ )
      {
         auto t2 = it->path().filename().generic_wstring();
         if ( t2.find(filename.c_str(), 0) == 0 )
         {
            possible_filenames.push_back(t2);
         }
      }
      intptr_t shortest = numeric_limits<intptr_t>::max();
      wstring sh = L"";
      if ( possible_filenames.size() > 1 )
      {
         for ( intptr_t i = 0; i < possible_filenames.size(); i++ )
         {
            for ( intptr_t j = i + 1; j < possible_filenames.size(); j++ )
            {
               intptr_t k;
               for ( k = 0; k < std::min(possible_filenames[i].length(), possible_filenames[j].length()); k++ )
               {
                  if ( possible_filenames[i][k] != possible_filenames[j][k] ) if ( k < shortest )
                     {
                        shortest = k;
                        sh = possible_filenames[i].substr(0, k);
                     }
               }
            }
         }
      } else if ( possible_filenames.size() == 1 )
      {
         fs::path temp;
         sh = possible_filenames[0];
         if ( fs::is_directory(full_path) ) temp = full_path / sh;
         else temp = full_path.parent_path() / sh;
         possible_filenames.clear();
         if ( fs::is_directory(temp) )
         {
            it = fs::directory_iterator(temp);
            for (; it != end; it++ )
            {
               possible_filenames.push_back(it->path().filename().generic_wstring());
            }
         }
      } else
      {

      }
      if ( fs::is_directory(full_path / sh) )
      {
         full_path = full_path / sh;
         full_path += (fs::path::preferred_separator);
      } else
      {
         full_path = (full_path / sh).parent_path() / sh;
      }
      model->erase(0);
      model->insert(0, full_path.generic_wstring());
      while ( model->number_of_lines() > 1 ) model->erase(1);
      for ( auto m : possible_filenames ) model->insert(1, m);
      control->wrap_content();
      my_end_line(model, view, control);
   } else // check relative cd
   {
      auto filename = full_path.filename().wstring();
      if ( filename == L"\\" ) filename = L"";
   }
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_file_select_enter(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   control->exit();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_open_file(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control, std::shared_ptr<Control> file_select_control, std::shared_ptr<Model> file_select_model, shared_ptr<View> file_select_view) {
   loop(file_select_model, file_select_view, file_select_control);
   //file_select_control->loop();
   auto path = file_select_model->get_row(0)->to_str();
   model->load(path);
   control->wrap_content();
   control->change_cursor(0, 0, Control::REAL);
   control->change_view(0, 0, model->number_of_lines());
   //assign_mode_based_on_extension(control);
   file_select_control->set_execute();
   control->get_command_history().clear();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_insert(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control, wchar_t wc) {
   shared_ptr<AttributedString> s = control->get_row(Control::REAL, 0);
   intptr_t start = control->get_col(Control::REAL);
   while ( s->length() < start ) s->append(1, L' ');
   auto it = s->begin();
   s->insert(it + start, wc);
   control->change(0, Control::REAL, 1, Control::REAL, model, view, control);
   return make_tuple(&my_insert_character_undo, true);
}

KeyCord::command_return_t my_kut_line(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   intptr_t view_row, view_col;
   control->get_view(view_row, view_col);
   static size_t last_key_press = 0;
   shared_ptr<AttributedString> s = control->get_row(Control::REAL, 0);
   if ( control->get_col(Control::REAL) < s->length() )
   {
      auto copy = s->substr(control->get_col(Control::REAL));
      s->erase(control->get_col(Control::REAL), s->length() - control->get_col(Control::REAL));
      if ( _number_key_presses - last_key_press == 1 )
      {
         if ( copy == L"" )
         {
            (*_clipboard.begin()).m_lines.push_back(copy);
         } else
         {
            auto j = (*_clipboard.begin()).m_lines.size();
            (*_clipboard.begin()).m_lines[j - 1] = copy;
         }
      } else
      {
         _clipboard.add_clip(copy);
      }
      if ( copy == L"" && control->get_row_no(Control::REAL) < model->number_of_lines() )
      {
         auto temp = control->get_row(Control::REAL, 1);
         s->append(temp->to_str());
         model->erase(control->get_row_no(Control::REAL) + 1);
         control->wrap_content();
      }
   } else
   {
      if ( view_row < model->number_of_lines() )
      {
         if ( _number_key_presses - last_key_press == 1 )
         {
            (*_clipboard.begin()).m_lines.push_back(L"");
            auto temp = control->get_row(Control::REAL, 1);
            s->append(temp->to_str());
            model->erase(control->get_row_no(Control::REAL) + 1);
            control->wrap_content();
         } else
         {
            _clipboard.add_clip(Clip(L""));
         }
      }
   }
   last_key_press = _number_key_presses;
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_paste_from_clipboard(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   auto start = control->get_row_no(Control::REAL);
   auto orig_col = control->get_col(Control::REAL);
   shared_ptr<AttributedString> s = control->get_row(Control::REAL, 0);
   auto clip = _clipboard.top_clip();
   size_t last_clip_length;
   if ( clip.m_lines.size() > 0 )
   {
      wstring cut;
      while ( control->get_col(Control::REAL) >= s->length() ) s->append(L" ");
      cut = s->substr(control->get_col(Control::REAL));
      s->erase(control->get_col(Control::REAL), s->length() - control->get_col(Control::REAL));
      s->append(clip.m_lines[0]);
      last_clip_length = clip.m_lines[0].length();
      for ( size_t i = 1; i < clip.m_lines.size(); i++ )
      {
         start++;
         model->insert(start, clip.m_lines[i]);
         control->wrap_content();
         orig_col = 0;
         last_clip_length = clip.m_lines[i].length();
      }
      s = model->get_row(start);
      s->insert(s->begin() + orig_col + last_clip_length, cut.begin(), cut.end());
      control->wrap_content();
   }
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_start_selection(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   intptr_t row, col;
   control->get_cursor_pos(row, col, Control::change_t::REAL);
   auto orig_col = control->get_col(Control::change_t::REAL);
   model->remove_selections();
   shared_ptr<Selection> s = make_shared<Selection>(row, col, Control::change_t::REAL);
   model->set_start_selection(s);
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_ctrl_g(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   model->remove_selections();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_copy_selection(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   auto selections = model->get_selections();
   if ( selections.size() == 1 )
   {
      auto selection = selections[0];
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
      Clip clip;
      size_t scol = get<1>(first);
      for ( size_t i = get<0>(first); i <= get<0>(second); i++ )
      {
         auto line = model->get_row(i);
         if ( i == get<0>(second) ) clip.m_lines.push_back(line->substr(scol, get<1>(second) - scol));
         else clip.m_lines.push_back(line->substr(scol));
         scol = 0;
      }
      _clipboard.add_clip(clip);
   } else if ( selections.size() > 1 ) throw SelectionError();
   model->remove_selections();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_cut_selection(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   auto selections = model->get_selections();
   if ( selections.size() == 1 )
   {
      auto selection = selections[0];
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
      Clip clip;
      vector<size_t> row_to_delete;
      intptr_t delta_col = 0;
      for ( size_t i = get<0>(first); i <= get<0>(second); i++ )
      {
         auto line = model->get_row(i);
         if ( i == get<0>(first) && i == get<0>(second) )
         {
            clip.m_lines.push_back(line->substr(get<1>(first), get<1>(second) - get<1>(first)));
            line->erase(get<1>(first), get<1>(second) - get<1>(first));
            delta_col = get<1>(first) - (control->get_col(Control::REAL));
         } else if ( i == get<0>(second) )
         {
            size_t scol = 0;
            clip.m_lines.push_back(line->substr(scol, get<1>(second) - scol));
            line->erase(scol, get<1>(second) - scol);
            delta_col = get<1>(first) - (control->get_col(Control::REAL));
         } else if ( i == get<0>(first) )
         {
            size_t scol = get<1>(first);
            clip.m_lines.push_back(line->substr(scol, line->length() - scol));
            line->erase(scol, line->length() - scol);
         } else
         {
            clip.m_lines.push_back(line->substr());
            row_to_delete.push_back(get<0>(first) + 1);
         }
      }
      intptr_t delta_row = 0;
      for ( auto r : row_to_delete )
      {
         model->erase(r);
         delta_row++;
      }
      if ( get<0>(first) < get<0>(second) )
      {
         auto current_line = model->get_row(get<0>(first));
         auto line = model->get_row(get<0>(first) + 1);
         current_line->append(line->to_str());
         model->erase(get<0>(first) + 1);
         delta_row++;
      }
      control->wrap_content();
      control->change(-delta_row, Control::REAL, delta_col, Control::REAL, model, view, control);
      _clipboard.add_clip(clip);
      auto ftemp = bind(&my_cut_undo, placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4, placeholders::_5, placeholders::_6, placeholders::_7, clip.m_lines, get<0>(first), get<1>(first));
      return make_tuple(ftemp, true);
   } else if ( selections.size() > 1 ) throw SelectionError();
   model->remove_selections();
   return make_tuple(&my_empty_undo, false);
}

void my_cut_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col, vector<wstring> lines, intptr_t r, intptr_t c) {
   control->change_cursor(r, c, Control::REAL);
   control->change_view(view_row, view_col, model->number_of_lines());
   for ( size_t i = r; i < r + lines.size(); i++ )
   {
      auto line = model->get_row(i);
      if ( i == r )
      {
         auto& string_to_paste = lines[i - r];
         line->insert(line->begin() + c, string_to_paste.begin(), string_to_paste.end());
      } else if ( i == r + lines.size() - 1 )
      {
         auto& c = lines[i - r];
         line->insert(line->begin(), c.begin(), c.end());
      } else
      {
         model->insert(i, lines[i - r]);
      }
   }
   control->wrap_content();
}

KeyCord::command_return_t my_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   control->get_command_history().undo();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_redo(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   control->get_command_history().redo();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_start_recording(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   _macro.clear_recording();
   _macro.set_recording();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_stop_recording(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   if ( _macro.is_recording() )
   {
      _macro.stop_recording();
   }
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_execute_recording(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   _macro.play_back();
   return make_tuple(&my_empty_undo, false);
}

KeyCord::command_return_t my_page_up(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   intptr_t width, height;
   view->get_win_prop(width, height);
   control->change(-height, Control::VISUAL, 0, Control::VISUAL, model, view, control);
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t my_page_down(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   intptr_t width, height;
   view->get_win_prop(width, height);
   control->change(height, Control::VISUAL, 0, Control::VISUAL, model, view, control);
   return make_tuple(&my_position_undo, true);
}

KeyCord::command_return_t my_command_line_insert(std::shared_ptr<Model> model, std::shared_ptr<View> view, shared_ptr<Control> control) {
   return make_tuple(&my_empty_undo, false);
}


