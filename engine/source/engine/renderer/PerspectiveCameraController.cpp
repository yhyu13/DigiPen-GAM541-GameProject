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
#include "PerspectiveCameraController.h"

gswy::PerspectiveCameraController::PerspectiveCameraController()
	:
	m_Camera(m_fovy* DEG2RAD, m_aspectRatio, m_nearZ, m_farZ)
{

}

gswy::PerspectiveCameraController::PerspectiveCameraController(float fovy,float aspectRatio, float nearZ, float farZ)
	:
	m_aspectRatio(aspectRatio),
	m_fovy(fovy),
	m_nearZ(nearZ),
	m_farZ(farZ),
	m_Camera(fovy*DEG2RAD, aspectRatio, nearZ, farZ)
{

}

void gswy::PerspectiveCameraController::OnUpdate(double ts)
{
	m_Camera.SetProjection(m_fovy * DEG2RAD, m_aspectRatio, m_nearZ, m_farZ);
}
