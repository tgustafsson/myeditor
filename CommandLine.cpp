#include "CommandLine.h"


KeyCord::command_return_t CommandLine::my_command_line_insert(std::shared_ptr<Model> model, std::shared_ptr<View> view, Control& control, std::shared_ptr<Control> command_line_control, std::shared_ptr<Model> command_line_model)
{
   return make_tuple(&my_empty_undo, false);
}
