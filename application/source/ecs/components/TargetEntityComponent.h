/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: TargetEntityComponent.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/28/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/Entity.h"
#include "ecs/EntityType.h"

namespace gswy
{

	struct TargetEntityComponent : public BaseComponent<TargetEntityComponent>
	{

		TargetEntityComponent(): m_target(nullptr)
		{
		}

		~TargetEntityComponent()
		{
		}

		std::shared_ptr<Entity<GameObjectType>> m_target;
	};
}
