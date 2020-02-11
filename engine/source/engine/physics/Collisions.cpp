/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Collisions.cpp
Purpose			: Contains implementations for Shapes and Collisions declarations in header file.
Author			: Taksh Goyal (taksh.goyal@digipen.edu | 60001319)
Creation date	: 02/11/2020
- End Header ----------------------------*/

#include "Collisions.h"
#include <math.h>
#include "glm/glm.hpp"

using namespace gswy;

Shape::Shape(SHAPE_TYPE Type) : mType(Type)
{

}

Circle::Circle() : Shape(SHAPE_TYPE::CIRCLE)
{
	m_Radius = 0.0f;
}

Circle::~Circle()
{

}

AABB::AABB() : Shape(SHAPE_TYPE::AABB)
{
	m_Width = m_Height = 0.0f;
}

AABB::~AABB()
{

}

bool gswy::CircleCollisions(Circle* Circle1, float PosX1, float PosY1, 
	Circle* Circle2, float PosX2, float PosY2) 
{
	//Collision Math And Logic
	float CCDistSq, Radius1, Radius2;
	Radius1 = Circle1->GetRadius();
	Radius2 = Circle2->GetRadius();

	CCDistSq = pow((PosX2 - PosX1), 2) + pow((PosY2 - PosY1), 2);

	//Actual Collision test
	float dissq = pow((Radius1 + Radius2), 2);
	if (CCDistSq > dissq)
		return false;

	return true;
}

bool gswy::AABBCollisions(AABB* AABB1, float PosX1, float PosY1,
	AABB* AABB2, float PosX2, float PosY2)
{
	//Collision Math And Logic
	AABB* pAABB1 = (AABB*)AABB1;
	AABB* pAABB2 = (AABB*)AABB2;

	glm::vec2 tl0, br0, tl1, br1;

	//Rectangle 1
	tl0.x = PosX1 - (pAABB1->GetWidth() / 2);
	tl0.y = PosY1 + (pAABB1->GetHeight() / 2);
	br0.x = PosX1 + (pAABB1->GetWidth() / 2);
	br0.y = PosY1 - (pAABB1->GetHeight() / 2);

	//Rectangle 2
	tl1.x = PosX2 - (pAABB2->GetWidth() / 2);
	tl1.y = PosY2 + (pAABB2->GetHeight() / 2);
	br1.x = PosX2 + (pAABB2->GetWidth() / 2);
	br1.y = PosY2 - (pAABB2->GetHeight() / 2);

	//Actual Collision Test
	if (br0.x < tl1.x || tl0.y < br1.y ||
		br1.x < tl0.x || br0.y > tl1.y)
		return false;

	return true;
}


bool gswy::CircleAABBCollisions(Circle* Circle1, float PosX1, float PosY1,
	AABB* AABB2, float PosX2, float PosY2)
{
	//Collision Math And Logic
	float Radius1;
	Radius1 = Circle1->GetRadius();

	Circle* pCircle1 = (Circle*)Circle1;
	AABB* pAABB2 = (AABB*)AABB2;

	//Half Extents for reaching the edge point from center
	glm::vec2 AABB_HalfExtents(pAABB2->GetWidth() / 2, pAABB2->GetHeight() / 2);
	glm::vec2 Circle_Center(PosX1/* + Radius1*/, PosY1/* + Radius1*/);
	glm::vec2 AABB_Center(PosX2/* + AABB_HalfExtents.x*/, PosY2/* + AABB_HalfExtents.y*/);

	glm::vec2 difference = Circle_Center - AABB_Center; 
	glm::vec2 clamped = clamp(difference, -AABB_HalfExtents, AABB_HalfExtents);

	glm::vec2 edgepoint = AABB_Center + clamped;

	difference = edgepoint - Circle_Center;

	if (glm::length(difference) <= Radius1)
		return true;

	return false;
}

bool gswy::AABBCircleCollisions(AABB* AABB1, float PosX1, float PosY1,
	Circle* Circle2, float PosX2, float PosY2)
{
	//Collision Math And Logic
	float Radius2;
	Radius2 = Circle2->GetRadius();

	Circle* pCircle2 = (Circle*)Circle2;
	AABB* pAABB1 = (AABB*)AABB1;

	//Half Extents for reaching the edge point from center
	glm::vec2 AABB_HalfExtents(pAABB1->GetWidth() / 2, pAABB1->GetHeight() / 2);
	glm::vec2 Circle_Center(PosX2/* + Radius2*/, PosY2/* + Radius2*/);
	glm::vec2 AABB_Center(PosX1/* + AABB_HalfExtents.x*/, PosY1/* + AABB_HalfExtents.y*/);

	glm::vec2 difference = Circle_Center - AABB_Center;
	glm::vec2 clamped = clamp(difference, -AABB_HalfExtents, AABB_HalfExtents);

	glm::vec2 edgepoint = AABB_Center + clamped;

	difference = edgepoint - Circle_Center;

	if (glm::length(difference) <= Radius2)
		return true;

	return false;
}

bool CheckCollisionAndGenerateContact(Shape* pShape1, float PosX1, float PosY1,
	Shape* pShape2, float PosX2, float PosY2)
{
	//return CollisionFunctions[(unsigned int)pShape1->mType][(unsigned int)pShape2->mType](pShape1, PosX1, PosY1, pShape2, PosX2, PosY2);
	return false;
}
