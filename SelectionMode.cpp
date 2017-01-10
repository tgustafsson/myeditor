#include "SelectionMode.h"
#include "Selection.h"
#include <algorithm>
#include "Control.h"

using namespace std;

SelectionMode::SelectionMode(const KeyCords& keys, std::shared_ptr<Control> control, shared_ptr<Model> model, shared_ptr<View> view, KeyCord::insert_func insert) : 
	Mode(keys, control, insert), m_model(model), m_view(view) {}

vector<std::shared_ptr<AttributedString>> SelectionMode::syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows)
{
	intptr_t width, height, srow, scol;
	m_control->get_view(srow, scol);
	m_view->get_win_prop(width, height);
	auto selections = m_model->get_selections();
	if (selections.size() > 0)
	{
		for (auto selection : selections)
		{
			Selection::selection_t first;
			Selection::selection_t second;
			if (selection->has_end())
			{
				if (selection->get_start(Control::change_t::REAL, m_control) <= selection->get_end(Control::change_t::REAL, m_control)) 
				{
					first = selection->get_start(Control::change_t::VISUAL, m_control);
					second = selection->get_end(Control::change_t::VISUAL, m_control);
				}
				else
				{
					first = selection->get_end(Control::change_t::VISUAL, m_control);
					second = selection->get_start(Control::change_t::VISUAL, m_control);
				}
			}
			else
			{
				Selection::selection_t cursor = make_tuple(m_control->get_row_no(Control::VISUAL), m_control->get_col(Control::VISUAL), Control::change_t::VISUAL); 
				if (selection->get_start(Control::change_t::VISUAL, m_control) <= cursor)
				{
					first = selection->get_start(Control::change_t::VISUAL, m_control);
					second = cursor;
				}
				else
				{
					first = cursor;
					second = selection->get_start(Control::change_t::VISUAL, m_control);
				}
			}
			size_t i = 0;
			for (; i < rows.size(); i++)
			{
				for (size_t j = scol; j < std::min(rows[i]->length(), static_cast<size_t>(scol + width)); j++)
				{
					Selection::selection_t pos = make_tuple(i + srow, j, Control::change_t::VISUAL); 
					if (first <= pos && pos < second)
					{
						auto temp = (*rows[i])[j];
						get<1>(temp) = AttributedString::color::INC_SEARCH;
						rows[i]->invalidate_syntax_coloring();
					}
				}
			}
		}
	}
	return rows;
}