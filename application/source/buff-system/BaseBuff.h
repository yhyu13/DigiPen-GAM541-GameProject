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
#pragma once
#include "ecs/EntityType.h"
#include "BuffType.h"

namespace gswy {

	template <typename EntityType>
	class GameWorld;

	template<typename EntityType>
	struct Entity;

	/*
		Buffs with -1 duration are permenant
	*/
	class BaseBuff 
	{
	public:
		explicit BaseBuff(GameBuffType type, double duration) : m_type(type), m_duration(duration){};
		virtual ~BaseBuff() {};
		virtual void ApplyBuff(GameWorld<GameObjectType>* world,Entity<GameObjectType>& entity, double timeRemaning) = 0;

	public:
		GameBuffType m_type;
		double m_duration;
	};

	class ModifySpeedPercentBuff : public BaseBuff
	{
	public:
		explicit ModifySpeedPercentBuff(float value, double duration) 
			: 
			BaseBuff(GameBuffType::CHANGE_SPEED_PERCENT, duration),
			m_value(value)
		{};
		virtual ~ModifySpeedPercentBuff() {};
		virtual void ApplyBuff(GameWorld<GameObjectType>* world, Entity<GameObjectType>& entity, double timeRemaning) override;
	public:
		float m_value;
	};

	class ModifySpeedPointBuff : public BaseBuff
	{
	public:
		explicit ModifySpeedPointBuff(float value, double duration)
			: 
			BaseBuff(GameBuffType::CHANGE_SPEED_POINT, duration),
			m_value(value)
		{};
		virtual ~ModifySpeedPointBuff() {};
		virtual void ApplyBuff(GameWorld<GameObjectType>* world, Entity<GameObjectType>& entity, double timeRemaning) override;
	public:
		float m_value;
	};

	class ModifyHPPercentBuff : public BaseBuff
	{
	public:
		explicit ModifyHPPercentBuff(float value, double duration) 
			: 
			BaseBuff(GameBuffType::CHANGE_HP_PERCENT, duration), 
			m_value(value)
		{};
		virtual ~ModifyHPPercentBuff() {};
		virtual void ApplyBuff(GameWorld<GameObjectType>* world, Entity<GameObjectType>& entity, double timeRemaning) override;
	public:
		float m_value;
	};

	class ModifyHPPointBuff : public BaseBuff
	{
	public:
		explicit ModifyHPPointBuff(float value, double duration) 
			: 
			BaseBuff(GameBuffType::CHANGE_HP_POINT, duration),
			m_value(value)
		{};
		virtual ~ModifyHPPointBuff() {};
		virtual void ApplyBuff(GameWorld<GameObjectType>* world, Entity<GameObjectType>& entity, double timeRemaning) override;
	public:
		float m_value;
	};
}
