#include "Mode.h"
#include "Control.h"

using namespace std;

Mode::Mode(const KeyCords& keys, shared_ptr<Control> control, KeyCord::insert_func insert) : m_empty({
}, &my_empty), m_insert(insert) {
   m_cords = keys;
   m_control = control;
}

KeyCord::cord_func Mode::get_key() {
   tuple<Control::keys, int> current_key = m_control->get_key();
   if ( get<0>(current_key) == Control::keys::UNDEFINED )
   {
      m_current_cord.push_back(get<1>(current_key));
   } else
   {
      m_current_cord.push_back(get<0>(current_key));
   }
   bool found = false;
   for ( auto& cord : m_cords )
   {
      if ( cord.m_cord.size() > m_current_cord.size() )
      {
         size_t i;
         for ( i = 0; i < m_current_cord.size(); i++ )
         {
            if ( cord.m_cord[i] != m_current_cord[i] ) break;
         }
         if ( i == m_current_cord.size() ) found = true;
      }
   }
   if ( !found )
   {
      for ( auto& cord : m_cords )
      {
         if ( cord.m_cord.size() == m_current_cord.size() )
         {
            size_t i;
            for ( i = 0; i < m_current_cord.size(); i++ )
            {
               if ( cord.m_cord[i] != m_current_cord[i] ) break;
            }
            if ( i == m_current_cord.size() )
            {
               m_current_cord.clear();
               return cord.m_func;
            }
         }
      }
      m_current_cord.clear();
      auto f = bind(m_insert, placeholders::_1, placeholders::_2, placeholders::_3, get<1>(current_key));
      return f;
   }
   return m_empty.m_func;
}

std::vector<std::shared_ptr<AttributedString>> Mode::syntax_highlight(std::vector<std::shared_ptr<AttributedString>> rows) {
   return rows;
}
