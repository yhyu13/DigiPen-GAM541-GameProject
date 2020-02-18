/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/17/2020
- End Header ----------------------------*/

#pragma once
#include "ecs/EntityType.h"
#include "ecs/EventType.h"
#include "engine/events/EventQueue.h"

namespace gswy
{
	struct CollisionEvent : Event<GameObjectType, EventType> {

		Entity<GameObjectType> m_entityA;
		Entity<GameObjectType> m_entityB;
	};

}