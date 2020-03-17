/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/20/2020
- End Header ----------------------------*/

#pragma once
#include <map>
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "ecs/components/ParticleCom.h"
#include "ecs/components/TransformCom.h"
#include "ecs/EntityType.h"

namespace gswy
{
	class ParticleComSys : public BaseComponentSystem<GameObjectType> {
	public:
		ParticleComSys() {
			m_systemSignature.AddComponent<ParticleCom>();
			m_systemSignature.AddComponent<TransformCom>();
		}

		virtual void Init() override
		{
			{
				Particle m_Particle;
				m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
				m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
				m_Particle.SizeBegin = 0.01f, m_Particle.SizeVariation = 0.0f, m_Particle.SizeEnd = 0.0f;
				m_Particle.LifeTime = 0.1f;
				m_Particle.Velocity = { 0.0f, 0.0f, 0.0f };
				m_Particle.VelocityVariation = { 0.0f, 0.0f, 0.0f };
				m_Particle.Position = { 0.0f, 0.0f, 0.0f };
				m_Particle.Speed = { 1.0f, 1.0f, 0.0f };
				m_particleMap["ExplosionParticle"] = m_Particle;
			}
		}

		virtual void Render(double dt) override
		{
			lock();
			for (auto& entity : m_registeredEntities) {
				// Check active
				ComponentDecorator<ActiveCom, GameObjectType> active;
				m_parentWorld->Unpack(entity, active);
				if (!active->IsActive())
				{
					continue;
				}
				ComponentDecorator<ParticleCom, GameObjectType> particle;
				m_parentWorld->Unpack(entity, particle);
				/* Drawing particle */
				particle->Get()->Render();
			}
			unlock();
		}

		virtual void PreRenderUpdate(double ts) override
		{
			if (!ts)
			{
				return;
			}

			lock();
			for (auto& entity : m_registeredEntities) {
				ComponentDecorator<ParticleCom, GameObjectType> particle;
				ComponentDecorator<TransformCom, GameObjectType> transform;
				m_parentWorld->Unpack(entity, particle);
				m_parentWorld->Unpack(entity, transform);
				/*
					Emit particles with different type of particle system
				*/
				if (dynamic_cast<ExplosionParticle*>(particle->Get().get()))
				{
					if (particle->IsActive())
					{
						auto m_Particle = m_particleMap["ExplosionParticle"];
						m_Particle.Position += transform->GetPos3D();
						for (int i = 0; i < 5; i++)
						{
							particle->Get()->Emit(m_Particle);
						}
					}
				}
				particle->Get()->Update(ts);
			}
			unlock();
		}
	private:
		std::map<std::string, Particle> m_particleMap;
	};
}