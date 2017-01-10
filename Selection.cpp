#include "Selection.h"

using namespace std;

Selection::Selection(size_t row, size_t col, Control::change_t c) : m_has_end(false)
{
	if (c == Control::change_t::REAL)
		m_start = make_tuple(row, col, c);
	else
		throw(SelectionError());
}

Selection::Selection(selection_t start)
{
		m_start = start;
}

void Selection::add_end(selection_t end)
{
	m_has_end = true;
	m_end = end;
}

bool Selection::has_end() const
{
	return m_has_end;
}

Selection::selection_t Selection::get_start(Control::change_t c, shared_ptr<Control> control) const
{
	if (c == Control::change_t::REAL)
	{
		return m_start;
	}
	else if (c == Control::change_t::VISUAL)
	{
		intptr_t row, col;
		control->convert_to(Control::change_t::REAL, get<0>(m_start), get<1>(m_start), Control::change_t::VISUAL, row, col);
		return make_tuple(row, col, Control::change_t::VISUAL);
	}
}

Selection::selection_t Selection::get_end(Control::change_t c, shared_ptr<Control> control) const
{
	if (c == Control::change_t::REAL)
	{
		return m_end;
	}
	else if (c == Control::change_t::VISUAL)
	{
		intptr_t row, col;
		control->convert_to(Control::change_t::REAL, get<0>(m_end), get<1>(m_end), Control::change_t::VISUAL, row, col);
		return make_tuple(row, col, Control::change_t::VISUAL);
	}
}

bool operator<(const Selection::selection_t& lhs, const Selection::selection_t& rhs)
{
	if (get<0>(lhs) < get<0>(rhs))
		return true;
	else if (get<0>(lhs) == get<0>(rhs))
	{
		if (get<1>(lhs) < get<1>(rhs))
			return true;
	}
	return false;
}

bool operator<=(const Selection::selection_t& lhs, const Selection::selection_t& rhs)
{
	if (get<0>(lhs) < get<0>(rhs))
		return true;
	else if (get<0>(lhs) == get<0>(rhs))
	{
		if (get<1>(lhs) <= get<1>(rhs))
			return true;
	}
	return false;
}

bool operator==(const Selection::selection_t& lhs, const Selection::selection_t& rhs)
{
	if (get<0>(lhs) == get<0>(rhs))
		if (get<1>(lhs) == get<1>(rhs))
		{
			return true;
		}
	return false;
}