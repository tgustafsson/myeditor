#pragma once

#include <tuple>
#include "Control.h"

class SelectionError
{
};

class Selection
{
public:
   typedef std::tuple<intptr_t, intptr_t, Control::change_t> selection_t;
private:
   selection_t m_start;
   selection_t m_end;
   bool m_has_end;
public:
   Selection(size_t row, size_t col, Control::change_t c);
   Selection(selection_t);
   void add_end(selection_t);
   bool has_end() const;
   selection_t get_start(Control::change_t c, std::shared_ptr<Control> control) const;
   selection_t get_end(Control::change_t c, std::shared_ptr<Control> control) const;
   friend bool operator<(const Selection::selection_t& lhs, const Selection::selection_t& rhs);
   friend bool operator<=(const Selection::selection_t& lhs, const Selection::selection_t& rhs);
   friend bool operator==(const Selection::selection_t& lhs, const Selection::selection_t& rhs);
};
