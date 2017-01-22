#pragma once

#include "CursesControl.h"
#include "SoftwrapAttributedString.h"
#include <vector>
#include <map>
#include <tuple>

class SoftwrapControl : public CursesControl
{
private:
   std::vector<std::shared_ptr<SoftwrapAttributedString>> m_softwrap;
   std::map<size_t, std::tuple<size_t, size_t>> m_map_soft_to_real;
   std::map<size_t, size_t> m_map_real_to_soft;
   size_t m_visual_row;
   size_t m_visual_col;
   size_t m_width;

public:
   SoftwrapControl(std::shared_ptr<Model> m, std::shared_ptr<View> v);
   virtual void wrap_content();
   //virtual void change(intptr_t delta_row, change_t row_change, intptr_t delta_col, change_t col_change, std::shared_ptr<Model> model, std::shared_ptr<View> view);
   virtual void change_cursor(intptr_t row, intptr_t col, change_t t);
   virtual void change_view(intptr_t row, intptr_t col, size_t number_of_lines);
   virtual void get_cursor_pos(intptr_t& row, intptr_t& col, change_t t);
   //virtual void get_view(intptr_t& row, intptr_t& col);
   virtual std::shared_ptr<AttributedString> get_row(change_t t, intptr_t delta = 0);
   virtual size_t get_row_no(change_t t);
   virtual size_t get_col(change_t t);
   virtual std::vector<std::shared_ptr<AttributedString>> rows(change_t t, intptr_t start_row, intptr_t end_row);
   //virtual void loop();
};
