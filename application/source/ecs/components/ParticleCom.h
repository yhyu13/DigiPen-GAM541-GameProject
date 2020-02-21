/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/18/2020
- End Header ----------------------------*/

#pragma once

#include "engine/ecs/BaseComponent.h"
#include "engine/renderer/ParticleSystem.h"
#include "particle/ExplosionParticle.h"

namespace gswy
{
	/* Data class of sprite */
	struct ParticleCom : BaseComponent<ParticleCom> {

		ParticleCom()
			:
			m_ParticleActive(true),
			m_ParticleSystem(nullptr)
		{
		}

		template<typename ParticleType>
		void Init()
		{
			m_ParticleSystem = MemoryManager::Make_shared<ParticleType>();
		}

		bool IsActive()
		{
			return m_ParticleActive;
		}

		void SetActive(bool b)
		{
			m_ParticleActive = b;
		}

		std::shared_ptr<ParticleSystem> Get()
		{
			return m_ParticleSystem;
		}

	private:

		std::shared_ptr<ParticleSystem> m_ParticleSystem;
		bool m_ParticleActive;
	};
}
