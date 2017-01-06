#pragma once

#include "Control.h"
#include "Model.h"
#include "View.h"
#include <memory>

class CommandLine
{
private:
   std::shared_ptr<Model> m_model;
   std::shared_ptr<View> m_view;
   bool m_run_loop;
   bool m_quitting;
public:
   CommandLine() : m_run_loop(true), m_quitting(false) 
   {
   }
   KeyCord::command_return_t my_command_line_insert(std::shared_ptr<Model> model, std::shared_ptr<View> view, Control& control, std::shared_ptr<Control> command_line_control, std::shared_ptr<Model> command_line_model); 
};
