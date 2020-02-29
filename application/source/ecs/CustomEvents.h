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

		CollisionEvent(const Entity<GameObjectType>& entity1, const Entity<GameObjectType>& entity2)
			:
			Event(EventType::COLLISION)
		{
			m_entityA = entity1;
			m_entityB = entity2;
		}
		Entity<GameObjectType> m_entityA;
		Entity<GameObjectType> m_entityB;
	};

	struct FireWeaponEvent : Event<GameObjectType, EventType> {

		FireWeaponEvent(const Entity<GameObjectType>& entity)
			:
			Event(EventType::FIREWEAPON)
		{
			m_entity = entity;
		}

		Entity<GameObjectType> m_entity;
	};

	struct SoundEvent : Event<GameObjectType, EventType> {

		SoundEvent(const std::string& name)
			:
			Event(EventType::SOUND)
		{
			soudName = name;
		}

		std::string soudName;
	};

	struct DeathEvent : Event<GameObjectType, EventType> {

		DeathEvent(const Entity<GameObjectType>& entity)
			:
			Event(EventType::DEATH)
		{
			m_entity = entity;
		}

		Entity<GameObjectType> m_entity;
	};

	struct GCEvent : Event<GameObjectType, EventType> {
		
		GCEvent(const Entity<GameObjectType>& entity)
			:
			Event(EventType::GC)
		{
			m_entity = entity;
		}

		Entity<GameObjectType> m_entity;
	};

	struct SpawnEvent : Event<GameObjectType, EventType> {

		explicit SpawnEvent(GameObjectType type, const vec3& pos)
			:
			Event(EventType::SPAWN),
			m_type(type),
			m_pos(pos)
		{
		}

		vec3 m_pos;
		GameObjectType m_type;
	};

}