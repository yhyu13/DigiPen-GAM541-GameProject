/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: BodyCom.h
Purpose			: Contains implementations for Body related Operations including Gravity, Body Position, Velocity, Mass etc...
Author			: Taksh Goyal (taksh.goyal@digipen.edu | 60001319)
Creation date	: 02/12/2020
- End Header ----------------------------*/

#pragma once
#include "engine/ecs/BaseComponent.h"
#include "TransformCom.h"
#include "EngineExport.h"

namespace gswy
{
	struct BodyCom : BaseComponent<BodyCom>
	{
		BodyCom() = default;
		BodyCom(float posx, float posy)
			:m_PosX(posx), m_PosY(posy), m_Mass(0), m_AccX(0), m_AccY(0),
			 m_VelX(0), m_VelY(0), m_PrevPosX(0), m_PrevPosY(0), m_InvMass(0),
			 m_TotalForceX(0), m_TotalForceY(0),m_Restitution(0)
		{
		};

		BodyCom& CopyBodyCom(const BodyCom& bodycom)
		{
			if (this != &bodycom)
			{
				m_PosX = bodycom.m_PosX;
				m_PosY = bodycom.m_PosY;
				m_Mass = bodycom.m_Mass;
				m_AccX = bodycom.m_AccX;
				m_AccY = bodycom.m_AccY;
				m_VelX = bodycom.m_VelX;
				m_VelY = bodycom.m_VelY;
				m_InvMass = bodycom.m_InvMass;
				m_PrevPosX = bodycom.m_PrevPosX;
				m_PrevPosY = bodycom.m_PrevPosY;
				m_TotalForceX = bodycom.m_TotalForceX;
				m_TotalForceY = bodycom.m_TotalForceY;
				m_Restitution = bodycom.m_Restitution;
				shape = bodycom.shape;
			}
			return *this;
		}

		float m_PosX;
		float m_PosY;
		float m_PosZ;
		float m_Mass;
		float m_AccX;
		float m_AccY;
		float m_VelX;
		float m_VelY;
		float m_PrevPosX;
		float m_PrevPosY;
		float m_InvMass;
		float m_TotalForceX;
		float m_TotalForceY;
		float m_Restitution;
		std::shared_ptr<Shape> shape;
		Entity<GameObjectType> m_otherEntity;

		void ResetOtherEntity()
		{
			m_otherEntity.m_type = GameObjectType::EMPTY;
		}

		void SetOtherEntity(const Entity<GameObjectType>& other)
		{
			m_otherEntity = other;
		}

		const Entity<GameObjectType>& GetOtherEntity()
		{
			return m_otherEntity;
		}

		//Choose Shape, fill in width and height for the body
		void ChooseShape(const std::string& name, float width, float height) 
		{
			if (0 == name.compare("AABB"))
			{
				shape = std::make_shared<AABB>();
				static_pointer_cast<AABB>(shape)->SetWidth(width);
				static_pointer_cast<AABB>(shape)->SetHeight(height);
			}
			else
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Incorrect Shape Type, should be: AABB, instead: " + str2wstr(name) + L", Try Again!");
		}

		//Choose Shape, leave with 1 less argument to activate below method
		void ChooseShape(const std::string& name, float radius)
		{
			if (0 == name.compare("Circle"))
			{
				shape = std::make_shared<Circle>();
				static_pointer_cast<Circle>(shape)->SetRadius(radius);
			}
			else
				throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Incorrect Shape Type, should be: Circle, instead: " + str2wstr(name) + L", Try Again!");
		}

		void SetVelocity(float VelX, float VelY)
		{
			m_VelX = VelX;
			m_VelY = VelY;
		}

		void SetVelocity(glm::vec2& velocity)
		{
			m_VelX = velocity.x;
			m_VelY = velocity.y;
		}

		void SetPos3D(const glm::vec3& pos)
		{
			m_PosX = pos.x;
			m_PosY = pos.y;
			m_PosZ = pos.z;
		}

		void SetRestitution(float rest)
		{
			m_Restitution = rest;
		}

		void SetMass(float mass)
		{
			m_Mass = mass;
		}

		void SetPos(const vec2& v)
		{
			m_PosX = v.x;
			m_PosY = v.y;
		}

		const vec2& GetPos() const
		{
			return vec2(m_PosX, m_PosY);
		}

		const vec2& GetVelocity() const
		{
			return vec2(m_VelX, m_VelY);
		}

		//void Serialize() {}

		void Integrate(/*float Gravity, */float dt)
		{
			//m_AccX = m_AccY = 0.0f;

			//dt = dt / 1000.0f;

			m_PrevPosX = m_PosX;
			m_PrevPosY = m_PosY;

			if (m_Mass == 0)
				m_InvMass = 0;
			else
				m_InvMass = 1 / m_Mass;

			//Applying Gravity
			//float g = m_Mass * Gravity;
			//m_TotalForceY = /* m_AddedForceY * */g;

			//Apply Acceleration
			//m_AccX = m_TotalForceX * m_InvMass;
			//m_AccY = m_TotalForceY * m_InvMass;

			//Change in Position
			m_PosX = m_VelX * dt + m_PrevPosX;
			m_PosY = m_VelY * dt + m_PrevPosY;

			//Simulating Friction
			m_VelX *= 0.99;
			m_VelY *= 0.99;

			//Nullifying All Forces To Activate with Press of a button
			//m_TotalForceX = m_TotalForceY = 0.0f;
		}
	};
}