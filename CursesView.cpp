#include "CursesView.h"
#ifdef _MSC_VER
#include <curses.h>
#else
#include <ncursesw/curses.h>
#endif
#include "Mode.h"
#include "Debug.h"

using namespace std;

int CursesView::m_initialized = 0;
map<AttributedString::color, int> CursesView::m_color_to_pair = {
};

extern "C"
{ FILE __iob_func[3] = { *stdin, *stdout, *stderr }; }

void set_prop(intptr_t& prop, intptr_t value, intptr_t _max) {
   if ( value < 0 ) prop = _max + value;
   else prop = value;
}

CursesView::CursesView(intptr_t width, intptr_t height, intptr_t win_x, intptr_t win_y) {
   if ( m_initialized == 0 )
   {
      initscr();
      if ( has_colors() )
      {
         start_color();
         /*STRING,
                 TRIPLE_QUOTE_STRING,
                 LONG_COMMENT,
                 LINE_COMMENT,
                 NORMAL,
                 KEYWORD,
                 DATATYPE,
                 CERROR,
                 INC_SEARCH*/
         init_pair(1, COLOR_YELLOW, COLOR_BLACK); // keywords
         init_pair(2, COLOR_WHITE, COLOR_BLACK); // normal text
         init_pair(3, COLOR_GREEN, COLOR_BLACK); // comments
         init_pair(4, COLOR_BLUE, COLOR_BLACK); // strings
         init_pair(5, COLOR_RED, COLOR_BLACK); // error
         init_pair(6, COLOR_GREEN, COLOR_CYAN); // long comment
         init_pair(7, COLOR_BLUE, COLOR_CYAN); // triple strings
         init_pair(8, COLOR_CYAN, COLOR_BLACK); // data type
         init_pair(9, COLOR_BLACK, COLOR_WHITE); // inc search
         init_pair(10, COLOR_BLACK, COLOR_YELLOW); // hilight
         init_pair(11, COLOR_BLACK, COLOR_MAGENTA); // tab
         m_color_to_pair[AttributedString::color::KEYWORD] = 1;
         m_color_to_pair[AttributedString::color::NORMAL] = 2;
         m_color_to_pair[AttributedString::color::LINE_COMMENT] = 3;
         m_color_to_pair[AttributedString::color::STRING] = 4;
         m_color_to_pair[AttributedString::color::CERROR] = 5;
         m_color_to_pair[AttributedString::color::LONG_COMMENT] = 6;
         m_color_to_pair[AttributedString::color::TRIPLE_QUOTE_STRING] = 7;
         m_color_to_pair[AttributedString::color::DATATYPE] = 8;
         m_color_to_pair[AttributedString::color::INC_SEARCH] = 9;
         m_color_to_pair[AttributedString::color::HILIGHT] = 10; 
         m_color_to_pair[AttributedString::color::TAB] = 11; 
      }
      raw();
      noecho();
      keypad(stdscr, TRUE);
   }
   m_initialized++;
   intptr_t rows, cols;
   this->getmaxxy(rows, cols);
   set_prop(m_width, width, cols);
   set_prop(m_height, height, rows);
   set_prop(m_win_x, win_x, cols);
   set_prop(m_win_y, win_y, rows);
}

CursesView::~CursesView() {
   m_initialized--;
   if ( m_initialized == 0 )
   {
      noraw();
      endwin();
   }
}

void CursesView::change_visual_cursor(size_t row, size_t col) {
   m_cursor_row = row;
   m_cursor_col = col;
   move(m_win_y + m_cursor_row, m_win_x + m_cursor_col);
}

void CursesView::getmaxxy(intptr_t& rows, intptr_t& cols) {
#ifdef _MSC_VER
   rows = ::getmaxy(stdscr);
   cols = ::getmaxx(stdscr);
#else
   getmaxyx(stdscr, rows, cols);
#endif
}

void CursesView::update(const std::vector<shared_ptr<AttributedString>> rows, size_t view_col) {
   auto attributed_view = rows;
   int y = m_win_y;
   for ( intptr_t bx = m_win_x - 1; bx <= m_width; bx++ )
   {
      attron(A_BOLD | COLOR_PAIR(1));
      mvaddch(y - 1, bx, L'=');
      attroff(A_BOLD | COLOR_PAIR(1));
   }
   for ( auto line : attributed_view )
   {
      int x = m_win_x;
      attron(A_BOLD | COLOR_PAIR(1));
      mvaddch(y, x - 1, L'=');
      attroff(A_BOLD | COLOR_PAIR(1));
      for ( size_t i = view_col; i < line->length() && i <= view_col + m_width; i++ )
      {
         auto t = (*line)[i];
         attron(A_BOLD | COLOR_PAIR(m_color_to_pair[get<1>(t)]));
         if ( get<0>(t) == '\r' )
         {
            mvaddch(y, x, ' ');
         } else
         {
            mvaddch(y, x, get<0>(t));
         }
         attroff(A_BOLD | COLOR_PAIR(m_color_to_pair[get<1>(t)]));
         x++;
      }
      while ( x < m_win_x + m_width )
      {
         attron(COLOR_PAIR(m_color_to_pair[AttributedString::color::NORMAL]));
         mvaddch(y, x, ' ');
         attroff(COLOR_PAIR(m_color_to_pair[AttributedString::color::NORMAL]));
         x++;
      }
      attron(A_BOLD | COLOR_PAIR(1));
      mvaddch(y, x, L'=');
      attroff(A_BOLD | COLOR_PAIR(1));
      y++;
   }
   while ( y < m_win_y + m_height )
   {
      intptr_t x = m_win_x;
      attron(A_BOLD | COLOR_PAIR(1));
      mvaddch(y, x - 1, L'=');
      attroff(A_BOLD | COLOR_PAIR(1));
      while ( x < m_win_x + m_width )
      {
         attron(COLOR_PAIR(m_color_to_pair[AttributedString::color::NORMAL]));
         mvaddch(y, x, ' ');
         attroff(COLOR_PAIR(m_color_to_pair[AttributedString::color::NORMAL]));
         x++;
      }
      attron(A_BOLD | COLOR_PAIR(1));
      mvaddch(y, x, L'=');
      attroff(A_BOLD | COLOR_PAIR(1));
      y++;
   }
   for ( intptr_t bx = m_win_x - 1; bx <= m_width; bx++ )
   {
      attron(A_BOLD | COLOR_PAIR(1));
      mvaddch(y, bx, L'=');
      attroff(A_BOLD | COLOR_PAIR(1));
   }
}

void CursesView::get_win_prop(intptr_t& width, intptr_t& height) {
   width = m_width;
   height = m_height;
}

void CursesView::refresh() {
   ::refresh();
}
