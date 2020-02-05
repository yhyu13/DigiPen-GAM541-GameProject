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
#include "engine/input/Input.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/SpriteCom.h"

namespace gswy
{
	class SceneComSys : public BaseComponentSystem {
	public:
		SceneComSys() {
			m_systemSignature.AddComponent<SpriteCom>();
		}

		virtual void Render() override
		{
			for (auto& entity : m_registeredEntities) {
				ComponentDecorator<SpriteCom> sprite;
				m_parentWorld->Unpack(entity, sprite);
				auto m_ControlSprite = sprite->Get();
				m_ControlSprite->Draw();
			}
		}
	};
}