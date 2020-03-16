/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Renderer2D.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#pragma once

#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Texture.h"
#include "engine/EngineCore.h"
#include "engine/allocator/MemoryManager.h"

namespace gswy {

	class ENGINE_API Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		static void OnWindowResize(uint32_t width, uint32_t height);
		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const std::shared_ptr<Texture2D>& texture);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const std::shared_ptr<Texture2D>& texture);
		static void DrawQuad(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<IndexBuffer>& indexBuffer, const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture = nullptr);
		static void DrawDebugQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawSprite(const std::shared_ptr<VertexArray>& vertexArray, const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture);
		static void DrawSprite(const std::shared_ptr<VertexArray>& vertexArray, const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, const std::string& shader);
		static void DrawSprite(const std::shared_ptr<VertexArray>& vertexArray, const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, const std::string& shader, float alpha);
		
		static void BeginBatch(const OrthographicCamera& camera);
		static void EndBatch();
		static void AddBatch(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color, const std::vector<glm::vec3>& vertexData, const std::vector<glm::vec2>& texCoord, const std::shared_ptr<Texture2D>& texture = nullptr);
		static void DrawBatch();

		struct BatchStats
		{
			uint32_t DrawCount = 0;
			uint32_t QuadCount = 0;
		};

		//static const BatchStats& GetStats();
		//static void ResetStats();
	};
}