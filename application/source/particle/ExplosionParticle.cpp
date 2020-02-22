/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/15/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "ExplosionParticle.h"
#include "engine/utility/Random.h"

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

