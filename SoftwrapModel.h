#pragma once

#include "Model.h"
#include "SoftwrapAttributedString.h"
#include <map>

class SoftwrapModel : public Model
{
private:
	size_t m_width;
	std::vector<std::shared_ptr<SoftwrapAttributedString>> m_softwrap;
	std::map<size_t, size_t> m_map_soft_to_real;
	void wrap_content();
public:
	SoftwrapModel() : Model(), m_width(78) {}
	SoftwrapModel(std::wstring path) : Model(path), m_width(80) {}
	virtual void load();
	virtual std::vector<std::shared_ptr<AttributedString>> rows(intptr_t start = -1, intptr_t stop = -1);
	virtual size_t number_of_lines() const;
	virtual std::shared_ptr<AttributedString> get_row(size_t row);
	virtual void insert(size_t row, const std::wstring& content);
	virtual void erase(size_t row);
	virtual void load(std::wstring path);
	//virtual SearchPos search(intptr_t row, intptr_t col, intptr_t view_row, intptr_t view_col, std::wstring search_term);
	//virtual void set_start_selection(size_t row, size_t col);
};