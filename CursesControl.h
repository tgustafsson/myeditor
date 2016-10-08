#pragma once

#include "Control.h"

class CursesControl : public Control
{
public:
	CursesControl(std::shared_ptr<Model> m, std::shared_ptr<View> v) : Control(m, v) {
	}
	virtual std::tuple<Control::keys, int> get_key() const;
	virtual void loop();
};
