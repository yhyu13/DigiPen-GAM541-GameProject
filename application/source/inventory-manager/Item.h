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
		bool m_purchased;
		std::vector<std::string> m_text;
	};
}