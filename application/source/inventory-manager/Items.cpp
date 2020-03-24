#include "Items.h"

namespace gswy
{
	int Items::count = 0;

	Items::Items() : m_supportSkillCount(0)
	{
	}

	Items::~Items()
	{
	}

	void Items::Add(std::shared_ptr<Item> item)
	{
		item->m_id = count++;
		if (item->m_category._Equal("ACTIVE"))
		{
			m_activeItems.push_back(item);
		}
		else if (item->m_category._Equal("SUPPORT"))
		{
			m_supportItems.push_back(item);
			std::vector<std::string> tags = item->m_tags;
			for (int i = 0; i < tags.size(); ++i)
			{
				m_tagsAndSupportSkills[tags.at(i)].push_back(m_supportSkillCount);
			}
			++m_supportSkillCount;
		}
	}

	std::vector<std::shared_ptr<Item>> Items::GetActiveItems()
	{
		return m_activeItems;
	}

	std::vector<std::shared_ptr<Item>> Items::GetSupportItems()
	{
		return m_supportItems;
	}

	std::set<std::shared_ptr<Item>> Items::GetSupportItems(std::shared_ptr<Item> activeItem)
	{
		std::set<std::shared_ptr<Item>> qualifiedItems;

		std::vector<std::string> tags = activeItem->m_tags;
		for (int i = 0; i < tags.size(); ++i)
		{
			std::vector<int> supportSkillIndexes = m_tagsAndSupportSkills[tags.at(i)];
			for (int j = 0; j < supportSkillIndexes.size(); ++j)
			{
				qualifiedItems.insert(m_supportItems.at(supportSkillIndexes.at(j)));
			}
		}

		return qualifiedItems;
	}
}