/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Event.h
Purpose			: Contains definition for event struct.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 02/09/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/Entity.h"

namespace gswy {

	struct BaseEvent {

		BaseEvent() = default;
		
		virtual ~BaseEvent() {
		}
	};
	
	template <typename EntityType, typename EventType>
	struct Event: BaseEvent {

		Event() = default;
		explicit Event(EventType type)
			:m_type(type)
		{}

		explicit Event(const Event& other): m_type(other.m_type) {		
		}

		virtual ~Event() {
		}

		EventType m_type;
	};
}

template <typename EntityType, typename EventType>
std::ostream& operator<<(std::ostream& os, const gswy::Event<EntityType, EventType>& dt)
{
	os << '(' << dt.m_type << ") ";
	return os;
}