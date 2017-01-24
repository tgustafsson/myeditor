#pragma once

#include "Mode.h"
#include "Model.h"
#include "View.h"
#include <memory>
#include <vector>
#include "AttributedString.h"

class SelectionMode : public Mode
{
private:
	std::shared_ptr<Model> m_model;
	std::shared_ptr<View> m_view;
public:
	SelectionMode(const KeyCords& keys, std::shared_ptr<Control> control, std::shared_ptr<Model> model, std::shared_ptr<View> view, KeyCord::insert_func insert);
	std::vector<std::shared_ptr<AttributedString>> syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows);
};

void selection_background(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, std::shared_ptr<Selection> selection, std::vector<std::shared_ptr<AttributedString>> rows, AttributedString::color active_background, AttributedString::color inactive_background);
