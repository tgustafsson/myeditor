#pragma once

#include "CursesView.h"

class DummyView : public CursesView
{
public:
	DummyView()
	{ 
	}
	DummyView(intptr_t width, intptr_t height, intptr_t win_x, intptr_t win_y) : CursesView(width,height,win_x,win_y)
	{
	}
	virtual void getmaxxy(intptr_t& rows, intptr_t& cols);
	virtual void update(std::vector<std::shared_ptr<AttributedString>>, std::shared_ptr<Mode>, size_t a);
	virtual void  get_win_prop(intptr_t& a, intptr_t& b);
	virtual void  change_visual_cursor(size_t a, size_t b);
	virtual void  refresh();
};
