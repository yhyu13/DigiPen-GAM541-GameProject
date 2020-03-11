/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/10/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/CustomEvents.h"

namespace gswy
{
	class FadeComSys : public BaseComponentSystem<GameObjectType> {
	public:
		FadeComSys() {
		}

		virtual void Init() override {
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<FadeComSys>(this, EventType::FADE, &FadeComSys::OnFADE);
			queue->Subscribe<FadeComSys>(this, EventType::_FADE_ING, &FadeComSys::OnFADE_ING);
			queue->Subscribe<FadeComSys>(this, EventType::_FADE_END, &FadeComSys::OnFADE_END);
		}

		void OnFADE(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (auto event = static_pointer_cast<FadeEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));
				auto entity = event->m_entity;

				ComponentDecorator<SpriteCom, GameObjectType> Sprite;
				m_parentWorld->Unpack(entity, Sprite);

				Sprite->Get()->SetSpriteAlpha;
			}
		}

		void OnFADE_ING(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (auto event = static_pointer_cast<_FadeIngEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));

			}
		}

		void OnFADE_END(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (auto event = static_pointer_cast<_FadeEndEvent>(e))
			{
				DEBUG_PRINT("Receive " + Str(*e));

			}
		}
	};
}