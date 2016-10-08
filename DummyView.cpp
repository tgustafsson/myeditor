#include "DummyView.h"

using namespace std;

void DummyView::getmaxxy(intptr_t& rows, intptr_t& cols)
{
	rows = 0;
	cols = 0;
}

void DummyView::update(vector<shared_ptr<AttributedString>>, shared_ptr<Mode>, size_t a)
{
}

void DummyView::get_win_prop(intptr_t& a, intptr_t& b)
{
	a = 0;
	b = 0;
}

void DummyView::change_visual_cursor(size_t a, size_t b)
{

}

void DummyView::refresh()
{
}
