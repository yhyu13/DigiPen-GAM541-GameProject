/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Entity.h
Purpose			: Contains declarations for entity struct.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/02/2020
- End Header ----------------------------*/

#pragma once
#include <iostream>

namespace gswy {

	/*
		The entities are just the data containers. Each entity gets an
		id. Entities and their components are held together by component
		managers.
		
		For more information on component-managers, please check
		ComponentManager.h.
	*/
	template<typename EntityType>
	struct Entity {

		unsigned int m_id;
		EntityType m_type;

		Entity(unsigned int id, EntityType type): m_id(id), m_type(type) {
		}

		Entity() = default;
		Entity(const Entity& other) = default;

		friend inline bool operator==(const Entity& lhs, const Entity& rhs) {
			return lhs.m_id == rhs.m_id;
		}

		friend inline bool operator<(const Entity& lhs, const Entity& rhs) {
			return lhs.m_id < rhs.m_id;
		}
	};

}

template<typename EntityType>
std::ostream& operator<<(std::ostream& os, const gswy::Entity<EntityType>& dt)
{
	os << '(' << dt.m_id << '/' << dt.m_type << ") ";
	return os;
}