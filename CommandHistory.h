#pragma once

#include <functional>
#include <tuple>
#include <list>
#include <vector>

class CommandHistory
{
public:
	typedef std::function<void(void)> insert_command;
	typedef std::function<void(void)> undo_command;
private:
	std::vector<std::tuple<insert_command, undo_command>> m_history;
	size_t m_pos;
public:
	CommandHistory() : m_pos(0) {}
	void add_command(insert_command history, undo_command undo);
	void undo();
	void redo();
	void clear();
};