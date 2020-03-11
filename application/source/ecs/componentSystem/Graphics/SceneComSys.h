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
#include "ecs/components/SpriteCom.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class SceneComSys : public BaseComponentSystem<GameObjectType> {
	public:
		SceneComSys() {
			m_systemSignature.AddComponent<SpriteCom>();
		}

		virtual void Render(double dt) override
		{
			lock();
			for (auto& entity : m_registeredEntities) {

				ComponentDecorator<SpriteCom, GameObjectType> sprite;
				m_parentWorld->Unpack(entity, sprite);
				/* Drawing sprites */
				sprite->Get()->Draw();
			}
			unlock();
		}
	};
}