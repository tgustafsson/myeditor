#include "CommandHistory.h"
#include "Debug.h"

using namespace std;

void CommandHistory::add_command(insert_command history, undo_command undo)
{
   tuple<insert_command, undo_command> temp = make_tuple(history, undo);
   m_history.insert(m_history.begin() + m_pos, temp);//.insert(m_history_iterator, temp);
   m_pos++;
}

void CommandHistory::undo()
{
   if (m_history.size() > 0 && m_pos > 0)
   {
      auto temp = m_history.begin();
      auto hist = *(temp + m_pos - 1);
      get<1>(hist)();
      m_pos--;
   }
}

void CommandHistory::redo()
{
   if (m_history.size() > 0 && m_pos < m_history.size())
   {
      auto temp = m_history.begin();
      auto hist = *(temp + m_pos);
      get<0>(hist)();
      m_pos++;
   }
}

void CommandHistory::clear()
{
   m_history.clear();
   m_pos = 0;
}
