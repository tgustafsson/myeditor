#pragma once
#include "Mode.h"
#include <string>

class TabsMode : public Mode
{
private:
public:
   TabsMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert);
   virtual std::vector<std::shared_ptr<AttributedString>> syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows);
}; 
