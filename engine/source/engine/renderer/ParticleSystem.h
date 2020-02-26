/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: ParticleSystem.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/05/2020
- End Header ----------------------------*/

#pragma once
#include "engine/math/MathHelper.h"
#include "VertexArray.h"
#include "Shader.h"

namespace gswy {

	struct Particle
	{
		glm::vec3 Position;
		glm::vec3 Speed;
		glm::vec3 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
		float LifeRemaining = 1.0f;
		bool Active = false;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();
		virtual ~ParticleSystem();
		
		virtual void OpenGLInit();
		virtual void Update(float ts);
		virtual void Render();
		virtual void Emit(const Particle& particle);
		void SetPoolSize(uint32_t size) { m_ParticlePoolSize = size; m_ParticlePool.resize(m_ParticlePoolSize); }

	protected:
		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;
		uint32_t m_ParticlePoolSize = 1000;
		
		struct BufferData
		{
			std::shared_ptr<VertexArray> ParticleVertexArray;
			std::shared_ptr<VertexBuffer> ParticleVertexBuffer;
			std::shared_ptr<IndexBuffer> ParticleIndexBuffer;
			std::shared_ptr<Shader> ParticleShader;
		};
		BufferData m_BufferData;
		bool m_init;
	};
}