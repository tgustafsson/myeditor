#pragma once

#include "Model.h"
#include "View.h"
#include "Control.h"
#include "TabsMode.h"

void loop(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control, std::shared_ptr<TabsMode> tabsmode);
void assign_mode_based_on_extension(std::shared_ptr<Model> model, std::shared_ptr<Control> control);
