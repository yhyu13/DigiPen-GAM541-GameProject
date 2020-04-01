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
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/audio/AudioManager.h"
#include "ecs/CustomEvents.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class SoundComSys : public BaseComponentSystem<GameObjectType> {
	public:
		std::vector<GameObjectType> enemyTypes;

		SoundComSys() 
		{
			enemyTypes = { GameObjectType::ENEMY_1, GameObjectType::ENEMY_2, GameObjectType::ENEMY_BOSS_1 };
		}

		virtual void Init() override 
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<SoundComSys>(this, EventType::SOUND, &SoundComSys::OnPLAYSOUND);
			queue->Subscribe<SoundComSys>(this, EventType::COLLISION, &SoundComSys::CollisionPLAYSOUND);
			queue->Subscribe<SoundComSys>(this, EventType::WEAPON_SOUND, &SoundComSys::WeaponPLAYSOUND);
		}

		void OnPLAYSOUND(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto audio = AudioManager::GetInstance();
			if (auto event = static_pointer_cast<SoundEvent>(e))
			{
				if (!audio->IsPlaying(event->soudName))
				{
					audio->PlaySound(event->soudName, AudioVector3{ 0, 0, 0 }, 1, 1.0);
				}	
			}
			
		}

		void WeaponPLAYSOUND(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto audio = AudioManager::GetInstance();
			auto event = static_pointer_cast<WeaponSoundEvent>(e);
			auto playing = audio->PlaySound(event->soundName, AudioVector3{ 0, 0, 0 }, 1, 1.0);
		}

		void CollisionPLAYSOUND(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto audio = AudioManager::GetInstance();
			auto event = static_pointer_cast<CollisionEvent>(e);

			if ((std::find(enemyTypes.begin(), enemyTypes.end(), event->m_entityA.m_type) != enemyTypes.end() &&
				event->m_entityB.m_type == GameObjectType::FIREBALL) ||
				(std::find(enemyTypes.begin(), enemyTypes.end(), event->m_entityB.m_type) != enemyTypes.end() &&
				event->m_entityA.m_type == GameObjectType::FIREBALL))
			{
				auto e1 = MemoryManager::Make_shared<WeaponSoundEvent>("fireball_hit1");

				if(!audio->IsPlaying(e1->soundName))
					queue->Publish(e1);
			}
			else
			if ((std::find(enemyTypes.begin(), enemyTypes.end(), event->m_entityA.m_type) != enemyTypes.end() &&
				event->m_entityB.m_type == GameObjectType::ICEBALL) ||
				(std::find(enemyTypes.begin(), enemyTypes.end(), event->m_entityB.m_type) != enemyTypes.end() &&
				event->m_entityA.m_type == GameObjectType::ICEBALL))
			{
				auto e1 = MemoryManager::Make_shared<WeaponSoundEvent>("ice_hit1");

				if (!audio->IsPlaying(e1->soundName))
					queue->Publish(e1);
			}
		}
	};
}