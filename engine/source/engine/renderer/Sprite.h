/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Sprite.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/01/2020
- End Header ----------------------------*/

#pragma once
#include "Texture.h"
#include "Buffer.h"
#include "VertexArray.h"
#include "engine/math/MathHelper.h"

namespace gswy {

	class ENGINE_API Sprite
	{
	public:
		Sprite();
		Sprite(Texture2D* texture2D);
		~Sprite();
		
		void OpenGLInit();
		void Update(double ts);
		void Draw();
		void DrawInternal(const glm::vec2& rect, const glm::vec2& texCoord, const glm::vec2& texCoordOffset);

		void SetSpritePosition(const glm::vec3& pos);
		void SetSpriteTexture(std::shared_ptr<Texture2D>& texture2D);

		uint32_t GetTextureWidth() const;
		uint32_t GetTextureHeight() const;

		uint32_t GetSpriteWidth() const;
		uint32_t GetSpriteHeight() const;

		void SetSpriteWidth(int w);
		void SetSpriteHeight(int h);

		void SetSpriteX(int x);
		void SetSpritey(int y);

		void SetSpriteScale(const glm::vec2& scale);
		void SetSpriteRotation(float rotation);

		void SetSpriteAlpha(float alpha);
		float GetSpriteAlpha();

		void SetSpriteShader(const std::string& name);
		void ResetSpriteShader();

	private:
		std::shared_ptr<Texture2D> m_Texture2D;
		std::shared_ptr<VertexArray> m_SpriteVertexArray = nullptr;
		std::shared_ptr<VertexBuffer> m_SpriteVertexBuffer = nullptr;
		std::vector<float> m_Vertices;
		glm::vec3 m_Position;
		uint32_t m_SpriteWidth, m_SpriteHeight;
		uint32_t m_SpriteX, m_SpriteY;
		glm::vec2 m_Scale;
		float_t m_Rotation;
		std::string m_ShaderName;
		float_t m_alpha;
		bool m_init;

		std::vector<glm::vec3> m_VertexData;
		std::vector<glm::vec2> m_TexCoord;
	};
}