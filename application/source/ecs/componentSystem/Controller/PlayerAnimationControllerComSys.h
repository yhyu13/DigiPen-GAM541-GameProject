/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/04/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/audio/AudioManager.h"
#include "engine/input/InputManager.h"
#include "ecs/components/AnimationCom.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class PlayerAnimationControllerComSys : public BaseComponentSystem<GameObjectType> {
	public:
		PlayerAnimationControllerComSys() {
		}

		virtual void Update(double dt) override {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto audio = AudioManager::GetInstance();

			auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
				{
					ComponentDecorator<AnimationCom, GameObjectType> animation;
					m_parentWorld->Unpack(entity, animation);
					return;
				}
		}
	};
}