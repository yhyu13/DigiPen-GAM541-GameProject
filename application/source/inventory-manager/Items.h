#pragma once

#include "Item.h"

#include <vector>
#include <memory>
#include <map>
#include <set>

namespace gswy
{
	class Items
	{

	public:

		Items();
		~Items();

		void Add(std::shared_ptr<Item> item);

		std::vector<std::shared_ptr<Item>> GetActiveItems();
		std::vector<std::shared_ptr<Item>> GetSupportItems();
		std::set<std::shared_ptr<Item>> GetSupportItems(std::shared_ptr<Item> activeItem);

	private:

		int m_supportSkillCount;

		std::vector<std::shared_ptr<Item>> m_activeItems;

		std::vector<std::shared_ptr<Item>> m_supportItems;

		/*
			This map maintains mapping between a tag and the
			supporting skills that have the tag
		*/
		std::map<std::string, std::vector<int>> m_tagsAndSupportSkills;

		static int count;
	};
}