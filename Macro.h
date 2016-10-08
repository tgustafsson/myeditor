#pragma once

#include <list>
#include <functional>

class Macro
{
public:
	typedef std::function<void(void)> macro_function_t;
private:
	std::list<macro_function_t> m_functions;
	bool m_recording;
public:
	Macro() : m_recording(false) {}
	void add_function(macro_function_t func);
	bool is_recording() const;
	void set_recording();
	void stop_recording();
	void play_back();
	void clear_recording();
};

extern Macro _macro;