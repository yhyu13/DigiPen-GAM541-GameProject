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
#include <glm/glm.hpp>

namespace gswy {
	
	class ENGINE_API Sprite
	{
	public:
		Sprite();
		Sprite(std::shared_ptr<Texture2D>& texture2D);
		Sprite(std::shared_ptr<Texture2D>& texture2D, int numRowFrames, int numColumnFrames);
		Sprite(const std::string& path, int numRowFrames, int numColumnFrames);
		~Sprite();

		bool LoadFromFile(const std::string& path, int numRowFrames, int numColumnFrames);

		void Update(float ts);
		void Draw();
		void DrawInternal(const glm::vec2& rect, const glm::vec2& texCoord, const glm::vec2& texCoordOffset);

		void SetSpritePosition(const glm::vec3& pos) { m_Position = pos; }
		void SetSpriteTexture(std::shared_ptr<Texture2D>& texture2D) { m_Texture2D = texture2D; }

		void SetCurrentFrame(unsigned int targetFrame) { m_CurrentFrame = targetFrame; }
		unsigned int GetCurrentFrame() const { return m_CurrentFrame; }
		void SetStartFrame(int targetFrame) { m_StartFrame = targetFrame; }
		unsigned int GetStartFrame() const { return m_StartFrame; }
		void SetLastFrame(int targetFrame) { m_LastFrame = targetFrame; }
		unsigned int GetLastFrame() const { return m_LastFrame; }
		unsigned int GetTotalFrames() const { return m_TotalFrames; }

		uint32_t GetTextureWidth() const { return m_Texture2D->GetWidth(); }
		uint32_t GetTextureHeight() const { return m_Texture2D->GetHeight(); }

		void SetAnimSequence(int startWithFrame, int numFrames);
		void SetAnimLooping(bool b) { m_LoopingAnim = b; };

	private:
		std::shared_ptr<Texture2D> m_Texture2D;
		std::shared_ptr<VertexArray> m_SpriteVertexArray;
		std::shared_ptr<VertexBuffer> m_SpriteVertexBuffer;
		glm::vec3 m_Position;
		uint32_t m_SpriteWidth, m_SpriteHeight;
		unsigned int m_CurrentFrame, m_StartFrame, m_LastFrame, m_TotalHorizontalFrame, m_TotalVerticalFrame, m_TotalFrames;
		int m_ElapsedTime;
		int m_MSPerFrame;

		bool m_IsPaused;
		bool m_LoopingAnim;
	};
}