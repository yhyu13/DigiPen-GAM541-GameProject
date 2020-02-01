/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Renderer2D.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

namespace gswy {

	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<IndexBuffer> QuadIndexBuffer;
		std::shared_ptr<Shader> QuadShader;
		std::shared_ptr<Texture2D> QuadTexture;
	};

	static Renderer2DStorage* s_Data;

	void Renderer2D::Init()
	{
		RenderCommand::Init();

		s_Data = new Renderer2DStorage();
		s_Data->QuadVertexArray = VertexArray::Create();

		float quadVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		s_Data->QuadVertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		s_Data->QuadVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		s_Data->QuadVertexArray->AddVertexBuffer(s_Data->QuadVertexBuffer);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		s_Data->QuadIndexBuffer = IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		s_Data->QuadVertexArray->SetIndexBuffer(s_Data->QuadIndexBuffer);

		s_Data->QuadTexture = Texture2D::Create(1, 1);
		uint32_t whiteTextureData = 0xffffffff;
		s_Data->QuadTexture->SetData(&whiteTextureData, sizeof(uint32_t));

		s_Data->QuadShader = Shader::Create("./asset/shaders/QuadShader.vs", "./asset/shaders/QuadShader.fs");
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetInt("u_Texture", 0);
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;
	}

	void Renderer2D::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->QuadIndexBuffer->Bind();
		s_Data->QuadVertexBuffer->Bind();
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
	}

	void Renderer2D::EndScene()
	{
		//s_Data->QuadShader->Unbind();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, scale, rotation, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
	{
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetFloat4("u_Color", color);
		s_Data->QuadTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		s_Data->QuadVertexArray->Unbind();
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& scale, float rotation, const std::shared_ptr<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, scale, rotation, texture);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const std::shared_ptr<Texture2D>& texture)
	{
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);

		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		s_Data->QuadVertexArray->Unbind();
	}
	void Renderer2D::DrawDebugQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
	{
		s_Data->QuadShader->SetFloat4("u_Color", color);
		s_Data->QuadTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();
		glDisable(GL_DEPTH_TEST);
		glDrawElements(GL_LINE_STRIP, s_Data->QuadVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glEnable(GL_DEPTH_TEST);
		s_Data->QuadVertexArray->Unbind();
	}
}