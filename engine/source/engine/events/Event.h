#pragma once

#include "engine/ecs/Entity.h"

namespace gswy {

	struct BaseEvent {
	};
	
	template <typename EntityType, typename EventType>
	struct Event: BaseEvent {
	
		EventType m_type;

		Entity<EntityType> m_entityA;
		Entity<EntityType> m_entityB;
	};

}
