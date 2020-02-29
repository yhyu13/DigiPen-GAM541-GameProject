/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: OpenGLPostProcessing.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/21/2020
- End Header ----------------------------*/
#pragma once
#include "engine/renderer/Shader.h"
#include "engine/renderer/Texture.h"
namespace gswy {

	class OpenGLPostProcessing
	{
	public:
		OpenGLPostProcessing();
		OpenGLPostProcessing(uint32_t screenWidth, uint32_t screenHeight);
		~OpenGLPostProcessing();
		
		void Init();
		void Bind();
		void Unbind();
		void Render(float ts);
		void SetScreenSize(uint32_t screenWidth, uint32_t screenHeight) { m_ScreenWidth = screenWidth; m_ScreenHeight = screenHeight; }

	private:
		uint32_t m_FrameBufferID;
		uint32_t m_DepthID;
		uint32_t m_VAO, m_VBO;
		uint32_t m_RenderTargetID;
		std::shared_ptr<Shader> m_Shader;
		uint32_t m_ScreenWidth, m_ScreenHeight;
	};

}