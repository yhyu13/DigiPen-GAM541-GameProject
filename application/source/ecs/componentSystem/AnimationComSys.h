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

#include <GLFW/glfw3.h>
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/components/SpriteCom.h"
#include "ecs/components/AnimationCom.h"

namespace gswy
{
	class AnimationComSys : public BaseComponentSystem {
	public:
		AnimationComSys() {
			m_systemSignature.AddComponent<AnimationCom>();
			m_systemSignature.AddComponent<SpriteCom>();
		}

		virtual void Update(double dt) override {
			for (auto& entity : m_registeredEntities) {
				ComponentDecorator<AnimationCom> animation;
				ComponentDecorator<SpriteCom> sprite;
				m_parentWorld->Unpack(entity, animation, sprite);
				auto m_sprite = sprite->Get();
				auto m_animation = animation->Get();
				
				m_animation->UpdateFrame(dt);
				auto currentFrame = m_animation->GetCurrentFrame();
				ResourceAllocator<Texture2D>::GetInstance()->Get(currentFrame->textureName);
			}
		}
	};
}