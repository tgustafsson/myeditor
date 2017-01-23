#pragma once

#include "Model.h"
#include "View.h"
#include "Control.h"

void loop(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
void assign_mode_based_on_extension(std::shared_ptr<Model> model, std::shared_ptr<Control> control);
void update_view(std::shared_ptr<Model> model, std::shared_ptr<View> view, std::shared_ptr<Control> control);
