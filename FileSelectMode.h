#pragma once
#include "Mode.h"

class FileSelectMode : public Mode
{
public:
	FileSelectMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert);
};