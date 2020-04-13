/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: OrthographicCameraController.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#pragma once
#include "OrthographicCamera.h"
#include "engine/EngineCore.h"

namespace gswy {
	
	class ENGINE_API OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool rotation = false);

		void OnUpdate(double ts);

		OrthographicCamera& GetCamera() { return m_Camera; }
		const OrthographicCamera& GetCamera() const { return m_Camera; }
		float GetZoomLevel() const { return m_ZoomLevel; }
		float GetCameraMoveSpeed() const { return m_CameraMoveSpeed; }
		float GetAspectRatio() const { return m_AspectRatio; }
		void SetAspectRatio(float aspectRatio) {  m_AspectRatio = aspectRatio; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
		void SetPosition(const glm::vec3& pos) { m_CameraPosition = pos; }
		const glm::vec3& GetPosition() const { return m_CameraPosition; }
	private:
		OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotation = 0.0f;
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraRotationSpeed = 180.0f;
		float m_ZoomLevel = 1.0f;
		float m_AspectRatio = 1.0f;
		bool m_Rotation = false;
	};
}