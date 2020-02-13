/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Sprite.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/01/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "Sprite.h"
#include "Renderer2D.h"

namespace gswy {

	Sprite::Sprite()
	{
		m_SpriteVertexArray = VertexArray::Create();
		m_Position = glm::vec3(0.0f);
		m_SpriteWidth = 0;
		m_SpriteHeight = 0;
		m_SpriteX = 0;
		m_SpriteY = 0;
		m_Scale = 1.0f;
		m_Rotation = 0.0f;
	}

	Sprite::Sprite(Texture2D* texture2D)
		: m_Texture2D(texture2D)
	{
		m_SpriteVertexArray = VertexArray::Create();
		m_Position = glm::vec3(0.0f);
		m_SpriteWidth = GetTextureWidth();
		m_SpriteHeight = GetTextureHeight();
		m_SpriteX = 0;
		m_SpriteY = 0;
		m_Scale = 1.0f;
		m_Rotation = 0.0f;
	}

	Sprite::~Sprite()
	{
	}

	void Sprite::Update(double ts)
	{
	}

	void Sprite::Draw()
	{
		float texWidth = GetTextureWidth();
		float texHeight = GetTextureHeight();
		float perTexCoordOffsetX = (float)(m_SpriteWidth / texWidth);
		float perTexCoordOffsetY = (float)(m_SpriteHeight / texHeight);
		float texCoordX = (float)(m_SpriteX / texWidth);
		float texCoordY = (float)(m_SpriteY / texHeight);
		DrawInternal(glm::vec2(0.5f), glm::vec2(texCoordX, texCoordY), glm::vec2(perTexCoordOffsetX, perTexCoordOffsetY));
	}

	void Sprite::DrawInternal(const glm::vec2& rect, const glm::vec2& texCoord, const glm::vec2& texCoordOffset)
	{
		float quadVertices[] = {
		   -rect.x, -rect.y, 0.0f, texCoord.x, texCoord.y,
			rect.x, -rect.y, 0.0f, texCoord.x + texCoordOffset.x, texCoord.y,
			rect.x,  rect.y, 0.0f, texCoord.x + texCoordOffset.x, texCoord.y + texCoordOffset.y,
		   -rect.x,  rect.y, 0.0f, texCoord.x, texCoord.y + texCoordOffset.y
		};

		m_SpriteVertexBuffer = VertexBuffer::Create(quadVertices, sizeof(quadVertices));
		m_SpriteVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SpriteVertexArray->AddVertexBuffer(m_SpriteVertexBuffer);
		Renderer2D::DrawSprite(m_SpriteVertexArray, m_Position, glm::vec2(m_Scale), m_Rotation, m_Texture2D);
	}
	void Sprite::SetSpritePosition(const glm::vec3& pos) { m_Position = pos; }
	void Sprite::SetSpriteTexture(std::shared_ptr<Texture2D>& texture2D) {
		m_Texture2D = texture2D;
		m_SpriteWidth = GetTextureWidth();
		m_SpriteHeight = GetTextureHeight();
	}
	uint32_t Sprite::GetTextureWidth() const { return m_Texture2D->GetWidth(); }
	uint32_t Sprite::GetTextureHeight() const { return m_Texture2D->GetHeight(); }
	uint32_t Sprite::GetSpriteWidth() const { return m_SpriteWidth; }
	uint32_t Sprite::GetSpriteHeight() const { return m_SpriteHeight; }
	void Sprite::SetSpriteWidth(int w) { m_SpriteWidth = w; }
	void Sprite::SetSpriteHeight(int h) { m_SpriteHeight = h; }
	void Sprite::SetSpriteX(int x) { m_SpriteX = x; }
	void Sprite::SetSpritey(int y) { m_SpriteY = y; }
	void Sprite::SetSpriteScale(float scale)
	{
		m_Scale = scale;
	}
	void Sprite::SetSpriteRotation(float rotation)
	{
		m_Rotation = rotation;
	}
}