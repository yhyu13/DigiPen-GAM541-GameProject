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

#include "engine-precompiled-header.h"
#include "Collisions.h"
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

bool gswy::CircleCollisions(Shape* Circle1, float PosX1, float PosY1,
	Shape* Circle2, float PosX2, float PosY2)
{
	//Collision Math And Logic
	float CCDistSq, Radius1, Radius2;

	Radius1 = ((Circle*)Circle1)->GetRadius();
	Radius2 = ((Circle*)Circle2)->GetRadius();

	CCDistSq = pow((PosX2 - PosX1), 2) + pow((PosY2 - PosY1), 2);

	//Actual Collision test
	float dissq = pow((Radius1 + Radius2), 2);
	if (CCDistSq > dissq)
		return false;

	return true;
}

bool gswy::AABBCollisions(Shape* AABB1, float PosX1, float PosY1,
	Shape* AABB2, float PosX2, float PosY2)
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


bool gswy::CircleAABBCollisions(Shape* Circle1, float PosX1, float PosY1,
	Shape* AABB2, float PosX2, float PosY2)
{
	//Collision Math And Logic
	float Radius1;
	Radius1 = ((Circle*)Circle1)->GetRadius();

	//Circle* pCircle1 = (Circle*)Circle1;
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

bool gswy::AABBCircleCollisions(Shape* AABB1, float PosX1, float PosY1,
	Shape* Circle2, float PosX2, float PosY2)
{
	return gswy::CircleAABBCollisions(Circle2, PosX2, PosY2, AABB1, PosX1, PosY1);
	////Collision Math And Logic
	//float Radius2;
	//Radius2 = Circle2->GetRadius();

	//Circle* pCircle2 = (Circle*)Circle2;
	//AABB* pAABB1 = (AABB*)AABB1;

	////Half Extents for reaching the edge point from center
	//glm::vec2 AABB_HalfExtents(pAABB1->GetWidth() / 2, pAABB1->GetHeight() / 2);
	//glm::vec2 Circle_Center(PosX2/* + Radius2*/, PosY2/* + Radius2*/);
	//glm::vec2 AABB_Center(PosX1/* + AABB_HalfExtents.x*/, PosY1/* + AABB_HalfExtents.y*/);

	//glm::vec2 difference = Circle_Center - AABB_Center;
	//glm::vec2 clamped = clamp(difference, -AABB_HalfExtents, AABB_HalfExtents);

	//glm::vec2 edgepoint = AABB_Center + clamped;

	//difference = edgepoint - Circle_Center;

	//if (glm::length(difference) <= Radius2)
	//	return true;

	//return false;
}

bool gswy::CircleReflection(Shape* Circle1, float PosX1, float PosY1,
	Shape* Circle2, float PosX2, float PosY2)
{

	if (gswy::CircleCollisions(Circle1, PosX1, PosY1, Circle2, PosX2, PosY2))
	{
		glm::vec2 Circle1_Center(PosX1, PosY1);
		glm::vec2 Circle2_Center(PosX2, PosY2);
		glm::vec2 IntersectionPoint = (Circle1_Center * ((Circle*)Circle1)->GetRadius()
			+ Circle2_Center * ((Circle*)Circle2)->GetRadius())
			/ ( ((Circle*)Circle1)->GetRadius() + ((Circle*)Circle2)->GetRadius());

        glm::vec2 Circle1_Center_To_IntersectionPoint = Circle1_Center - IntersectionPoint;
        glm::normalize(Circle1_Center_To_IntersectionPoint);
        
        //Calculating the reflected velocity
       // glm::vec2 ReflectedVel = CurrVel - 2(CurrVel * Circle1_Center_To_IntersectionPoint)*Circle1_Center_To_IntersectionPoint;
	}
	return false;
}

bool gswy::ResolveCollision(Shape* AABB1, float PosX1, float PosY1,
	Shape* AABB2, float PosX2, float PosY2)
{
	if (gswy::AABBCollisions(AABB1, PosX1, PosY1, AABB2, PosX2, PosY2) == true)
	{
		//Calculate Relative Velocity
		//glm::vec2 RelV = AABB2->mp_OwnerBody->GetVelocity() - AABB1->mp_OwnerBody->GetVelocity();

		//Calculate relative velocity in terms of the normal direction
		//float Vel_Along_Normal = glm::dot(RelV, normal);

		//Do not resolve if velocities are separating
		//if (Vel_Along_Normal > 0)
		//	return false;

		//Calculate Restitution
		//float e = std::min(AABB1->mp_OwnerBody->GetRestitution(), AABB2->mp_OwnerBody->GetRestitution());

		//Calculate impulse Scalar
		//float j = -(1 + e) * Vel_Along_Normal;
		//j /= 1 / AABB1->mp_OwnerBody->m_Mass + 1 / AABB2->mp_OwnerBody->m_Mass;

		//Apply Impulse
		//glm::vec2 Impulse = j * normal;
		//AABB1->mp_OwnerBody->m_VelX -= 1 / AABB1->mp_OwnerBody->m_Mass * Impulse;
		//AABB1->mp_OwnerBody->m_VelY -= 1 / AABB1->mp_OwnerBody->m_Mass * Impulse;
		
		//AABB2->mp_OwnerBody->m_VelX -= 1 / AABB2->mp_OwnerBody->m_Mass * Impulse;
		//AABB2->mp_OwnerBody->m_VelY -= 1 / AABB2->mp_OwnerBody->m_Mass * Impulse;

		//For calculating Raw Mass 
		//float Mass_Sum = AABB1->mp_OwnerBody->m_Mass + AABB2->mp_OwnerBody->m_Mass;
		
		//float Ratio = AABB1->mp_OwnerBody->m_Mass / Mass_Sum;
		
		//AABB1->mp_OwnerBody->m_VelX -= Ratio * impulse;
		//AABB1->mp_OwnerBody->m_VelY -= Ratio * impulse;
		
		//Ratio = AABB2->mp_OwnerBody->m_Mass / Mass_Sum;
		
		//AABB2->mp_OwnerBody->m_VelX += Ratio * impulse;
		//AABB2->mp_OwnerBody->m_VelY += Ratio * impulse;


		return true;
	}

	return false;
}



bool Collisions::CheckCollisionAndGenerateDetection(Shape* pShape1, float PosX1, float PosY1,
	Shape* pShape2, float PosX2, float PosY2)
{
	return CollisionFunctions[(unsigned int)(pShape1->mType)][(unsigned int)(pShape2->mType)](pShape1, PosX1, PosY1, pShape2, PosX2, PosY2);
}

Collisions::Collisions()
{
	CollisionFunctions[(unsigned int)Shape::SHAPE_TYPE::CIRCLE][(unsigned int)Shape::SHAPE_TYPE::CIRCLE] = CircleCollisions;
	CollisionFunctions[(unsigned int)Shape::SHAPE_TYPE::CIRCLE][(unsigned int)Shape::SHAPE_TYPE::AABB] = CircleAABBCollisions;
	CollisionFunctions[(unsigned int)Shape::SHAPE_TYPE::AABB][(unsigned int)Shape::SHAPE_TYPE::CIRCLE] = AABBCircleCollisions;
	CollisionFunctions[(unsigned int)Shape::SHAPE_TYPE::AABB][(unsigned int)Shape::SHAPE_TYPE::AABB] = AABBCollisions;
}
