#pragma once

#include <functional>
#include <memory>
#include <vector>
#include <tuple>

class Model;
class View;
class Control;

struct KeyCord
{
	typedef std::function<void(std::shared_ptr<Model>, std::shared_ptr<View>, std::shared_ptr<Control>, size_t, size_t, size_t, size_t)> undo_func;
	typedef std::tuple<KeyCord::undo_func, bool> command_return_t;
	typedef std::function<command_return_t(std::shared_ptr<Model>, std::shared_ptr<View>, std::shared_ptr<Control>)> cord_func;
	typedef std::function<command_return_t(std::shared_ptr<Model>, std::shared_ptr<View>, std::shared_ptr<Control>, wchar_t)> insert_func;
	std::vector<int> m_cord;
	cord_func m_func;
	KeyCord(std::vector<int> cord, cord_func func)
	{
		m_cord = cord;
		m_func = func;
	}
};

typedef std::vector<KeyCord> KeyCords;