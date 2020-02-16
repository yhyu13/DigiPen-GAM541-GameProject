/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: ParticleSystem.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/05/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "ParticleSystem.h"
#include "Renderer2D.h"
#include "../utility/Random.h"
#include <glm/gtx/compatibility.hpp>

namespace gswy {
	
	ParticleSystem::ParticleSystem()
	{
		m_ParticlePool.resize(1000);
		m_BufferData.ParticleVertexArray = VertexArray::Create();
	}

	ParticleSystem::~ParticleSystem()
	{
	}

	void ParticleSystem::Update(float ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}
			
			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * ts;
			particle.Rotation += 0.01f * ts;
		}
	}

	void ParticleSystem::Render()
	{
		if (!m_BufferData.ParticleVertexArray)
			return;

		float vertices[] =
		{
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f
		};

		m_BufferData.ParticleVertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		m_BufferData.ParticleVertexBuffer->SetLayout({
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" }
			});
		m_BufferData.ParticleVertexArray->AddVertexBuffer(m_BufferData.ParticleVertexBuffer);

		uint32_t quadIndices[6] = { 0, 1, 2, 2, 3, 0 };
		m_BufferData.ParticleIndexBuffer = IndexBuffer::Create(quadIndices, sizeof(quadIndices) / sizeof(uint32_t));
		m_BufferData.ParticleVertexArray->SetIndexBuffer(m_BufferData.ParticleIndexBuffer);

		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorBegin, particle.ColorEnd, life);
			float size = glm::lerp(particle.SizeBegin, particle.SizeEnd, life);

			Renderer2D::DrawQuad(m_BufferData.ParticleVertexArray, m_BufferData.ParticleIndexBuffer, particle.Position, glm::vec2(size), particle.Rotation, color);
		}

	}

	void ParticleSystem::Emit(const Particle& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

		
		particle.Velocity = particleProps.Velocity;
		particle.Velocity += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}
}