#pragma once

#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/EntityType.h"
#include "ecs/components/TargetEntityComponent.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/BodyCom.h"

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
			std::vector<Entity<GameObjectType>> enemies = m_parentWorld->GetAllEntityWithType({ GameObjectType::ENEMY_1, GameObjectType::ENEMY_2, GameObjectType::ENEMY_BOSS_1 });
			for (auto entity : m_registeredEntities)
			{
				auto targetEntityComponent = GetComponent<TargetEntityComponent>(entity);
				if (targetEntityComponent->m_target == nullptr)
				{
					if (enemies.size() > 0)
					{
						int i = rand() % (enemies.size() - 1);
						Entity<GameObjectType> randomEntity = enemies.at(i);
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
				if (targetEntityComponent->m_target == nullptr)
				{
					continue;
				}

				std::shared_ptr<Entity<GameObjectType>> target = targetEntityComponent->m_target;
				auto targetTransform = GetComponent<TransformCom>(*target);
				auto targetBody = GetComponent<BodyCom>(*target);

				auto entityTransform = GetComponent<TransformCom>(entity);
				auto entityBody = GetComponent<BodyCom>(entity);

				glm::vec2 direction = targetBody->GetPos() - entityBody->GetPos();
				glm::vec2 unitDirection = glm::normalize(direction);
				glm::vec2 velocityDirection = glm::normalize(entityBody->GetVelocity());

				float rotation = -glm::cross(glm::vec3(unitDirection, 0.0f), glm::vec3(velocityDirection, 0.0f)).z;
				glm::mat4 rotationMatrix;
				rotationMatrix = glm::mat4(1.0f);
				rotationMatrix = glm::rotate(rotationMatrix, rotation, glm::vec3(0.0f, 0.0f, 1.0f));
				entityBody->SetVelocity(glm::vec2(rotationMatrix * glm::vec4(entityBody->GetVelocity(), 0.0f, 0.0f)));

				/*float dot = glm::dot(entityBody->GetVelocity(), edgeVector);
				float denominator = glm::length(entityBody->GetVelocity()) * glm::length(edgeVector);
				float cosOfAngle = dot / denominator;
				float angle = acosf(cosOfAngle);

				glm::vec2 normalToEntity = glm::vec2(-entityBody->GetVelocity().y, entityBody->GetVelocity().x);
				if (glm::dot(normalToEntity, edgeVector) < 0)
				{
					angle = -angle;
				}

				float angleOfRotation = (3.1415926f / 2.0f) * dt;
				if (angle > angleOfRotation)
				{
					angle = angleOfRotation;
				}
				else if (angle < -angleOfRotation)
				{
					angle = -angleOfRotation;
				}

				entityTransform->SetRotation(entityTransform->GetRotation() + angle);
				glm::mat4 rotationMatrix;
				rotationMatrix = glm::mat4(1.0f);
				rotationMatrix = glm::rotate(rotationMatrix, entityTransform->GetRotation(), glm::vec3(0.0f, 0.0f, 1.0f));

				entityBody->SetVelocity(glm::vec2(rotationMatrix * glm::vec4(entityBody->GetVelocity(), 0.0f, 0.0f)));

				APP_CRITICAL("velocity: {0} {1}", entityBody->GetVelocity().x, entityBody->GetVelocity().y);*/
			}

		}

		bool IsTargetAlreadyLocked(Entity<GameObjectType> target)
		{
			for (auto entity : m_registeredEntities)
			{
				auto targetEntityComponent = GetComponent<TargetEntityComponent>(entity);
				if (targetEntityComponent->m_target != nullptr && targetEntityComponent->m_target->m_id == target.m_id)
				{
					return true;
				}
			}
			return false;
		}
	};


}