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
	public:
		PhysicsComSys()
		{
			m_systemSignature.AddComponent<BodyCom>();
			m_systemSignature.AddComponent<TransformCom>();
			m_systemSignature.AddComponent<OwnershiptCom<GameObjectType>>();
		}

		virtual void Render() override
		{
			static bool debugDraw = false;
			if (InputManager::GetInstance()->IsKeyTriggered(GLFW_KEY_F1))
			{
				debugDraw = !debugDraw;
			}
			if (debugDraw)
			{
				for (auto& entity : m_registeredEntities)
				{
					ComponentDecorator<BodyCom, GameObjectType> body;
					m_parentWorld->Unpack(entity, body);
					Renderer2D::DrawDebugQuad(glm::vec3(body->m_PosX, body->m_PosY, 0), glm::vec2(0.2f), 0, glm::vec4(1.0f));
				}
			}
		}

		virtual void Update(double dt) override
		{
			//Body And Transform Updates
			for (auto& entity : m_registeredEntities)
			{
				ComponentDecorator<TransformCom, GameObjectType> transform;
				ComponentDecorator<BodyCom, GameObjectType> body;
				m_parentWorld->Unpack(entity, transform);
				m_parentWorld->Unpack(entity, body); 
				body->m_PosX = transform->GetPos().x;
				body->m_PosY = transform->GetPos().y;
			}

			//For Collisions
			auto collision = Collisions::GetInstance();
			if (m_registeredEntities.empty() == false)
			{
				auto first_Entity = m_registeredEntities.begin();
				auto last_Entity = m_registeredEntities.end();
				for (; first_Entity != last_Entity; ++first_Entity)
				{
					ComponentDecorator<BodyCom, GameObjectType> body1;
					m_parentWorld->Unpack(*first_Entity, body1);

					for (auto second_Entity = first_Entity + 1; second_Entity != last_Entity; ++second_Entity)
					{
						ComponentDecorator<BodyCom, GameObjectType> body2;
						m_parentWorld->Unpack(*second_Entity, body2);

						bool collides = collision->CheckCollisionAndGenerateDetection(body1->shape.get(), body1->m_PosX, body1->m_PosY, body2->shape.get(), body2->m_PosX, body2->m_PosY);
						
						if (collides)
						{
							ComponentDecorator<OwnershiptCom<GameObjectType>, GameObjectType> owner1;
							m_parentWorld->Unpack(*first_Entity, owner1);
							ComponentDecorator<OwnershiptCom<GameObjectType>, GameObjectType> owner2;
							m_parentWorld->Unpack(*second_Entity, owner2);
							PRINT("Collisions Detected " + Str(owner1->GetEntity()) + Str(owner2->GetEntity()));
						}
					}

				}

			}
		}
	};
}