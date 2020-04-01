#pragma once

#include "Items.h"

namespace gswy
{

	class InventoryManager
	{

	public:

		static InventoryManager* GetInstance();
		~InventoryManager();

		void LoadInventory(const std::string& filepath);
		void PurchaseItem(std::vector<std::shared_ptr<Item>> items, std::shared_ptr<Item> item);
		void PurchaseActiveItem(std::shared_ptr<Item> item);
		void PurchaseSupportItem(std::shared_ptr<Item> item);
		std::vector<std::shared_ptr<Item>> GetActiveItems();
		std::vector<std::shared_ptr<Item>> GetSupportItems();
		std::set<std::shared_ptr<Item>> GetSupportItems(std::shared_ptr<Item> item);
		void Install(const int& skillNumber, const int& slotNumber, std::shared_ptr<Item> item);

	private:

		InventoryManager();

	private:

		std::shared_ptr<Items> m_items;
	};
}
