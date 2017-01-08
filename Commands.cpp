#include "Commands.h"

using namespace std;

vector<Command> commands;

Command::Command(wstring command) : m_command(command)
{
   commands.push_back(*this);
}
