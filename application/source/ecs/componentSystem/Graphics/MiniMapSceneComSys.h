/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/14/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/components/ActiveCom.h"
#include "ecs/components/MiniMapSprite.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class MiniMapSceneComSys : public BaseComponentSystem<GameObjectType> {
	public:
		MiniMapSceneComSys() {
			m_systemSignature.AddComponent<MiniMapSprite>();
		}

		virtual void PreRenderUpdate(double dt) override {
			lock();
			for (auto& entity : m_registeredEntities) {
				ComponentDecorator<TransformCom, GameObjectType> transform;
				ComponentDecorator<MiniMapSprite, GameObjectType> sprite;
				m_parentWorld->Unpack(entity, transform);
				m_parentWorld->Unpack(entity, sprite);
				auto m_ControlSprite = sprite->Get();
				/* Update sprite location */
				m_ControlSprite->SetSpritePosition(transform->GetPos3D());
			}
			unlock();
		}

		virtual void Render2(double dt) override
		{
			lock();
			for (auto& entity : m_registeredEntities) {
				// Check active
				ComponentDecorator<ActiveCom, GameObjectType> active;
				m_parentWorld->Unpack(entity, active);
				if (!active->IsActive())
				{
					continue;
				}

				ComponentDecorator<MiniMapSprite, GameObjectType> sprite;
				m_parentWorld->Unpack(entity, sprite);
				/* Drawing sprites */
				sprite->Get()->Draw();
			}
			unlock();
		}
	};
}