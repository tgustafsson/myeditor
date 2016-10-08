#include "IncrementalSearchMode.h"
#include "Control.h"
#include <tuple>

using namespace std;

IncrementalSearchMode::IncrementalSearchMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert) : Mode(keys, control, insert) {}

vector<shared_ptr<AttributedString>> IncrementalSearchMode::syntax_highlight(vector<shared_ptr<AttributedString>> rows)
{
	if (m_search != L"")
	{
		intptr_t row, col;
		m_control->get_cursor_pos(row, col, Control::VISUAL);
		size_t vrow = 0;
		for (auto r : rows)
		{
			size_t pos = 0;
			while (pos != wstring::npos)
			{
				auto s_to_s = r->to_str();
				pos = s_to_s.find(m_search, pos);
				if (pos != wstring::npos)
				{
					auto color = AttributedString::color::INC_SEARCH;
					if ((vrow == row) && (col == pos))
						color = AttributedString::color::HILIGHT;
					for (int i = 0; i < m_search.length(); i++)
					{
						auto temp = (*r)[pos];
						get<1>(temp) = color;
						pos++;
					}
					r->invalidate_syntax_coloring();
				}
			}
			vrow++;
		}
	}
	return rows;
}

void IncrementalSearchMode::set_search(wstring search)
{
	m_search = search;
}