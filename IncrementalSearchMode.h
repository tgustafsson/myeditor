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
};