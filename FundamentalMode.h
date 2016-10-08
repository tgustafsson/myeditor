#pragma once

#include "Mode.h"

class FundamentalMode : public Mode
{
private:
public:
	FundamentalMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert);
	virtual std::vector<std::shared_ptr<AttributedString>> syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows);
};