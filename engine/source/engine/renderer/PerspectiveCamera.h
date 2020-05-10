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

#include "engine/Math/MathHelper.h"

namespace gswy {

    class PerspectiveCamera
    {
    public:
        explicit PerspectiveCamera(float fovy, float aspectRatio, float nearZ, float farZ);

        void SetProjection(float fovy, float aspectRatio, float nearZ, float farZ);

        float GetZoom() const { return m_Zoom; }
        void SetZoom(float zoom) { m_Zoom = zoom; }

        const Vec3f& GetPosition() const { return m_Position; }
        void SetPosition(const Vec3f& position) { m_Position = position;}

        const Quaternion& GetRotation() const { return m_Rotation; }
        void SetRotation(const Quaternion& rotation) { m_Rotation = rotation;}

        const Mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const Mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const Mat4& GetViewProjectionMatrix() { RecalculateViewMatrix();  return m_ViewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();

    private:

        Mat4 m_ProjectionMatrix;
        Mat4 m_ViewMatrix;
        Mat4 m_ViewProjectionMatrix;
        Quaternion m_Rotation;
        Vec3f m_Position;
        float m_Zoom;
    };

}