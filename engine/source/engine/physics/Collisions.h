/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Collisions.h
Purpose			: Contains declarations for Shapes and Collisions.
Author			: Taksh Goyal (taksh.goyal@digipen.edu | 60001319)
Creation date	: 02/11/2020
- End Header ----------------------------*/

#pragma once
//#include "..\..\application\source\ecs\components\BodyCom.h"

namespace gswy
{
	class Shape
	{
	public:
		enum class SHAPE_TYPE
		{
			CIRCLE = 0,
			AABB,
			NUM
		};
		//Shape() = default;
		Shape(SHAPE_TYPE type);
		virtual ~Shape() {}

		SHAPE_TYPE mType;

	};

	class Circle : public Shape
	{
	public:
		Circle();
		~Circle();

		float GetRadius()
		{
			return m_Radius;
		}

		void SetRadius(float radius)
		{
			m_Radius = radius;
		}

	private:
		float m_Radius;

	};

	class AABB : public Shape
	{
	public:
		AABB();
		~AABB();

		float GetWidth()
		{
			return m_Width;
		}

		float GetHeight()
		{
			return m_Height;
		}

		void SetWidth(float width)
		{
			m_Width = width;
		}

		void SetHeight(float height)
		{
			m_Height = height;
		}

	private:
		float m_Width, m_Height;
	};

	bool AABBCollisions(Shape* AABB1, float PosX1, float PosY1,
		Shape* AABB2, float PosX2, float PosY2);

	bool CircleCollisions(Shape* Circle1, float PosX1, float PosY1,
		Shape* Circle2, float PosX2, float PosY2);

	bool CircleAABBCollisions(Shape* Circle1, float PosX1, float PosY1,
		Shape* AABB2, float PosX2, float PosY2);

	bool AABBCircleCollisions(Shape* AABB1, float PosX1, float PosY1,
		Shape* Circle2, float PosX2, float PosY2);

	bool CircleReflection(Shape* Circle1, float PosX1, float PosY1,
		Shape* Circle, float PosX2, float PosY2);

	bool ResolveCollision(Shape* AABB1, float PosX1, float PosY1,
		Shape* AABB2, float PosX2, float PosY2);

	class Collisions
	{
	public:

		static Collisions* GetInstance()
		{
			static Collisions instance;
			return &instance;
		}
		bool CheckCollisionAndGenerateDetection(Shape* pShape1, float PosX1, float PosY1,
			Shape* pShape2, float PosX2, float PosY2);

		bool (*CollisionFunctions[(unsigned int)Shape::SHAPE_TYPE::NUM][(unsigned int)Shape::SHAPE_TYPE::NUM])
			(Shape* pShape1, float PosX1, float PosY1,
				Shape* pShape2, float PosX2, float PosY2);
	private:
		Collisions();
	};

}