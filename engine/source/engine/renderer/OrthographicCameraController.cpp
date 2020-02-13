/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: OrthographicCameraController.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "OrthographicCameraController.h"

namespace gswy {

	OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio), m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
	{

	}

	void OrthographicCameraController::OnUpdate(double ts)
	{
		float left = -m_AspectRatio * m_ZoomLevel + m_CameraPosition.x;
		float right = m_AspectRatio * m_ZoomLevel + m_CameraPosition.x;
		float button = -m_ZoomLevel + m_CameraPosition.y;
		float top = m_ZoomLevel + m_CameraPosition.y;
		m_Camera.SetProjection(left, right, button, top);
	}
}
