#pragma once

#include "Mode.h"
#include <string>

class IncrementalSearchMode : public Mode
{
private:
   std::wstring m_search;
public:
   IncrementalSearchMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert);
   virtual std::vector<std::shared_ptr<AttributedString>> syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows);
   void set_search(std::wstring search);
   std::wstring get_search() const; 
};

void incremental_search_background(std::shared_ptr<Control> control, std::vector<std::shared_ptr<AttributedString>> rows, const std::wstring& search, AttributedString::color active_color, AttributedString::color matched_color);
