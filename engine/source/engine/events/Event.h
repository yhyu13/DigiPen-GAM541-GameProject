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

		virtual ~Event() {
		}

		EventType m_type;

		Entity<EntityType> m_entityA;
		Entity<EntityType> m_entityB;
	};

}
