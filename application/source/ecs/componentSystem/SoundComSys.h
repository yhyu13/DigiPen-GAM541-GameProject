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

namespace gswy
{
	class SoundComSys : public BaseComponentSystem<GameObjectType> {
	public:
		SoundComSys() {
		}

		virtual void Init() override {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<SoundComSys>(this, EventType::SOUND, &SoundComSys::OnPLAYSOUND);
		}

		void OnPLAYSOUND(Event<GameObjectType, EventType>* e)
		{
			auto audio = AudioManager::GetInstance();
			if (auto event = static_cast<SoundEvent*>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				if (!audio->IsPlaying(event->soudName))
				{
					audio->PlaySound(event->soudName, AudioVector3{ 0, 0, 0 }, 1, 1.0);
				}	
			}
		}
	};
}