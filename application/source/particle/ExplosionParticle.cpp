#include "engine-precompiled-header.h"
#include "ExplosionParticle.h"
#include "engine/utility/Random.h"


/* Explosion Particle Parameter Suggestion
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		m_Particle.SizeBegin = 0.35f, m_Particle.SizeVariation = 0.0f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 0.1f;
		m_Particle.Velocity = { 0.0f, 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 0.0f, 0.0f, 0.0f };
		m_Particle.Position = { 0.0f, 0.0f, 0.0f };
		m_Particle.Speed = { 1.0f, 1.0f, 0.0f };
		*/

namespace gswy {

	ExplosionParticle::ExplosionParticle()
	{
	}

	ExplosionParticle::~ExplosionParticle()
	{
	}

	void ExplosionParticle::Update(float ts)
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

			particle.Velocity.x = particle.Velocity.x + particle.Speed.x * cos(particle.Rotation) * ts;
			particle.Velocity.y = particle.Velocity.y + particle.Speed.y * sin(particle.Rotation) * ts;

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * Random::Float();
			particle.Rotation += 0.01f * ts;
		}
	}

}

