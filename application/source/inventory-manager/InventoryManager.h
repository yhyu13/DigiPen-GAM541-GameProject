/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: InventoryManager.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/20/2020
- End Header ----------------------------*/

#pragma once

#include "Items.h"

namespace gswy
{

	class InventoryManager
	{

	public:

		static InventoryManager* GetInstance();
		~InventoryManager();

		void ReLoadInventory(const std::string& filepath);
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
