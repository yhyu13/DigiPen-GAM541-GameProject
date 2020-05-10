/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: CS562
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 05/08/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "PerspectiveCamera.h"

gswy::PerspectiveCamera::PerspectiveCamera(float fovy, float aspectRatio, float nearZ, float farZ)
	:
	m_ProjectionMatrix(glm::perspective(fovy, aspectRatio, nearZ, farZ)), 
	m_ViewMatrix(1.0f)
{
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void gswy::PerspectiveCamera::SetProjection(float fovy, float aspectRatio, float nearZ, float farZ)
{
	m_ProjectionMatrix = glm::perspective(fovy, aspectRatio, nearZ, farZ);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}

void gswy::PerspectiveCamera::RecalculateViewMatrix()
{
	Mat4 transform = glm::translate(Mat4(1.0f), m_Position) * glm::toMat4(m_Rotation) * glm::translate(Mat4(1.0f), Vec3f(0,0,m_Zoom));
	m_ViewMatrix = glm::inverse(transform);
	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
}
