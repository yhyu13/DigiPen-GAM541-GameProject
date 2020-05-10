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

#pragma once
#include "PerspectiveCamera.h"
#include "engine/EngineCore.h"

namespace gswy {

	class ENGINE_API PerspectiveCameraController
	{
	public:
		PerspectiveCameraController();
		explicit PerspectiveCameraController(float fovy, float aspectRatio, float nearZ, float farZ);

		void OnUpdate(double ts);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }
		
		void SetCameraMoveSpeed(float speed) { m_CameraMoveSpeed = speed; }
		float GetCameraMoveSpeed() const { return m_CameraMoveSpeed; }

		void SetCameraRotationSpeed(float speed) { m_CameraRotationSpeed = speed; }
		float GetCameraRotationSpeed() const { return m_CameraRotationSpeed; }

		float GetAspectRatio() const { return m_aspectRatio; }
		void SetAspectRatio(float aspectRatio) { m_aspectRatio = aspectRatio; }

		float GetFovY() const { return m_fovy; }
		void SetFovY(float value) { m_fovy = value; }

		float GetNearZ() const { return m_nearZ; }
		void SetNearZ(float value) { m_nearZ = value; }

		float GetFarZ() const { return m_farZ; }
		void SetFarZ(float value) { m_farZ = value; }

		void SetZoom(float level) { m_Camera.SetZoom(level); }
		float GetZoom() const { return m_Camera.GetZoom(); }

		void SetPosition(const Vec3f& pos) { m_Camera.SetPosition(pos); }
		const Vec3f& GetPosition() const { return m_Camera.GetPosition(); }

		void SetRotation(const Quaternion& rot) { m_Camera.SetRotation(rot); }
		const Quaternion& GetRotation() const { return m_Camera.GetRotation(); }

	private:
		PerspectiveCamera m_Camera;
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraRotationSpeed = 90.0f;
		float m_aspectRatio = 1.0f;
		float m_fovy = 45.0f;
		float m_nearZ = .1f;
		float m_farZ = 2000.0f;
	};
}