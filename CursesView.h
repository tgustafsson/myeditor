#pragma once

#include "View.h"
#include "Mode.h"
#include <map>

class CursesView : public View
{
protected:
	static std::map<AttributedString::color, int> m_color_to_pair;
	static int m_initialized;
public:
	CursesView()
	{
	}
	CursesView(intptr_t width, intptr_t height, intptr_t win_x, intptr_t win_y);
	virtual ~CursesView();
	virtual void getmaxxy(intptr_t& rows, intptr_t& cols);
	virtual void update(const std::vector<std::shared_ptr<AttributedString>> rows, size_t view_col);
	virtual void get_win_prop(intptr_t& width, intptr_t& row);
	virtual void refresh();
	virtual void change_visual_cursor(size_t row, size_t col);
};
