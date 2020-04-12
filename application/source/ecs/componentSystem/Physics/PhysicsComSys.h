/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: PhysicsComSys.h
Purpose			: Contains Implementations for Managing Physics in the Game.
Author			: Taksh Goyal (taksh.goyal@digipen.edu | 60001319)
Creation date	: 02/14/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/components/BodyCom.h"
#include "ecs/components/TransformCom.h"
#include "ecs/components/OwnershiptCom.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class PhysicsComSys : public BaseComponentSystem<GameObjectType>
	{

	private:
		size_t m_CollisionDisableMap[(size_t)GameObjectType::NUM][(size_t)GameObjectType::NUM];

	public:
		PhysicsComSys()
		{
			m_systemSignature.AddComponent<BodyCom>();
			m_systemSignature.AddComponent<TransformCom>();
			m_systemSignature.AddComponent<OwnershiptCom<GameObjectType>>();
		}

		virtual void Init() override
		{
			// Init collision disable map
			for (size_t i = 0; i < (size_t)GameObjectType::NUM; ++i)
				for (size_t j = 0; j < (size_t)GameObjectType::NUM; ++j)
					m_CollisionDisableMap[i][j] = 0;

			// Collision disable list (self-self, mutal)
			GameObjectType disableCollisionList[] = { 
				GameObjectType::PLAYER ,GameObjectType::FIREBALL ,
				GameObjectType::ICEBALL ,GameObjectType::BOLT_STRIKE,
				GameObjectType::TOWER_BUILD, GameObjectType::TOWER_FIRE,
				GameObjectType::TOWER_ICE, GameObjectType::TOWER_LIGHTNING,
				GameObjectType::FORKED_FIREBALL, GameObjectType::RAZOR
			};

			for (auto& item1 : disableCollisionList)
			{
				for (auto& item2 : disableCollisionList)
				{
					m_CollisionDisableMap[(size_t)item1][(size_t)item2] = 1;
				}
			}
		}


#ifdef _DEBUG
		virtual void PostRenderUpdate(double dt) override
		{
			static bool debugDraw = false;
			if (InputManager::GetInstance()->IsKeyTriggered(GLFW_KEY_F1))
			{
				debugDraw = !debugDraw;
			}
			if (debugDraw)
			{
				lock();
				for (auto& entity : m_registeredEntities)
				{
					// Check active
					ComponentDecorator<ActiveCom, GameObjectType> active;
					m_parentWorld->Unpack(entity, active);
					if (!active->IsActive())
					{
						continue;
					}

					ComponentDecorator<BodyCom, GameObjectType> body;
					ComponentDecorator<TransformCom, GameObjectType> transform;
					m_parentWorld->Unpack(entity, body);
					m_parentWorld->Unpack(entity, transform);

					auto s = body->shape;
					if (auto aabb = dynamic_pointer_cast<AABB>(s))
					{
						Renderer2D::DrawDebugQuad(glm::vec3(body->m_PosX, body->m_PosY, 0),
							glm::vec2(aabb->GetWidth(), aabb->GetHeight()), 
							transform->GetRotation(), glm::vec4(1.0f));
					}
					else if (auto circle = dynamic_pointer_cast<Circle>(s))
					{
						Renderer2D::DrawDebugQuad(glm::vec3(body->m_PosX, body->m_PosY, 0),
							glm::vec2(circle->GetRadius(), circle->GetRadius()), 
							transform->GetRotation(), glm::vec4(1.0f));
					}
				}
				unlock();
			}
		}
#endif // _DEBUG

		virtual void Update(double dt) override
		{
			// Disable collision check if dt=0(game is paused)
			if (!dt)
			{
				return;
			}

			//Updating Positions
			for (auto& entity : m_registeredEntities)
			{
				ComponentDecorator<BodyCom, GameObjectType> body;
				ComponentDecorator<TransformCom, GameObjectType> transform;
				m_parentWorld->Unpack(entity, transform);
				m_parentWorld->Unpack(entity, body);

				body->Integrate(dt);

				//After Initialising, Setting Transform of each -
				//entity back from manipulated body positions for rendering
				transform->SetPos(body->GetPos());
				transform->SetVelocity(body->GetVelocity());
			}

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			//For Collisions
			auto collision = Collisions::GetInstance();
			vector<Entity<GameObjectType>> new_(m_registeredEntities);
			auto first_Entity = new_.begin();
			auto last_Entity = new_.end();
			for (; first_Entity != last_Entity; ++first_Entity)
			{
				// Check active
				ComponentDecorator<ActiveCom, GameObjectType> active;
				m_parentWorld->Unpack(*first_Entity, active);
				if (!active->IsActive())
				{
					continue;
				}

				ComponentDecorator<BodyCom, GameObjectType> body1;
				m_parentWorld->Unpack(*first_Entity, body1);

				// Reset colliding entity
				body1->ResetOtherEntity();
				for (auto second_Entity = first_Entity + 1; second_Entity != last_Entity; ++second_Entity)
				{
					// Check active
					ComponentDecorator<ActiveCom, GameObjectType> active;
					m_parentWorld->Unpack(*second_Entity, active);
					if (!active->IsActive())
					{
						continue;
					}

					// Enable collision diable map
					if (m_CollisionDisableMap[(size_t)first_Entity->m_type][(size_t)second_Entity->m_type])
					{
						continue;
					}
						
					if ((std::find(g_enemyTypes.begin(), g_enemyTypes.end(), first_Entity->m_type) != g_enemyTypes.end()) &&
						(std::find(g_enemyTypes.begin(), g_enemyTypes.end(), second_Entity->m_type) != g_enemyTypes.end()))
					{
						continue;
					}

					ComponentDecorator<BodyCom, GameObjectType> body2;
					m_parentWorld->Unpack(*second_Entity, body2);
					ComponentDecorator<TransformCom, GameObjectType> transform;
					m_parentWorld->Unpack(*second_Entity, transform);

					// Reset colliding entity
					body2->ResetOtherEntity();

					if (body1->shape && body2->shape)
					{
						bool collides = collision->CheckCollisionAndGenerateDetection
						(body1->shape.get(), body1->m_PosX, body1->m_PosY,
							body2->shape.get(), body2->m_PosX, body2->m_PosY);

						if (collides)
						{
							// Set colliding entity
							body1->SetOtherEntity(*second_Entity);
							body2->SetOtherEntity(*first_Entity);
							DEBUG_PRINT("Collisions Detected " + Str(*first_Entity) + Str(*second_Entity));
							auto e = MemoryManager::Make_shared<CollisionEvent>(*first_Entity, *second_Entity, body2->GetPos(), transform->GetRotation());
							queue->Publish(e);
						}
					}
				}
			}
		}
	};
}