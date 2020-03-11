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
			// Init collision diable map
			for (size_t i = 0; i < (size_t)GameObjectType::NUM; ++i)
				for (size_t j = 0; j < (size_t)GameObjectType::NUM; ++j)
					m_CollisionDisableMap[i][j] = 0;

			// Collision diable list (self-self, mutal)
			GameObjectType diableCollisionList[] = { GameObjectType::PLAYER ,GameObjectType::FIREBALL ,GameObjectType::ICEBALL ,GameObjectType::BOLT };
			for (auto& item1 : diableCollisionList)
			{
				for (auto& item2 : diableCollisionList)
				{
					m_CollisionDisableMap[(size_t)item1][(size_t)item2] = 1;
				}
			}
		}
//#ifdef _DEBUG
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
					ComponentDecorator<BodyCom, GameObjectType> body;
					ComponentDecorator<TransformCom, GameObjectType> transform;
					m_parentWorld->Unpack(entity, body);
					m_parentWorld->Unpack(entity, transform);

					auto s = body->shape;
					if (auto aabb = dynamic_pointer_cast<AABB>(s))
					{
						Renderer2D::DrawDebugQuad(glm::vec3(body->m_PosX, body->m_PosY, 0), glm::vec2(aabb->GetWidth(), aabb->GetHeight()), transform->GetRotation(), glm::vec4(1.0f));
					}
					else if (auto circle = dynamic_pointer_cast<Circle>(s))
					{
						Renderer2D::DrawDebugQuad(glm::vec3(body->m_PosX, body->m_PosY, 0), glm::vec2(circle->GetRadius(), circle->GetRadius()), transform->GetRotation(), glm::vec4(1.0f));
					}
				}
				unlock();
			}
		}
//#endif // _DEBUG

		

		virtual void Update(double dt) override
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			//For Collisions
			auto collision = Collisions::GetInstance();
			auto first_Entity = m_registeredEntities.begin();
			auto last_Entity = m_registeredEntities.end();
			for (; first_Entity != last_Entity; ++first_Entity)
			{
				ComponentDecorator<BodyCom, GameObjectType> body1;
				m_parentWorld->Unpack(*first_Entity, body1);

				for (auto second_Entity = first_Entity + 1; second_Entity != last_Entity; ++second_Entity)
				{
					// Enable collision diable map
					if (m_CollisionDisableMap[(size_t)first_Entity->m_type][(size_t)second_Entity->m_type])
						continue;

					ComponentDecorator<BodyCom, GameObjectType> body2;
					m_parentWorld->Unpack(*second_Entity, body2);

					bool collides = collision->CheckCollisionAndGenerateDetection(body1->shape.get(), body1->m_PosX, body1->m_PosY, body2->shape.get(), body2->m_PosX, body2->m_PosY);						
					if (collides)
					{
						ComponentDecorator<OwnershiptCom<GameObjectType>, GameObjectType> owner1;
						m_parentWorld->Unpack(*first_Entity, owner1);
						ComponentDecorator<OwnershiptCom<GameObjectType>, GameObjectType> owner2;
						m_parentWorld->Unpack(*second_Entity, owner2);
						DEBUG_PRINT("Collisions Detected " + Str(*first_Entity) + Str(*second_Entity));
						auto e = MemoryManager::Make_shared<CollisionEvent>(*first_Entity, *second_Entity);
						queue->Publish(e);
					}
				}
			}

			//Body And Transform Updates
			for (auto& entity : m_registeredEntities)
			{
				ComponentDecorator<TransformCom, GameObjectType> transform;
				ComponentDecorator<BodyCom, GameObjectType> body;
				m_parentWorld->Unpack(entity, transform);
				m_parentWorld->Unpack(entity, body);

				transform->AddPos(transform->GetVelocity() * (float)dt);

				body->m_PosX = transform->GetPos().x;
				body->m_PosY = transform->GetPos().y;
			}
		}
	};
}