#pragma once

#include <string>
#include <vector>

namespace gswy
{
	struct Item
	{
		Item();
		~Item();

		int m_id;
		std::string m_type;
		std::string m_category;
		std::vector<std::string> m_tags;
		int m_cost;
		bool m_purchased;
		bool m_installed;
		std::vector<std::string> m_text;
		std::string m_keyEventType;
		std::string m_icon;
		std::string m_iconGray;
	};
}
