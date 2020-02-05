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

namespace gswy {

	/*
		The entities are just the data containers. Each entity gets an
		id. Entities and their components are held together by component
		managers.
		
		For more information on component-managers, please check
		ComponentManager.h.
	*/
	struct Entity {

		unsigned int m_id;

		Entity(unsigned int id) {
			m_id = id;
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