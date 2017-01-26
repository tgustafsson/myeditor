#include <iostream>
#include "Debug.h"
#include "Model.h"
#include "View.h"
#include "CppMode.h"
#include "LatexMode.h"
#include "FundamentalMode.h"
#include "FileSelectMode.h"
#include "IncrementalSearchMode.h"
#include "CursesControl.h"
#include "CursesView.h"
#include "IncrementalSearch.h"
#include "SelectionMode.h"
#include "SoftwrapControl.h"
#include "DummyView.h"
#include "Cords.h"
#include "CommandLine.h"
#include "Utilities.h"
#include "TabsMode.h"

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/positional_options.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/program_options/parsers.hpp>

#include <locale>
#include <codecvt>
#include <string>

#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/path.hpp>

using namespace std;
namespace po = boost::program_options;
namespace fs = boost::filesystem;
int main(int argc, char *argv[]) {
   po::options_description desc;
   desc.add_options()
      ("if", po::value<string>(), "input file name");
   po::positional_options_description pd;
   pd.add("if", 1); 
   po::variables_map vm;

   po::store(po::command_line_parser(argc, argv).
             options(desc).positional(pd).run(), vm);
   po::notify(vm);

   std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
   string _filename = ""; 
   if ( vm.count("if") > 0)
      _filename = vm["if"].as<string>();
   //std::string narrow = converter.to_bytes(wide_utf16_source_string);
   std::wstring input_filename = converter.from_bytes(_filename);

   //vector<KeyCord> file_select_cords;
   _file_select_cords.push_back(KeyCord({ Control::keys::CTRL_F }, &my_right));
   _file_select_cords.push_back(KeyCord({ Control::keys::CTRL_A }, &my_begin_line));
   _file_select_cords.push_back(KeyCord({ Control::keys::CTRL_E }, &my_end_line));
   _file_select_cords.push_back(KeyCord({ Control::keys::CTRL_B }, &my_left));
   _file_select_cords.push_back(KeyCord({ Control::keys::BACKSPACE }, &my_backspace));
   _file_select_cords.push_back(KeyCord({ Control::keys::CTRL_D }, &my_delete));
   _file_select_cords.push_back(KeyCord({ Control::keys::TAB }, &my_file_select_tab));
   _file_select_cords.push_back(KeyCord({ Control::keys::ENTER }, &my_file_select_enter));
   shared_ptr<Model> file_select_model = make_shared<Model>();
   shared_ptr<CursesView> file_select_view = make_shared<CursesView>(-2, 12, 1, -13);
   shared_ptr<CursesControl> file_select_control = make_shared<CursesControl>(file_select_model);
   shared_ptr<Mode> file_select_mode = make_shared<FileSelectMode>(_file_select_cords, file_select_control, &my_insert);
   file_select_control->add_mode(Control::MAJOR_MODE, file_select_mode);

   //vector<KeyCord> incremental_search_cords;
   _incremental_search_cords.push_back(KeyCord({ Control::keys::CTRL_F }, &my_right));
   _incremental_search_cords.push_back(KeyCord({ Control::keys::CTRL_A }, &my_begin_line));
   _incremental_search_cords.push_back(KeyCord({ Control::keys::CTRL_E }, &my_end_line));
   _incremental_search_cords.push_back(KeyCord({ Control::keys::CTRL_B }, &my_left));
   _incremental_search_cords.push_back(KeyCord({ Control::keys::BACKSPACE }, &my_backspace));
   _incremental_search_cords.push_back(KeyCord({ Control::keys::CTRL_D }, &my_delete));
   //incremental_search_cords.push_back(KeyCord({ Control::keys::TAB }, &my_file_select_tab));
   IncrementalSearch is;
   auto incremental_search_enter = bind(&IncrementalSearch::my_incremental_search_enter, ref(is), placeholders::_1, placeholders::_2, placeholders::_3);
   _incremental_search_cords.push_back(KeyCord({ Control::keys::ENTER }, incremental_search_enter));
   auto incremental_search_quit = bind(&IncrementalSearch::my_incremental_search_quit, ref(is), placeholders::_1, placeholders::_2, placeholders::_3);
   _incremental_search_cords.push_back(KeyCord({ Control::keys::CTRL_G }, incremental_search_quit));
   auto incremental_search_again = bind(&IncrementalSearch::my_incremental_search_again, ref(is), placeholders::_1, placeholders::_2, placeholders::_3);
   _incremental_search_cords.push_back(KeyCord({ Control::keys::CTRL_S }, incremental_search_again));
   shared_ptr<Model> incremental_search_model = make_shared<Model>();
   shared_ptr<CursesView> incremental_search_view = make_shared<CursesView>(-2, 3, 1, -2);
   shared_ptr<CursesControl> incremental_search_control = make_shared<CursesControl>(incremental_search_model);
   auto incremental_search_insert = bind(&IncrementalSearch::my_incremental_search_insert, ref(is), placeholders::_1, placeholders::_2, placeholders::_3, placeholders::_4);
   shared_ptr<Mode> incremental_search_mode = make_shared<FundamentalMode>(_incremental_search_cords, incremental_search_control, incremental_search_insert);
   incremental_search_control->add_mode(Control::MAJOR_MODE, incremental_search_mode);

   _command_line_cords.push_back(KeyCord({ Control::keys::LEFT }, &my_empty));
   _command_line_cords.push_back(KeyCord({ Control::keys::RIGHT }, &my_empty));
   _command_line_cords.push_back(KeyCord({ Control::keys::DOWN }, &my_empty));
   _command_line_cords.push_back(KeyCord({ Control::keys::UP }, &my_empty)); 
   shared_ptr<Model> command_line_model = make_shared<Model>();
   shared_ptr<CursesView> command_line_view = make_shared<CursesView>(-2, 3, 1, -2); 
   shared_ptr<CursesControl> command_line_control = make_shared<CursesControl>(command_line_model);
   CommandLine cl;
   auto command_line_insert = bind(&CommandLine::my_command_line_insert, ref(cl), placeholders::_1, placeholders::_2, placeholders::_3, command_line_control, command_line_model); 
   shared_ptr<Mode> command_line_mode = make_shared<FundamentalMode>(_command_line_cords, command_line_control, command_line_insert);
   command_line_control->add_mode(Control::MAJOR_MODE, command_line_mode);
   shared_ptr<Model> m = make_shared<Model>(input_filename);
   //vector<KeyCord> cords;
   _main_cords.push_back(KeyCord({ Control::keys::LEFT }, &my_empty));
   _main_cords.push_back(KeyCord({ Control::keys::RIGHT }, &my_empty));
   _main_cords.push_back(KeyCord({ Control::keys::DOWN }, &my_empty));
   _main_cords.push_back(KeyCord({ Control::keys::UP }, &my_empty));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_F }, &my_right));
   _main_cords.push_back(KeyCord({ Control::keys::ALTF }, &word_right));
   _main_cords.push_back(KeyCord({ Control::keys::ALTB }, &word_left));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_A }, &my_begin_line));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_E }, &my_end_line));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_N }, &down));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_P }, &up));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_B }, &my_left));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_X, Control::keys::CTRL_C }, &my_exit));
   _main_cords.push_back(KeyCord({ Control::keys::BACKSPACE }, &my_backspace));
   _main_cords.push_back(KeyCord({ Control::keys::ENTER }, &my_enter));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_D }, &my_delete));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_X, Control::keys::CTRL_S }, &my_save));
   auto open_file = bind(my_open_file, placeholders::_1, placeholders::_2, placeholders::_3, file_select_control, file_select_model, file_select_view);
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_X, Control::keys::CTRL_F }, open_file));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_K }, &my_kut_line));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_Y }, &my_paste_from_clipboard));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_SPACE }, &my_start_selection));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_G }, &my_ctrl_g));
   _main_cords.push_back(KeyCord({ Control::keys::ALTW }, &my_copy_selection));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_W }, &my_cut_selection));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_UNDERSCORE }, &my_undo));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_X, 'u' }, &my_undo));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_X, 'r' }, &my_redo));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_X, '(' }, &my_start_recording));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_X, ')' }, &my_stop_recording));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_X, 'e' }, &my_execute_recording));
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_V }, &my_page_down));
   _main_cords.push_back(KeyCord({ Control::keys::ALTV }, &my_page_up));
   _main_cords.push_back(KeyCord({ Control::keys::ALTX}, &my_command_line_insert));
   shared_ptr<CursesView> main_view = make_shared<CursesView>(-2, -4, 1, 1);

   shared_ptr<CursesControl> main_control = make_shared<SoftwrapControl>(m, main_view);
   shared_ptr<Mode> latex_mode = make_shared<LatexMode>(_main_cords, main_control, &my_insert);
   shared_ptr<Mode> ism_mode = make_shared<IncrementalSearchMode>(_main_cords, main_control, &my_insert);
   //const KeyCords& keys, std::shared_ptr<Control> control, std::shared_ptr<Model> model, std::shared_ptr<View> view, KeyCord::insert_func insert
   shared_ptr<Mode> selection_mode = make_shared<SelectionMode>(_main_cords, main_control, m, main_view, &my_insert);
   shared_ptr<TabsMode> tabs_mode = make_shared<TabsMode>(_main_cords, main_control, &my_insert); 
   auto incremental_search = bind(&IncrementalSearch::my_incremental_search, ref(is), placeholders::_1, placeholders::_2, placeholders::_3, incremental_search_control, incremental_search_model, incremental_search_view);
   _main_cords.push_back(KeyCord({ Control::keys::CTRL_S }, incremental_search)); 
   main_control->add_mode(Control::TABS_MODE, tabs_mode);
   assign_mode_based_on_extension(m, main_control);
   main_control->add_mode(Control::INCREMENTAL_MODE, ism_mode);
   main_control->add_mode(Control::SELECTION_MODE, selection_mode);
   //main_control->loop();
   loop(m, main_view, main_control);
   incremental_search_control->exit();
   file_select_control->exit();
   main_view.reset(new DummyView);
   file_select_view.reset(new DummyView);
   incremental_search_view.reset(new DummyView);
   command_line_view.reset(new DummyView);
   return (0);
}

