#pragma once

#include "Items.h"

#include <vector>
#include <string>
#include <map>
#include <memory>

namespace gswy
{

	class InventoryManager
	{

	public:

		static InventoryManager* GetInstance();
		~InventoryManager();

		void LoadInventory(const std::string& filepath);
		void Purchase(const int& id);
		std::vector<std::shared_ptr<Item>> GetItems();

	private:

		InventoryManager();

	private:

		std::shared_ptr<Items> m_items;
	};
}
