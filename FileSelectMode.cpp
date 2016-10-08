#include "FileSelectMode.h"

FileSelectMode::FileSelectMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert) : Mode(keys, control, insert) {}

