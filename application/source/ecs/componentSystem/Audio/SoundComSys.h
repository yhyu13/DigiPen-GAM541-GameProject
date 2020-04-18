/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Taksh Goyal (taksh.goyal@digipen.edu | 60001319)
Creation date: 02/17/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/audio/AudioManager.h"
#include "ecs/components/TransformCom.h"
#include "ecs/CustomEvents.h"
#include "ecs/EntityType.h"
#include "ecs/componentSystem/Audio/SoundManager.h"

namespace gswy
{
	class SoundComSys : public BaseComponentSystem<GameObjectType> {
		bool m_muteSFX = {false};
		bool m_muteBGM = {false};
	public:
		
		SoundComSys() 
		{
		}

		virtual void Init() override 
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<SoundComSys>(this, EventType::SOUND, &SoundComSys::OnPLAYSOUND);
			queue->Subscribe<SoundComSys>(this, EventType::COLLISION, &SoundComSys::CollisionPLAYSOUND);
			queue->Subscribe<SoundComSys>(this, EventType::WEAPON_SOUND, &SoundComSys::WeaponPLAYSOUND);
			queue->Subscribe<SoundComSys>(this, EventType::MUTE_SFX, &SoundComSys::OnMuteSFX);
			queue->Subscribe<SoundComSys>(this, EventType::MUTE_BGM, &SoundComSys::OnMuteBGM);
		}

		virtual void Update(double dt) override
		{
		}

		void OnPLAYSOUND(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (!m_muteSFX)
			{
				auto audio = AudioManager::GetInstance();
				if (auto event = static_pointer_cast<SoundEvent>(e))
				{
					audio->PlaySound(event->soundName, AudioVector3(event->m_location), event->m_vol, event->m_freq);
				}
			}
		}

		void OnMuteBGM(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = static_pointer_cast<OnMuteBGMEvent>(e))
			{
				AudioManager::GetInstance()->SetSoundMute("Track_1", event->m_mute);
				AudioManager::GetInstance()->SetSoundMute("Menu_Track", event->m_mute);
			}
		}

		void OnMuteSFX(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = static_pointer_cast<OnMuteSFXEvent>(e))
			{
				m_muteSFX = event->m_mute;
			}
		}

		void WeaponPLAYSOUND(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (!m_muteSFX)
			{
				auto audio = AudioManager::GetInstance();
				if (auto event = static_pointer_cast<WeaponSoundEvent>(e))
				{
					auto playing = audio->PlaySound(event->soundName, AudioVector3(event->m_location), event->m_vol, event->m_freq);
				}
			}
		}

		void CollisionPLAYSOUND(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (!m_muteSFX)
			{
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto audio = AudioManager::GetInstance();
				if (auto event = static_pointer_cast<CollisionEvent>(e))
				{
					bool isEnemyA = std::find(g_enemyTypes.begin(), g_enemyTypes.end(), event->m_entityA.m_type) != g_enemyTypes.end();
					bool isEnemyB = std::find(g_enemyTypes.begin(), g_enemyTypes.end(), event->m_entityB.m_type) != g_enemyTypes.end();
					if ((isEnemyA &&
						event->m_entityB.m_type == GameObjectType::FIREBALL) ||
						(isEnemyB &&
							event->m_entityA.m_type == GameObjectType::FIREBALL))
					{
						auto e1 = MemoryManager::Make_shared<WeaponSoundEvent>("fireball_hit1", GetComponent<TransformCom>(event->m_entityA)->GetPos(),-5);

						if (!audio->IsPlaying(e1->soundName))
						{
							queue->Publish(e1);
						}
					}
					else if ((isEnemyA &&
						event->m_entityB.m_type == GameObjectType::BOLT_STRIKE) ||
						(isEnemyB &&
							event->m_entityA.m_type == GameObjectType::BOLT_STRIKE))
					{
						auto e1 = MemoryManager::Make_shared<WeaponSoundEvent>("lightning_shoot", GetComponent<TransformCom>(event->m_entityA)->GetPos(),10);

						if (!audio->IsPlaying(e1->soundName))
						{
							queue->Publish(e1);
						}
					}
					else if ((isEnemyA &&
						event->m_entityB.m_type == GameObjectType::ICEBALL) ||
						(isEnemyB &&
						event->m_entityA.m_type == GameObjectType::ICEBALL))
					{
						auto e1 = MemoryManager::Make_shared<WeaponSoundEvent>("ice_hit1", GetComponent<TransformCom>(event->m_entityA)->GetPos(),-5);

						if (!audio->IsPlaying(e1->soundName))
						{
							queue->Publish(e1);
						}
					}
					else if ((isEnemyA &&
						event->m_entityB.m_type == GameObjectType::RAZOR) ||
						(isEnemyB &&
						event->m_entityA.m_type == GameObjectType::RAZOR))
					{
						auto e1 = MemoryManager::Make_shared<WeaponSoundEvent>("razor_hit", GetComponent<TransformCom>(event->m_entityA)->GetPos(),-10);

						if (!audio->IsPlaying(e1->soundName))
						{
							queue->Publish(e1);
						}
					}
					else if ((isEnemyA &&
						event->m_entityB.m_type == GameObjectType::CYCLONE_SFX) ||
						(isEnemyB &&
						event->m_entityA.m_type == GameObjectType::CYCLONE_SFX))
					{
						auto e1 = MemoryManager::Make_shared<WeaponSoundEvent>("razor_hit", GetComponent<TransformCom>(event->m_entityA)->GetPos(),-10);

						if (!audio->IsPlaying(e1->soundName))
						{
							queue->Publish(e1);
						}
					}
				}
			}
		}
	};
}