#include "Clipboard.h"
#include <vector>

using namespace std;

Clip empty_clip;

Clip::Clip()
{}

Clip::Clip(const wstring& str)
{
	m_lines.push_back(str);
}

Clip::Clip(const vector<wstring>& lines)
{
	m_lines = lines;
}

void Clipboard::add_clip(const Clip& clip)
{
	m_clips.push_front(clip);
}

const Clip& Clipboard::top_clip() const
{
	if (m_clips.size() > 0)
		return m_clips.front();
	else
		return empty_clip;
}

Clipboard::iterator Clipboard::begin()
{
	return m_clips.begin();
}

Clipboard::iterator Clipboard::end()
{
	return m_clips.end();
}

void Clipboard::clear()
{
	m_clips.clear();
}

Clipboard _clipboard;