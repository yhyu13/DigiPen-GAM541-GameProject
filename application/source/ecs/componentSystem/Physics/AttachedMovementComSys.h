/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/18/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/CustomEvents.h"
#include "ecs/components/AttachedMovementCom.h"
#include "ecs/components/OwnershiptCom.h"
#include "ecs/components/TransformCom.h"

namespace gswy
{
	class AttachedMovementComSys : public BaseComponentSystem<GameObjectType> {
	public:
		AttachedMovementComSys() {
			m_systemSignature.AddComponent<OwnershiptCom<GameObjectType>>();
			m_systemSignature.AddComponent<AttachedMovementCom>();
			m_systemSignature.AddComponent<TransformCom>();
			m_systemSignature.AddComponent<BodyCom>();
		}

		virtual void Update(double dt) override {
			
			for (auto& entity : m_registeredEntities) {
				// Check active
				auto active = GetComponent<ActiveCom>(entity);
				if (!active->IsActive())
				{
					continue;
				}

				auto onwer = GetComponent<OwnershiptCom<GameObjectType>>(entity);

				// Check active
				active = GetComponent<ActiveCom>(onwer->GetEntity());
				if (!active->IsActive())
				{
					continue;
				}

				auto owner_transform = GetComponent<TransformCom>(onwer->GetEntity());
				auto owner_body = GetComponent<BodyCom>(onwer->GetEntity());
				auto transform = GetComponent<TransformCom>(entity);
				auto attach = GetComponent<AttachedMovementCom>(entity);
				auto body = GetComponent<BodyCom>(entity);

				if (attach->followPos)
				{
					transform->SetPos(owner_transform->GetPos() + attach->rPos);
				}
				if (attach->followRot)
				{
					transform->SetRotation(owner_transform->GetRotation() + attach->rRot);
				}
				if (attach->followVelocity)
				{
					body->SetVelocity(owner_body->GetVelocity() + attach->rVelocity);
				}
			}
		}
	};
}