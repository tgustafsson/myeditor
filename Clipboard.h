#pragma once

#include <vector>
#include <string>
#include <list>

class Clip
{
private:
public:
	Clip();
	Clip(const std::wstring& str);
	Clip(const std::vector<std::wstring>& lines);
	std::vector < std::wstring > m_lines;
};

class Clipboard
{
public:
	typedef std::list<Clip>::iterator iterator;
private:
	std::list<Clip> m_clips;
public:
	void add_clip(const Clip& clip);
	const Clip& top_clip() const;
	iterator begin();
	iterator end();
	void clear();
};

extern Clipboard _clipboard;