#include "Debug.h"
#include <ios>
#include <iostream>

#include <locale>
#include <codecvt>
#include <string>

Debug _debug;

using namespace std;

Debug::Debug()
{
    m_fs.open("/tmp/debug.txt", ios_base::out);
}

Debug::~Debug()
{
    m_fs.flush();
    m_fs.close();
}

Debug& operator<<(Debug& lhs, const char* rhs)
{
    lhs.m_fs << rhs;
    lhs.m_fs.flush();
    return lhs;
}

Debug& operator<<(Debug& lhs, char rhs)
{
    lhs.m_fs << rhs;
    lhs.m_fs.flush();
    return lhs;
}

Debug& operator<<(Debug& lhs, int rhs)
{
    lhs.m_fs << rhs;
    lhs.m_fs.flush();
    return lhs;
}

Debug& operator<<(Debug& lhs, intptr_t rhs)
{
	lhs.m_fs << rhs;
	lhs.m_fs.flush();
	return lhs;
}

Debug& operator<<(Debug& lhs, size_t rhs)
{
	lhs.m_fs << rhs;
	lhs.m_fs.flush();
	return lhs;
}

Debug& operator<<(Debug& lhs, const string& s)
{
  lhs.m_fs << s;
	for (auto a : s)
	{
	  lhs.m_fs << static_cast<char>(a);
	}
	lhs.m_fs.flush();
	return lhs;
}

Debug& operator<<(Debug& lhs, const wstring& s)
{
  std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
  std::string narrow = converter.to_bytes(s);

  lhs.m_fs << narrow;

	lhs.m_fs.flush();
	return lhs;
}
