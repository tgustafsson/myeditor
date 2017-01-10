#include "CommandLine.h"

using namespace std;

KeyCord::command_return_t CommandLine::my_command_line_insert(shared_ptr<Model> model, shared_ptr<View> view, shared_ptr<Control> control, shared_ptr<Control> command_line_control, shared_ptr<Model> command_line_model)
{
   return make_tuple(&my_empty_undo, false);
}
