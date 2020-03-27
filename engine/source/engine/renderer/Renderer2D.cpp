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

#include "engine-precompiled-header.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>

static const size_t MaxBatchCount = 1000;
static const size_t MaxVertexCount = MaxBatchCount * 4;
static const size_t MaxIndexCount = MaxBatchCount * 6;
static const size_t MaxTextures = 32;

namespace gswy {

	struct Renderer2DStorage
	{
		std::shared_ptr<VertexArray> QuadVertexArray;
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;
		std::shared_ptr<IndexBuffer> QuadIndexBuffer;
		std::shared_ptr<Shader> QuadShader;
		std::shared_ptr<Texture2D> QuadTexture;

		std::map<std::string, std::shared_ptr<Shader>> ShaderMap;
	};

	static Renderer2DStorage* s_Data;

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoords;
		glm::vec4 Color;
		float TexIndex;
	};

	struct Renderer2DBatchStorage
	{
		Renderer2DBatchStorage() {}
		~Renderer2DBatchStorage() {}

		uint32_t BatchVertexArray = 0;
		uint32_t BatchVertexBuffer = 0;
		uint32_t BatchIndexBuffer = 0;
		uint32_t IndexCount = 0;

		Vertex* BatchBuffer = nullptr;
		Vertex* BatchBufferPtr = nullptr;

		std::array<uint32_t, MaxTextures> TextureSlots;
		uint32_t TextureSlotIndex = 1;
		
		std::shared_ptr<Texture2D> RawTexture;
		
		std::shared_ptr<Shader> BatchShader;
		std::vector<glm::vec3> VertexData;
		std::vector<glm::vec2> TexCoord;

		OrthographicCamera cam = OrthographicCamera(1.0f, 1.0f, 1.0f, 1.0f);
		Renderer2D::BatchStats RenderStats;
	};

	static Renderer2DBatchStorage* s_BatchData;

	void Renderer2D::Init()
	{
		RenderCommand::Init();

		// Quad Rendering Initialize
		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

		// Create all shaders
		s_Data->ShaderMap["Default"] = Shader::Create("./asset/shaders/QuadShader.vs", "./asset/shaders/QuadShader.fs");
		s_Data->ShaderMap["White"] = Shader::Create("./asset/shaders/QuadShader.vs", "./asset/shaders/QuadShaderWhite.fs");
		// Init all shaders
		for (auto it = s_Data->ShaderMap.begin(); it != s_Data->ShaderMap.end(); ++it)
		{
			// OpenGL shader parameters could be assigned only after binding.
			it->second->Bind();
			it->second->SetInt("u_Texture", 0);
		}
		// Assign default shader
		s_Data->QuadShader = s_Data->ShaderMap["Default"];
		s_Data->QuadShader->Bind();

		//Batch Rendering Initialize
		//------------------------------------------------------------------------------------------------------------------------------------------------------------------------
		s_BatchData = new Renderer2DBatchStorage();
		s_BatchData->BatchShader = Shader::Create("./asset/shaders/BatchShader.vs", "./asset/shaders/BatchShader.fs");
		int samplers[32];
		for (int i = 0; i < 32; ++i)
			samplers[i] = i;
		s_BatchData->BatchShader->Bind();
		s_BatchData->BatchShader->SetIntV("u_Textures", 32, samplers);
		s_BatchData->BatchBuffer = new Vertex[MaxVertexCount];

		glCreateVertexArrays(1, &s_BatchData->BatchVertexArray);
		glBindVertexArray(s_BatchData->BatchVertexArray);

		glCreateBuffers(1, &s_BatchData->BatchVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, s_BatchData->BatchVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

		glEnableVertexArrayAttrib(s_BatchData->BatchVertexArray, 0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));
		glEnableVertexArrayAttrib(s_BatchData->BatchVertexArray, 1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));
		glEnableVertexArrayAttrib(s_BatchData->BatchVertexArray, 2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));
		glEnableVertexArrayAttrib(s_BatchData->BatchVertexArray, 3);
		glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexIndex));

		uint32_t indices[MaxIndexCount];
		uint32_t offset = 0;
		for (int i = 0; i < MaxIndexCount; i += 6)
		{
			indices[i + 0] = 0 + offset;
			indices[i + 1] = 1 + offset;
			indices[i + 2] = 2 + offset;

			indices[i + 3] = 2 + offset;
			indices[i + 4] = 3 + offset;
			indices[i + 5] = 0 + offset;

			offset += 4;
		}

		glCreateBuffers(1, &s_BatchData->BatchIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_BatchData->BatchIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		s_BatchData->RawTexture = Texture2D::Create(1, 1);
		s_BatchData->RawTexture->SetData(&whiteTextureData, sizeof(uint32_t));
		s_BatchData->TextureSlots[0] = s_BatchData->RawTexture->GetRendererID();
		
		for (size_t i = 1; i < MaxTextures; ++i)
			s_BatchData->TextureSlots[i] = 0;
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;

		glDeleteVertexArrays(1, &s_BatchData->BatchVertexArray);
		glDeleteBuffers(1, &s_BatchData->BatchVertexBuffer);
		glDeleteBuffers(1, &s_BatchData->BatchIndexBuffer);
		delete s_BatchData->BatchBuffer;
	}

	void Renderer2D::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer2D::BeginScene(const OrthographicCamera& camera)
	{
		s_Data->QuadIndexBuffer->Bind();
		s_Data->QuadVertexBuffer->Bind();
		// Init all shaders at the begining of the scene
		for (auto it = s_Data->ShaderMap.begin(); it != s_Data->ShaderMap.end(); ++it)
		{
			it->second->Bind();
			it->second->SetMat4("u_ViewProjection", camera.GetViewProjectionMatrix());
		}
	}

	void Renderer2D::EndScene()
	{
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
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
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
		if (texture)
			texture->Bind();
		else
			s_Data->QuadTexture->Bind();
		
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);
		
		s_Data->QuadVertexArray->Bind();
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);
		s_Data->QuadVertexArray->Unbind();
	}
	
	void Renderer2D::DrawQuad(const std::shared_ptr<VertexArray>& vertexArray, const std::shared_ptr<IndexBuffer>& indexBuffer, const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture)
	{
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetFloat4("u_Color", color);

		texture ? texture->Bind() : s_Data->QuadTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		vertexArray->SetIndexBuffer(indexBuffer ? indexBuffer : s_Data->QuadIndexBuffer);
		RenderCommand::DrawIndexed(vertexArray);
		vertexArray->Unbind();
	}

	void Renderer2D::DrawDebugQuad(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color)
	{
		s_Data->QuadShader->SetFloat4("u_Color", color);
		s_Data->QuadTexture->Bind();

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);
		s_Data->QuadVertexArray->Bind();
		glDisable(GL_DEPTH_TEST);
		glDrawElements(GL_LINE_STRIP, s_Data->QuadVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);
		glEnable(GL_DEPTH_TEST);
		s_Data->QuadVertexArray->Unbind();
	}


	void Renderer2D::DrawSprite(const std::shared_ptr<VertexArray>& vertexArray, const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture)
	{
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetFloat4("u_Color", glm::vec4(1.0f));
		texture->Bind();
	
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);
	
		vertexArray->Bind();
		vertexArray->SetIndexBuffer(s_Data->QuadIndexBuffer);
		RenderCommand::DrawIndexed(vertexArray);
		vertexArray->Unbind();
	}

	void Renderer2D::DrawSprite(const std::shared_ptr<VertexArray>& vertexArray, const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, const std::string& shader)
	{
		if (s_Data->ShaderMap.find(shader) == s_Data->ShaderMap.end())
		{
			// TODO : Engine exception
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Shader called " + str2wstr(shader) + L" has not been managed!");
		}
		s_Data->QuadShader = s_Data->ShaderMap[shader];
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetFloat4("u_Color", glm::vec4(1.0f));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);
		texture->Bind();
		vertexArray->Bind();
		vertexArray->SetIndexBuffer(s_Data->QuadIndexBuffer);
		RenderCommand::DrawIndexed(vertexArray);
		vertexArray->Unbind();
	}
	void Renderer2D::DrawSprite(const std::shared_ptr<VertexArray>& vertexArray, const glm::vec3& position, const glm::vec2& size, float rotation, const std::shared_ptr<Texture2D>& texture, const std::string& shader, float alpha)
	{
		if (s_Data->ShaderMap.find(shader) == s_Data->ShaderMap.end())
		{
			// TODO : Engine exception
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Shader called " + str2wstr(shader) + L" has not been managed!");
		}
		s_Data->QuadShader = s_Data->ShaderMap[shader];
		s_Data->QuadShader->Bind();
		s_Data->QuadShader->SetFloat4("u_Color", glm::vec4(1,1,1,alpha));
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
		s_Data->QuadShader->SetMat4("u_Transform", transform);
		texture->Bind();
		vertexArray->Bind();
		vertexArray->SetIndexBuffer(s_Data->QuadIndexBuffer);
		RenderCommand::DrawIndexed(vertexArray);
		vertexArray->Unbind();
	}

	void Renderer2D::BeginBatch(const OrthographicCamera& camera)
	{
		s_BatchData->cam = camera;
		s_BatchData->BatchShader->Bind();
		s_BatchData->BatchShader->SetMat4("u_ViewProjection", s_BatchData->cam.GetViewProjectionMatrix());
		s_BatchData->BatchShader->SetMat4("u_Transform", glm::translate(glm::mat4(1.0f), glm::vec3(0.0f)));
		s_BatchData->BatchBufferPtr = s_BatchData->BatchBuffer;
	}

	void Renderer2D::EndBatch()
	{
		GLsizeiptr size = (uint8_t*)s_BatchData->BatchBufferPtr - (uint8_t*)s_BatchData->BatchBuffer;
		glBindBuffer(GL_ARRAY_BUFFER, s_BatchData->BatchVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_BatchData->BatchBuffer);
	}

	void Renderer2D::AddBatch(const glm::vec3& position, const glm::vec2& scale, float rotation, const glm::vec4& color, const std::vector<glm::vec3>& vertexData, const std::vector<glm::vec2>& texCoord, const std::shared_ptr<Texture2D>& texture)
	{
		if (texture)
		{
			if (s_BatchData->IndexCount >= MaxIndexCount || s_BatchData->TextureSlotIndex > 31)
			{
				EndBatch();
				DrawBatch();
				BeginBatch(s_BatchData->cam);
			}
		}
		else
		{
			if (s_BatchData->IndexCount >= MaxIndexCount)
			{
				EndBatch();
				DrawBatch();
				BeginBatch(s_BatchData->cam);
			}
		}
		glm::vec4 resolvedColor = color;
		//GPU like float
		float textureIndex = 0.0f;

		if (texture)
		{
			resolvedColor = color;

			//Get the texture already in slot
			for (uint32_t i = 1; i < s_BatchData->TextureSlotIndex; ++i)
			{
				if (s_BatchData->TextureSlots[i] == texture->GetRendererID())
				{
					textureIndex = (float)i;
					break;
				}
			}

			//New Texture
			if (textureIndex == 0.0f)
			{
				textureIndex = (float)s_BatchData->TextureSlotIndex;
				s_BatchData->TextureSlots[s_BatchData->TextureSlotIndex] = texture->GetRendererID();
				s_BatchData->TextureSlotIndex++;
			}
		}
		else
		{
			resolvedColor = color;
		}

		glm::vec4 pos;
		if (!vertexData.size())
		{
			s_BatchData->VertexData = { 
				glm::vec3(-0.5f, -0.5f, 0.0f),
				glm::vec3( 0.5f, -0.5f, 0.0f),
				glm::vec3( 0.5f,  0.5f, 0.0f),
				glm::vec3(-0.5f,  0.5f, 0.0f)
			};
		}
		else
		{
			s_BatchData->VertexData = vertexData;
		}

		if (!texCoord.size())
		{
			s_BatchData->TexCoord = {
				glm::vec2(0.0f, 0.0f),
				glm::vec2(1.0f, 0.0f),
				glm::vec2(1.0f, 1.0f),
				glm::vec2(0.0f, 1.0f)
			};
		}
		else
		{
			s_BatchData->TexCoord = texCoord;
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), rotation, glm::vec3(0.0f, 0.0f, 1.0f)) * glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		pos = transform * glm::vec4(s_BatchData->VertexData[0], 1.0f);
		s_BatchData->BatchBufferPtr->Position = { pos.x, pos.y, pos.z };
		s_BatchData->BatchBufferPtr->TexCoords = s_BatchData->TexCoord[0];
		s_BatchData->BatchBufferPtr->Color = resolvedColor;
		s_BatchData->BatchBufferPtr->TexIndex = textureIndex;
		s_BatchData->BatchBufferPtr++;

		pos = transform * glm::vec4(s_BatchData->VertexData[1].x, s_BatchData->VertexData[1].y, s_BatchData->VertexData[1].z, 1.0f);
		s_BatchData->BatchBufferPtr->Position = { pos.x, pos.y, pos.z };
		s_BatchData->BatchBufferPtr->TexCoords = s_BatchData->TexCoord[1];
		s_BatchData->BatchBufferPtr->Color = resolvedColor;
		s_BatchData->BatchBufferPtr->TexIndex = textureIndex;
		s_BatchData->BatchBufferPtr++;

		pos = transform * glm::vec4(s_BatchData->VertexData[2].x, s_BatchData->VertexData[2].y, s_BatchData->VertexData[2].z, 1.0f);
		s_BatchData->BatchBufferPtr->Position = { pos.x, pos.y, pos.z };
		s_BatchData->BatchBufferPtr->TexCoords = s_BatchData->TexCoord[2];
		s_BatchData->BatchBufferPtr->Color = resolvedColor;
		s_BatchData->BatchBufferPtr->TexIndex = textureIndex;
		s_BatchData->BatchBufferPtr++;

		pos = transform * glm::vec4(s_BatchData->VertexData[3].x, s_BatchData->VertexData[3].y, s_BatchData->VertexData[3].z, 1.0f);
		s_BatchData->BatchBufferPtr->Position = { pos.x, pos.y, pos.z };
		s_BatchData->BatchBufferPtr->TexCoords = s_BatchData->TexCoord[3];
		s_BatchData->BatchBufferPtr->Color = resolvedColor;
		s_BatchData->BatchBufferPtr->TexIndex = textureIndex;
		s_BatchData->BatchBufferPtr++;

		s_BatchData->IndexCount += 6;
		s_BatchData->RenderStats.QuadCount++;
	}

	void Renderer2D::DrawBatch()
	{
		for (uint32_t i = 0; i < s_BatchData->TextureSlotIndex; ++i)
			glBindTextureUnit(i, s_BatchData->TextureSlots[i]);

		glBindVertexArray(s_BatchData->BatchVertexArray);
		glDrawElements(GL_TRIANGLES, s_BatchData->IndexCount, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		s_BatchData->RenderStats.DrawCount++;

		s_BatchData->IndexCount = 0;
		s_BatchData->TextureSlotIndex = 1;
	}
}
