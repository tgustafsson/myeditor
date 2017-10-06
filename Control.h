#ifndef CONTROL_H
#define CONTROL_H

#include "Model.h"
#include "View.h"
#include <tuple>
#include <functional>
#include "CommandHistory.h"
#include <map>

class Mode;

class Control
{
protected:
   std::shared_ptr<Model> m_model;
   std::vector<std::shared_ptr<Mode>> m_ref_to_modes;
   std::map<int, std::shared_ptr<Mode>> m_modes;
   std::map<intptr_t, std::shared_ptr<AttributedString>> m_map_real_row_to_visual_contents;
   bool m_execute;
   CommandHistory m_commandhistory;
   intptr_t m_real_row;
   intptr_t m_real_col;
   intptr_t m_view_row;
   intptr_t m_view_col;
public:
   enum keys
   {
      CTRL_X,
      CTRL_C,
      CTRL_F,
      CTRL_A,
      CTRL_E,
      CTRL_S,
      CTRL_V,
      CTRL_N,
      CTRL_P,
      CTRL_B,
      CTRL_D,
      CTRL_G,
      CTRL_K,
      CTRL_Y,
      CTRL_HALF,
      CTRL_SPACE,
      CTRL_UNDERSCORE,
      CTRL_W,
      LEFT,
      UP,
      RIGHT,
      DOWN,
      ENTER,
      TAB,
      BACKSPACE,
      HOME,
      END,
      ALTF,
      ALTB,
      ALTW,
      ALTV,
      ALTX,
      UNDEFINED
   };

   enum change_t
   {
      VISUAL,
      REAL,
      WITH_INSERTED_VISUAL_LINES
   };
   
   const static int MAJOR_MODE = 0;
   const static int INCREMENTAL_MODE = 2;
   const static int SELECTION_MODE = 3;
   const static int TABS_MODE = 1;

   Control(std::shared_ptr<Model> m);
   virtual ~Control();
   void add_mode(int mode_type, std::shared_ptr<Mode> mode);
   void exit();
   void set_execute();
   bool get_execute();
   const std::vector<std::shared_ptr<Mode>>& get_modes();
   std::shared_ptr<Mode> get_mode(int mode_type); 
   virtual std::tuple<keys, int> get_key() const = 0;
   //virtual void loop() = 0;
   CommandHistory& get_command_history();
   virtual void wrap_content();
   virtual void change(intptr_t delta_row, change_t row_change, intptr_t delta_col, change_t col_change, std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
   virtual void change_cursor(intptr_t row, intptr_t col, change_t t);
   virtual void change_view(intptr_t row, intptr_t col, size_t number_of_lines, Control::change_t t);
   virtual void get_cursor_pos(intptr_t& row, intptr_t& col, change_t t);
   virtual void get_view(intptr_t& row, intptr_t& col, Control::change_t t);
   virtual std::shared_ptr<AttributedString> get_row(change_t t, intptr_t delta);
   virtual size_t get_row_no(change_t t);
   virtual size_t get_col(change_t t);
   virtual std::vector<std::shared_ptr<AttributedString>> rows(change_t t, intptr_t start_row, intptr_t end_row);
   virtual void convert_to(Control::change_t from, size_t row, size_t col, Control::change_t to, intptr_t& _row, intptr_t& _col);
   virtual void insert_visual_rows(const std::vector<std::shared_ptr<AttributedString>>&, intptr_t start_row); 
   virtual std::vector<intptr_t> get_real_rows_being_visual(std::shared_ptr<View> view); 
};

extern size_t _number_key_presses;

//void change(intptr_t delta_row, intptr_t delta_col, std::shared_ptr<Model> model, std::shared_ptr<View> view);
KeyCord::command_return_t my_left(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_right(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t down(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t up(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t word_right(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t word_left(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_exit(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_empty(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_begin_line(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_end_line(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_insert(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, wchar_t wc);
KeyCord::command_return_t my_backspace(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_enter(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_delete(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_save(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_open_file(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, std::shared_ptr<Control> file_select_control, std::shared_ptr<Model> file_select_model,std::shared_ptr<View> file_select_view);
KeyCord::command_return_t my_file_select_tab(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_file_select_enter(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_kut_line(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_paste_from_clipboard(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_start_selection(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_ctrl_g(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_copy_selection(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_cut_selection(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_redo(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_start_recording(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_stop_recording(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_execute_recording(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_page_up(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
KeyCord::command_return_t my_page_down(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);

void my_empty_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col);
void my_position_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col);
void my_insert_character_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col);
void my_delete_character_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col, wchar_t wch);
void my_cut_undo(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, size_t row, size_t col, size_t view_row, size_t view_col, std::vector<std::wstring> lines, intptr_t r, intptr_t c);

KeyCord::command_return_t my_command_line_insert(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);

#endif //CONTROL_H
