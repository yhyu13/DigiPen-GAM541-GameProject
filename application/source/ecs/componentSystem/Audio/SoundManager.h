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
#include "ecs/componentSystem/Audio/SoundComSys.h"

namespace gswy
{
	class SoundManager : public BaseComponentSystem<GameObjectType>
	{
		SoundManager() 
		{
		
		}

		void operator= (const SoundManager&) = delete;

		SoundManager(const SoundManager&) = delete;

	public:


		~SoundManager()
		{

		}

		static SoundManager* GetInstance() 
		{
			static SoundManager input;
			return &input;
		}

		void CallForMuteSFX(bool play)
		{
			auto e = MemoryManager::Make_shared<OnMuteSFXEvent>(play);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Publish(e);
		}

		void CallForMuteBGM(bool play)
		{
			auto e = MemoryManager::Make_shared<OnMuteBGMEvent>(play);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Publish(e);
		}
	};
}
