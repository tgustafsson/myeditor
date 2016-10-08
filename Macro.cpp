#include "Macro.h"

void Macro::add_function(Macro::macro_function_t func)
{
	m_functions.push_back(func);
}

bool Macro::is_recording() const
{
	return m_recording;
}

void Macro::set_recording()
{
	m_recording = true;
}

void Macro::stop_recording()
{
	m_recording = false;
	m_functions.pop_front();
	m_functions.pop_back();
}

void Macro::play_back()
{
	for (auto f : m_functions)
		f();
}

void Macro::clear_recording()
{
	m_functions.clear();
}

Macro _macro;