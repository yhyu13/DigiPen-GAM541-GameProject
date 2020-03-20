#include "Items.h"

namespace gswy
{

	Items::Items() : m_size(0)
	{
	}

	Items::~Items()
	{
	}

	void Items::Add(std::shared_ptr<Item> item)
	{
		item->m_id = m_size++;
		m_items.push_back(item);
		m_itemIdToIndexMapping[m_size] = m_size;
	}

	std::shared_ptr<Item> Items::GetItem(const int& id)
	{
		int index = m_itemIdToIndexMapping[id];
		return m_items.at(index);
	}

	std::vector<std::shared_ptr<Item>> Items::GetItems()
	{
		return m_items;
	}

}