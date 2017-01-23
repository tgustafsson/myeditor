#include "FundamentalMode.h"
#include "Debug.h"
using namespace std;

FundamentalMode::FundamentalMode(const KeyCords& keys, std::shared_ptr<Control> control, KeyCord::insert_func insert) : Mode(keys, control, insert) {}

vector<shared_ptr<AttributedString>> FundamentalMode::syntax_highlight(vector<shared_ptr<AttributedString>> rows)
{
   return rows;
}