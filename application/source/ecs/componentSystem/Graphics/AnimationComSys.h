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
#include "ecs/components/AnimationCom.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class AnimationComSys : public BaseComponentSystem<GameObjectType> {
	public:
		AnimationComSys() {
			m_systemSignature.AddComponent<AnimationCom>();
			m_systemSignature.AddComponent<SpriteCom>();
		}

		virtual void PreRenderUpdate(double dt) override {
			lock();
			for (auto& entity : m_registeredEntities) {
				ComponentDecorator<AnimationCom, GameObjectType> animation;
				ComponentDecorator<SpriteCom, GameObjectType> sprite;
				m_parentWorld->Unpack(entity, animation);
				m_parentWorld->Unpack(entity, sprite);
				auto m_sprite = sprite->Get();
				auto m_animation = animation->GetCurrentAnimation();
				if (m_animation->UpdateFrame(dt))
				{
					/* Loading current animation into sprite component*/
					auto currentFrame = m_animation->GetCurrentFrame();
					m_sprite->SetSpriteTexture(ResourceAllocator<Texture2D>::GetInstance()->Get(currentFrame->textureName));
					m_sprite->SetSpriteX(currentFrame->x);
					m_sprite->SetSpritey(currentFrame->y);
					m_sprite->SetSpriteWidth(currentFrame->width);
					m_sprite->SetSpriteHeight(currentFrame->height);
				}
			}
			unlock();
		}

	};
}