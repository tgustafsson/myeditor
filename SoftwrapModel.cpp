#include "SoftwrapModel.h"

using namespace std;

void SoftwrapModel::wrap_content()
{
    size_t i = 0;
	size_t total = 0;
	m_softwrap.clear();
	m_map_soft_to_real.clear();
	for (auto c : m_content)
	{
		size_t j = 0;
		do
		{
			m_map_soft_to_real[total] = i;
			m_softwrap.push_back(make_shared<SoftwrapAttributedString>(c, j * m_width, m_width));
			j++;
			i++;
			total++;
		} while (j * m_width < c->length());
	}
}

void SoftwrapModel::load()
{
	Model::load();
	wrap_content();
}

vector<shared_ptr<AttributedString>> SoftwrapModel::rows(intptr_t start, intptr_t stop)
{
	wrap_content();
	vector<shared_ptr<AttributedString>> ret;
	if (start == -1) {
		start = 0;
	}
	if (stop == -1) {
		stop = m_content.size() - 1;
	}
	for (intptr_t i = start; i < stop && i < m_softwrap.size(); i++) {
		ret.push_back(m_softwrap[i]);
	}
	return ret;
}

size_t SoftwrapModel::number_of_lines() const
{
	return m_softwrap.size();
}

shared_ptr<AttributedString> SoftwrapModel::get_row(size_t row)
{
	wrap_content();
	return m_softwrap[row];
}

void SoftwrapModel::insert(size_t row, const wstring& content)
{
	Model::insert(m_map_soft_to_real[row], content);
	wrap_content();
}

void SoftwrapModel::erase(size_t row)
{
	Model::erase(m_map_soft_to_real[row]);
	wrap_content();
}

void SoftwrapModel::load(wstring path)
{
	Model::load(path);
	wrap_content();
}
