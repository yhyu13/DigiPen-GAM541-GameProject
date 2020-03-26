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
#include "skill-system/support-skills/SupportSkill.h"
#include "skill-system/active-skills/ActiveSkill.h"
#include "skill-system/active-skills/ActiveSkillType.h"

#include <memory>

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

		explicit FireWeaponEvent(const Entity<GameObjectType>& entity, const vec2& pos = vec2(0), float rot = 0)
			:
			m_pos(pos),
			m_rot(rot),
			Event(EventType::FIREWEAPON)
		{
			m_entity = entity;
		}
		vec2 m_pos;
		float m_rot;
		Entity<GameObjectType> m_entity;
	};

	struct AttackBaseEvent : Event<GameObjectType, EventType> {

		explicit AttackBaseEvent(float damage)
			:
			m_damage(damage),
			Event(EventType::ATTACKBASE)
		{
		}
		float m_damage;
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
			Event(EventType::GC),
			m_entity(entity)
		{
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

	struct FadeEvent : Event<GameObjectType, EventType> {

		explicit FadeEvent(const Entity<GameObjectType>& entity,float start, float end, float time, EventType _event = EventType::EMPTY)
			:
			Event(EventType::FADE),
			alpha_start(start),
			alpha_end(end),
			time_span(time),
			m_entity(entity),
			event_end(_event)
		{
		}
		Entity<GameObjectType> m_entity;
		float alpha_start;
		float alpha_end;
		float time_span;
		EventType event_end;
	};

	struct _FadeIngEvent : Event<GameObjectType, EventType> {

		explicit _FadeIngEvent(const Entity<GameObjectType>& entity,float _delta, float _end, EventType _event)
			:
			Event(EventType::_FADE_ING),
			delta(_delta),
			end(_end),
			m_entity(entity),
			event_end(_event)
		{
		}

		Entity<GameObjectType> m_entity;
		float delta;
		float end;
		EventType event_end;
	};

	struct _FadeEndEvent : Event<GameObjectType, EventType> {

		explicit _FadeEndEvent(const Entity<GameObjectType>& entity, EventType _event)
			:
			Event(EventType::_FADE_END),
			m_entity(entity),
			event_end(_event)
		{
		}

		Entity<GameObjectType> m_entity;
		EventType event_end;
	};

	struct SkillUpgradeEvent : Event<GameObjectType, EventType>
	{
		explicit SkillUpgradeEvent(const ActiveSkillType& type, std::shared_ptr<SupportSkill> supportSkill): Event(EventType::SKILL_UPGRADE), m_supportSkill(supportSkill), m_activeSkillType(type)
		{
		}

		std::shared_ptr<SupportSkill> m_supportSkill;
		ActiveSkillType m_activeSkillType;
	};

	struct SkillUseEvent : Event<GameObjectType, EventType>
	{
		explicit SkillUseEvent(std::shared_ptr<ActiveSkill> skill) : Event(EventType::SKILL_USE), m_skill(skill)
		{
		}

		std::shared_ptr<ActiveSkill> m_skill;
	};

	struct SkillActivationEvent : Event<GameObjectType, EventType>
	{
		explicit SkillActivationEvent() : Event(EventType::SKILL_ACTIVATION)
		{
		}
	};

	struct SkillAdditionEvent : Event<GameObjectType, EventType>
	{
		explicit SkillAdditionEvent(std::shared_ptr<ActiveSkill> skill) : Event(EventType::SKILL_ADDITION), m_skill(skill)
		{
		}

		std::shared_ptr<ActiveSkill> m_skill;
	};

	struct SkillRemovalEvent : Event<GameObjectType, EventType>
	{
		explicit SkillRemovalEvent(const int& id) : Event(EventType::SKILL_REMOVAL), m_id(id)
		{
		}

		int m_id;
	};

	struct LoadMainMenuEvent : Event<GameObjectType, EventType>
	{
		explicit LoadMainMenuEvent() : Event(EventType::LOAD_MAIN_MENU)
		{
		}
	};
}