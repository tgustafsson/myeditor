#pragma once

#include "KeyCord.h"
#include "AttributedString.h"

class Control;

class Mode
{
protected:
	KeyCords m_cords;
	std::shared_ptr<Control> m_control;
	std::vector<int> m_current_cord;
	KeyCord m_empty;
protected:
	KeyCord::insert_func m_insert;
public:
	Mode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert);
	KeyCord::cord_func get_key();
	virtual std::vector<std::shared_ptr<AttributedString>> syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows);
};