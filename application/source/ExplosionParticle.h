#pragma once
#include "engine/renderer/ParticleSystem.h"

namespace gswy {
	
	class ExplosionParticle : public ParticleSystem
	{
	public:
		ExplosionParticle();
		~ExplosionParticle();

		void Update(float ts) override;
	};
};