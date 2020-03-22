#pragma once

#include <string>

namespace gswy
{

	struct Skill
	{
		int m_skillNumber;
		int m_slotNumber;
		std::string m_type; // FIRE or ICE
		std::string m_category; // ACTIVE or SUPPORT
		std::set<std::string> m_tags;
	};
}