#pragma once

#include "Item.h"

#include <vector>
#include <memory>
#include <map>

namespace gswy
{
	class Items
	{

	public:

		Items();
		~Items();

		void Add(std::shared_ptr<Item> item);
		std::shared_ptr<Item> GetItem(const int& id);
		std::vector<std::shared_ptr<Item>> GetItems();

	private:
		int m_size;
		std::vector<std::shared_ptr<Item>> m_items;
		std::map<int, int> m_itemIdToIndexMapping;
	};
}