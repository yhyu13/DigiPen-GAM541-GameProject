/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/29/2020
- End Header ----------------------------*/

#include "BaseBuff.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/components/BodyCom.h"
#include "ecs/components/HitPointCom.h"

void gswy::ModifySpeedPercentBuff::ApplyBuff(GameWorld<GameObjectType>* world, Entity<GameObjectType>& entity, double timeRemaning, double dt)
{
	ComponentDecorator<BodyCom, GameObjectType> body;
	world->Unpack(entity, body);
	body->SetVelocity(body->GetVelocity() * m_value);
}

void gswy::ModifySpeedPointBuff::ApplyBuff(GameWorld<GameObjectType>* world, Entity<GameObjectType>& entity, double timeRemaning, double dt)
{
	ComponentDecorator<BodyCom, GameObjectType> body;
	world->Unpack(entity, body);
	body->SetVelocity(body->GetVelocity() + m_value);
}

void gswy::ModifyHPPercentBuff::ApplyBuff(GameWorld<GameObjectType>* world, Entity<GameObjectType>& entity, double timeRemaning, double dt)
{
	ComponentDecorator<HitPointCom, GameObjectType> hp;
	world->Unpack(entity, hp);
	hp->AddHitPoint(hp->GetMaxHP() * m_value * dt);
}

void gswy::ModifyHPPointBuff::ApplyBuff(GameWorld<GameObjectType>* world, Entity<GameObjectType>& entity, double timeRemaning, double dt)
{
	ComponentDecorator<HitPointCom, GameObjectType> hp;
	world->Unpack(entity, hp);
	hp->AddHitPoint(m_value * dt);
}


