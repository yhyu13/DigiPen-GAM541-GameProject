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
