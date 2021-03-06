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
		int m_coinsBaseValue = { 2 };
		int m_coinsValue = m_coinsBaseValue;
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
			m_coinsBaseValue = items["coins_per_mob"].asInt();

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<CoinControllerComSys>(this, EventType::ADD_COIN, &CoinControllerComSys::OnAddCoin);
		}

		void OnAddCoin(EventQueue<GameObjectType, EventType>::EventPtr event)
		{
			if (auto addCoinEvent = std::static_pointer_cast<AddCoinEvent>(event))
			{
				auto coin = m_parentWorld->GenerateEntity(GameObjectType::COIN);
				auto active = ActiveCom();
				coin.AddComponent(active);

				auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::BASE)[0];
				coin.AddComponent(OwnershiptCom<GameObjectType>(entity));

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

				auto targetBody = GetComponent<BodyCom>(entity);
				aabb.SetVelocity(glm::normalize(targetBody->GetPos() - aabb.GetPos()) * 1.5f);

				coin.AddComponent(aabb);
			}
		}

		virtual void Update(double dt) override
		{
			if (!dt)
			{
				return;
			}
			SyncRegisteredEntities();
			int diff = GameLevelMapManager::GetInstance()->m_gameDifficulty;
			switch (diff)
			{
			case 0:
				m_coinsValue = (int)((float)m_coinsBaseValue * 1.5f);
				break;
			case 1:
				m_coinsValue = m_coinsBaseValue * 1;
				break;
			default:
				break;
			}

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto coins = m_parentWorld->GetAllEntityWithType(GameObjectType::COIN);
			auto entity = m_parentWorld->GetAllEntityWithType(GameObjectType::BASE)[0];
			auto playerBodyCom = GetComponent<BodyCom>(entity);
			for (auto& coin : coins)
			{
				auto bodyCom = GetComponent<BodyCom>(coin);
				auto delta = playerBodyCom->GetPos() - bodyCom->GetPos();

				if (glm::length(delta) <= 0.2)
				{
					auto e = MemoryManager::Make_shared<GCEvent>(coin);
					queue->Publish(e, 0.10);
					// Add coins
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