#include <vector>
#include <string>

class Command
{
private:
   std::wstring m_command;
public:
   Command(std::wstring command);
};

extern std::vector<Command> commands;

