#pragma once
#include "Mode.h"
#include <string>

class TabsMode : public Mode
{
private:
   intptr_t m_delta_cursor_col;
   char m_tabsize;
public:
   TabsMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert);
   virtual std::vector<std::shared_ptr<AttributedString>> syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows);
   intptr_t get_delta_col() const;
   void set_tabsize(char);
   char get_tabsize() const;
}; 
