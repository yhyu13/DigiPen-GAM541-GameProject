/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/24/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/CustomEvents.h"
#include "ecs/components/BodyCom.h"

namespace gswy
{
	class CoinControllerComSys : public BaseComponentSystem<GameObjectType> {
	private:
		int m_coinsValue = { 2 };
	public:
		CoinControllerComSys() {
		}

		virtual void Init() override
		{
			Json::Value root;
			std::ifstream file("./asset/archetypes/levels/sample-level.json", std::ifstream::binary);
			file >> root;
			file.close();
			Json::Value items = root["data"];
			m_coinsValue = items["coins_per_mob"].asInt();

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<CoinControllerComSys>(this, EventType::ADD_COIN, &CoinControllerComSys::OnAddCoin);
		}

		void OnAddCoin(EventQueue<GameObjectType, EventType>::EventPtr event)
		{
			auto addCoinEvent = std::static_pointer_cast<AddCoinEvent>(event);

			auto coin = m_parentWorld->GenerateEntity(GameObjectType::COIN);
			auto active = ActiveCom();
			coin.AddComponent(active);

			auto player = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
			coin.AddComponent(OwnershiptCom<GameObjectType>(player));

			auto transform = TransformCom(addCoinEvent->m_enemyPosition.x, addCoinEvent->m_enemyPosition.y, Z_ORDER(500));
			coin.AddComponent(transform);

			auto animCom = AnimationCom();
			animCom.Add("coinAnimation", "Move");
			animCom.SetCurrentAnimationState("Move");
			coin.AddComponent(animCom);

			auto sprite = SpriteCom();
			sprite.SetScale(vec2(0.1, 0.1));
			coin.AddComponent(sprite);

			auto aabb = BodyCom();
			aabb.SetPos(transform.GetPos());
			aabb.m_overrideFriction = true;
			aabb.ChooseShape("Circle", 0.1);

			auto targetBody = GetComponent<BodyCom>(player);
			aabb.SetVelocity(glm::normalize(targetBody->GetPos() - aabb.GetPos()) * 1.5f);

			coin.AddComponent(aabb);
		}

		virtual void Update(double dt) override
		{
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto coins = m_parentWorld->GetAllEntityWithType(GameObjectType::COIN);
			auto player = m_parentWorld->GetAllEntityWithType(GameObjectType::PLAYER)[0];
			auto playerBodyCom = GetComponent<BodyCom>(player);
			for (auto& coin : coins)
			{
				auto bodyCom = GetComponent<BodyCom>(coin);
				auto delta = playerBodyCom->GetPos() - bodyCom->GetPos();

				if (glm::length(delta) <= 0.2)
				{
					auto e = MemoryManager::Make_shared<GCEvent>(coin);
					queue->Publish(e, 0.10);
					// Add coins to player
					GameLevelMapManager::GetInstance()->AddCoins(m_coinsValue);
				}
				else
				{
					bodyCom->SetVelocity(glm::normalize(delta) * 1.5f);
				}
				
			}
		}
	};
}