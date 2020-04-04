#include "InventoryManager.h"
#include "skill-manager/SkillManager.h"

#include <json/json.h>
#include <fstream>

namespace gswy
{

	InventoryManager::InventoryManager(): m_items(std::make_shared<Items>())
	{
	}

	InventoryManager::~InventoryManager()
	{
	}

	InventoryManager* InventoryManager::GetInstance()
	{
		static InventoryManager instance;
		return &instance;
	}

	void InventoryManager::ReLoadInventory(const std::string& filepath)
	{
		m_items = std::make_shared<Items>();
		Json::Value root;
		std::ifstream file(filepath, std::ifstream::binary);
		file >> root;
		file.close();
		Json::Value items = root["items"];
		for (int i = 0; i < items.size(); ++i)
		{
			Json::Value itemData = items[i];
			std::shared_ptr<Item> item = std::make_shared<Item>();
			item->m_type = itemData["type"].asString();
			item->m_category = itemData["category"].asString();
			item->m_purchased = itemData["purchased"].asBool();
			item->m_cost = itemData["cost"].asInt();
			Json::Value tags = itemData["tags"];
			for (int j = 0; j < tags.size(); ++j)
			{
				item->m_tags.push_back(tags[j].asString());
			}
			Json::Value text = itemData["text"];
			for (int j = 0; j < text.size(); ++j)
			{
				item->m_text.push_back(text[j].asString());
			}
			item->m_keyEventType = itemData["key-event"].asString();
			m_items->Add(item);
		}
	}

	void InventoryManager::LoadInventory(const std::string& filepath)
	{
		Json::Value root;
		std::ifstream file(filepath, std::ifstream::binary);
		file >> root;
		file.close();
		Json::Value items = root["items"];
		for (int i = 0; i < items.size(); ++i)
		{
			Json::Value itemData = items[i];
			std::shared_ptr<Item> item = std::make_shared<Item>();
			item->m_type = itemData["type"].asString();
			item->m_category = itemData["category"].asString();
			item->m_purchased = itemData["purchased"].asBool();
			item->m_cost = itemData["cost"].asInt();
			Json::Value tags = itemData["tags"];
			for (int j = 0; j < tags.size(); ++j)
			{
				item->m_tags.push_back(tags[j].asString());
			}
			Json::Value text = itemData["text"];
			for (int j = 0; j < text.size(); ++j)
			{
				item->m_text.push_back(text[j].asString());
			}
			item->m_keyEventType = itemData["key-event"].asString();
			m_items->Add(item);
		}
	}

	void InventoryManager::PurchaseItem(std::vector<std::shared_ptr<Item>> items, std::shared_ptr<Item> item)
	{
		auto it = std::find_if(items.begin(), items.end(), [&](std::shared_ptr<Item> const& theItem)
			{
				return theItem->m_id == item->m_id;
			});

		if (it != items.end())
		{
			(*it)->m_purchased = true;
		}
	}

	void InventoryManager::PurchaseActiveItem(std::shared_ptr<Item> item)
	{
		PurchaseItem(m_items->GetActiveItems(), item);
	}

	void InventoryManager::PurchaseSupportItem(std::shared_ptr<Item> item)
	{
		PurchaseItem(m_items->GetSupportItems(), item);
	}

	std::vector<std::shared_ptr<Item>> InventoryManager::GetActiveItems()
	{
		return m_items->GetActiveItems();
	}

	std::vector<std::shared_ptr<Item>> InventoryManager::GetSupportItems()
	{
		return m_items->GetSupportItems();
	}

	std::set<std::shared_ptr<Item>> InventoryManager::GetSupportItems(std::shared_ptr<Item> item)
	{
		return m_items->GetSupportItems(item);
	}

	void InventoryManager::Install(const int& skillNumber, const int& slotNumber, std::shared_ptr<Item> item)
	{
		if (item->m_purchased)
		{
			// call skill manager to install the skill
			SkillManager* skillManager = SkillManager::GetInstance();
			skillManager->AddSkill(skillNumber, slotNumber, item);
			item->m_installed = true;
		}
	}
}