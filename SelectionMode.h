#pragma once

#include "Mode.h"
#include "Model.h"
#include "View.h"

class SelectionMode : public Mode
{
private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<View> m_view;
public:
	SelectionMode(const KeyCords& keys, std::shared_ptr<Control> control, std::shared_ptr<Model> model, std::shared_ptr<View> view, KeyCord::insert_func insert);
	std::vector<std::shared_ptr<AttributedString>> syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows);
};