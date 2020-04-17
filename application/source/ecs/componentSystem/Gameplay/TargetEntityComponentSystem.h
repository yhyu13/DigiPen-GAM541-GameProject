/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: TargetEntityComponentSystem.h
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
Creation date	: 03/28/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/EntityType.h"
#include "ecs/components/TargetEntityComponent.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/BodyCom.h"
#include "ecs/components/LifeTimeCom.h"
#include "ecs/components/HitPointCom.h"

#include <glm/glm.hpp>
#include <glm/gtx/perpendicular.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace gswy
{

	class TargetEntityComponentSystem : public BaseComponentSystem<GameObjectType>
	{

	public:

		TargetEntityComponentSystem()
		{
			m_systemSignature.AddComponent<TargetEntityComponent>();
		}

		~TargetEntityComponentSystem()
		{
		}

		virtual void Init() override
		{
		}

		virtual void Update(double dt) override
		{
			// Lock a random enemy as target
			std::vector<Entity<GameObjectType>> enemies = m_parentWorld->GetAllEntityWithType(g_enemyTypes);

			for (auto& entity : m_registeredEntities)
			{
				auto targetEntityComponent = GetComponent<TargetEntityComponent>(entity);
				if (targetEntityComponent->m_target == nullptr)
				{
					if (!enemies.empty())
					{
						auto randomEntity = enemies[RAND_I(0,enemies.size())];
						if (!IsTargetAlreadyLocked(randomEntity))
						{
							targetEntityComponent->m_target = MemoryManager::Make_shared<Entity<GameObjectType>>(randomEntity);
						}
					}
				}
			}

			// update the movement
			for (auto entity : m_registeredEntities)
			{
				auto targetEntityComponent = GetComponent<TargetEntityComponent>(entity);
				if (targetEntityComponent->m_target == nullptr) // if a target could not be locked
				{
					auto entityBody = GetComponent<BodyCom>(entity);
					float minDistance = 3;
					for (auto& enemy : enemies)
					{
						auto enemyBody = GetComponent<BodyCom>(enemy);
						float distance = glm::length(enemyBody->GetPos() - entityBody->GetPos());
						if (distance < minDistance)
						{
							minDistance = distance;
							targetEntityComponent->m_target = std::make_shared<Entity<GameObjectType>>(enemy);
						}
					}
				}

				if (targetEntityComponent->m_target == nullptr) // if a target could not be locked
				{
					continue;
				}

				std::shared_ptr<Entity<GameObjectType>> target = targetEntityComponent->m_target;
				if (std::find(enemies.begin(), enemies.end(), *target) != enemies.end())
				{
					auto targetBody = GetComponent<BodyCom>(*target);
					auto entityBody = GetComponent<BodyCom>(entity);

					glm::vec2 direction = targetBody->GetPos() - entityBody->GetPos();
					glm::vec2 unitDirection = glm::normalize(direction);
					glm::vec2 velocityDirection = glm::normalize(entityBody->GetVelocity());

					float rotation = -glm::cross(glm::vec3(unitDirection, 0.0f), glm::vec3(velocityDirection, 0.0f)).z;
					glm::mat4 rotationMatrix;
					rotationMatrix = glm::mat4(1.0f);
					rotationMatrix = glm::rotate(rotationMatrix, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
					entityBody->SetVelocity(glm::vec2(rotationMatrix * glm::vec4(entityBody->GetVelocity(), 0.0f, 0.0f)));
				}
				else
				{
					// target is already dead
					targetEntityComponent->m_target.reset();
					targetEntityComponent->m_target = nullptr;
				}
			}
		}

		bool IsTargetAlreadyLocked(Entity<GameObjectType> target)
		{
			for (auto& entity : m_registeredEntities)
			{
				auto targetEntityComponent = GetComponent<TargetEntityComponent>(entity);
				if (targetEntityComponent->m_target)
				{
					if (targetEntityComponent->m_target->m_id == target.m_id)
					{
						return true;
					}
				}
			}
			return false;
		}
	};


}