#pragma once

#include "Control.h"
#include "Model.h"
#include "View.h"
#include "TabsMode.h"
#include <memory>

class IncrementalSearch
{
private:
   intptr_t m_start_row, m_start_col, m_start_view_row, m_start_view_col, m_delta_row, m_delta_col;
   bool m_wrap;
   Model::SearchDirection m_dir;
   std::shared_ptr<Model> m_model;
   std::shared_ptr<View> m_view;
   bool m_run_loop;
   bool m_quitting;
public:
   IncrementalSearch() : m_delta_row(0), m_delta_col(0), m_wrap(true), m_dir(Model::SearchDirection::FORWARD), m_run_loop(true), m_quitting(false) { }
   KeyCord::command_return_t my_incremental_search(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, std::shared_ptr<Control> incremental_search_control, std::shared_ptr<Model> incremental_search_model, std::shared_ptr<View> incremental_search_view);
   KeyCord::command_return_t my_incremental_search_insert(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, wchar_t wc);
   KeyCord::command_return_t my_incremental_search_enter(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
   KeyCord::command_return_t my_incremental_search_again(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
   KeyCord::command_return_t my_incremental_search_quit(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
   KeyCord::command_return_t my_incremental_search_backspace(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
};