#ifndef VIEW_H
#define VIEW_H

#include "Mode.h"
#include <vector>

class View
{
protected:
   intptr_t m_width;
   intptr_t m_height;
   intptr_t m_win_x;
   intptr_t m_win_y;
   intptr_t m_cursor_row;
   intptr_t m_cursor_col;
public:
   virtual ~View() {
   }
   virtual void getmaxxy(intptr_t& rows, intptr_t& cols) = 0;
   virtual void update(const std::vector<std::shared_ptr<AttributedString>> rows, size_t view_col) = 0;
   virtual void get_win_prop(intptr_t& width, intptr_t& height) = 0;
   virtual void change_visual_cursor(size_t row, size_t col) = 0;
   virtual void refresh() = 0;
};

#endif //VIEW_H
