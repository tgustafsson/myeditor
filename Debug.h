#ifndef DEBUG_H
#define DEBUG_H

#include <fstream>
#include <string>

class Debug
{
private:
    std::fstream m_fs;
public:
    Debug();
    virtual ~Debug();
    friend Debug& operator<<(Debug& lhs, const char* rhs);
    friend Debug& operator<<(Debug& lhs, int rhs);
    friend Debug& operator<<(Debug& lhs, intptr_t rhs);
    friend Debug& operator<<(Debug& lhs, char rhs);
    friend Debug& operator<<(Debug& lhs, size_t rhs);
    friend Debug& operator<<(Debug& lhs, const std::string& s);
    friend Debug& operator<<(Debug& lhs, const std::wstring& s);
};

extern Debug _debug;

#endif // DEBUG_H
