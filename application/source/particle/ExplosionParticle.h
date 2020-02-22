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