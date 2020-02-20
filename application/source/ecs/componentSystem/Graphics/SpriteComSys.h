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
#include "ecs/components/TransformCom.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class SpriteComSys : public BaseComponentSystem<GameObjectType> {
	public:
		SpriteComSys() {
			m_systemSignature.AddComponent<TransformCom>();
			m_systemSignature.AddComponent<SpriteCom>();
		}

		virtual void Update(double dt) override {
			for (auto& entity : m_registeredEntities) {
				ComponentDecorator<TransformCom, GameObjectType> position;
				ComponentDecorator<SpriteCom, GameObjectType> sprite;
				m_parentWorld->Unpack(entity, position);
				m_parentWorld->Unpack(entity, sprite);
				auto m_ControlSprite = sprite->Get();
				/* Update sprite location */
				m_ControlSprite->SetSpritePosition(position->GetPos3D());
				m_ControlSprite->SetSpriteRotation(position->GetRotation());
			}
		}
	};
}