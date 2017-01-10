#pragma once

#include "Control.h"

class CursesControl : public Control
{
public:
	CursesControl(std::shared_ptr<Model> m) : Control(m) {
	}
	virtual std::tuple<Control::keys, int> get_key() const;
	//virtual void loop();
};
