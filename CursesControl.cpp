#include "CursesControl.h"
#include "Mode.h"
#include <tuple>
#include <vector>
#ifdef _MSC_VER
#include <curses.h>
#else
#include <ncursesw/curses.h>
#endif
#include "Debug.h"
#include "Macro.h"

using namespace std;

tuple<Control::keys, int> CursesControl::get_key() const
{
	_number_key_presses++;
	Control::keys ret_key = Control::keys::UNDEFINED;
	int ret_code = getch();
	_debug << "code is " << ret_code << "\n";
#ifdef _MSC_VER
	switch (ret_code)
	{
	case ALT_F: return make_tuple(keys::ALTF, ret_code);
	case ALT_B: return make_tuple(keys::ALTB, ret_code);
	case ALT_W: return make_tuple(keys::ALTW, ret_code);
	case ALT_V: return make_tuple(keys::ALTV, ret_code);
	}
#else
	if (ret_code == 27)
	{
		ret_code = getch();
		switch (ret_code)
		{
		case 'f': return make_tuple(keys::ALTF, ret_code);
		case 'b': return make_tuple(keys::ALTB, ret_code);
		case 'w': return make_tuple(keys::ALTW, ret_code);
		case 'v': return make_tuple(keys::ALTV, ret_code);
		}
	}
#endif
	switch (ret_code)
	{
	case 24:  return make_tuple(keys::CTRL_X, ret_code);
	case 3:   return make_tuple(keys::CTRL_C, ret_code);
	case 6:   return make_tuple(keys::CTRL_F, ret_code);
	case 1:   return make_tuple(keys::CTRL_A, ret_code);
	case 5:   return make_tuple(keys::CTRL_E, ret_code);
	case 19:  return make_tuple(keys::CTRL_S, ret_code);
	case 22:  return make_tuple(keys::CTRL_V, ret_code);
	case 0:   return make_tuple(keys::CTRL_SPACE, ret_code); 
	case 31:  return make_tuple(keys::CTRL_UNDERSCORE, ret_code);
	case 260: return make_tuple(keys::LEFT, ret_code);
	case 258: return make_tuple(keys::DOWN, ret_code);
	case 259: return make_tuple(keys::UP, ret_code);
	case 261: return make_tuple(keys::RIGHT, ret_code);
	case 10:  return make_tuple(keys::ENTER, ret_code);
	case 9:   return make_tuple(keys::TAB, ret_code);
	case 263: return make_tuple(keys::BACKSPACE, ret_code);
	case 262: return make_tuple(keys::HOME, ret_code);
	case 360: return make_tuple(keys::END, ret_code);
	case 14:  return make_tuple(keys::CTRL_N, ret_code);
	case 16:  return make_tuple(keys::CTRL_P, ret_code);
	case 2:   return make_tuple(keys::CTRL_B, ret_code);
	case 8:   return make_tuple(keys::BACKSPACE, ret_code);
	case 127: return make_tuple(keys::BACKSPACE, ret_code);
	case 4:   return make_tuple(keys::CTRL_D, ret_code);
	case 7:   return make_tuple(keys::CTRL_G, ret_code);
	case 11:  return make_tuple(keys::CTRL_K, ret_code);
	case 25:  return make_tuple(keys::CTRL_Y, ret_code);
	case 28:  return make_tuple(keys::CTRL_HALF, ret_code);
	//case 32:  return make_tuple(keys::CTRL_SPACE, ret_code);
	case 23:  return make_tuple(keys::CTRL_W, ret_code);
	}
	return make_tuple(UNDEFINED, ret_code);
}

void CursesControl::loop()
{
	while (this->m_execute)
	{
		intptr_t width, height;
		m_view->get_win_prop(width, height);
		auto _rows = rows(Control::VISUAL, m_view_row, m_view_row + height);
		m_view->update(_rows, m_modes[0], m_view_col);
		if (m_modes.size() >= 3)
		{
			m_view->update(_rows, m_modes[2], m_view_col);
		}
		intptr_t row, col;
		get_cursor_pos(row, col, Control::VISUAL);
		m_view->change_visual_cursor(row, col);
		m_view->refresh();
		auto func = m_modes[0]->get_key();
		auto hist = bind(func, m_model, m_view, ref(*this));
		auto undo = func(m_model, m_view, ref(*this));
		wrap_content();
		auto f = get<0>(undo);
		if (get<1>(undo))
			m_commandhistory.add_command(hist, bind(f, m_model, m_view, ref(*this), m_real_row, m_real_col, m_view_row, m_view_col));
		if (_macro.is_recording())
			_macro.add_function(hist);
	}
}