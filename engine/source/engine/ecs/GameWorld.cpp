#include "GameWorld.h"
#include "BaseComponentSystem.h"
#include "EntityDecorator.h"

namespace gswy {

	GameWorld::GameWorld(std::unique_ptr<EntityManager> manager): m_entityManager(std::move(manager)) {
	}

	GameWorld::~GameWorld() {
	}

	void GameWorld::Init() {
		for (auto&& system : m_systems) {
			system->Init();
		}
	}

	void GameWorld::Update(double frameTime) {
		for (auto&& system : m_systems) {
			system->Update(frameTime);
		}
	}

	void GameWorld::Render() {
		for (auto&& system : m_systems) {
			system->Render();
		}
	}

	EntityDecorator GameWorld::GenerateEntity() {
		return EntityDecorator(m_entityManager->Create(), this);
	}

	void GameWorld::RegisterSystem(std::unique_ptr<BaseComponentSystem> componentSystem) {
		componentSystem->SetWorld(this);
		m_systems.push_back(std::move(componentSystem));
	}

	void GameWorld::RemoveEntity(Entity entity) {
		for (auto&& system : m_systems) {
			system->RemoveEntity(entity);
		}
		m_entityManager->Destroy(entity);
	}

	/*
		This method is called when the component-signature for an entity changes.
		As the components are added and removed from entities, all component-systems
		must either register or unregister the updated entities.
	*/
	void GameWorld::UpdateComponentSystems(const Entity& entity, const BitMaskSignature& oldMask) {
		BitMaskSignature updatedMask = m_entityMasks[entity];
		for (auto&& system : m_systems) {
			BitMaskSignature systemSignature = system->GetSystemSignature();
			if (updatedMask.IsNewMatch(oldMask, systemSignature)) {
				system->AddEntity(entity);
			}
			else if (updatedMask.IsNoLongerMatched(oldMask, systemSignature)) {
				system->RemoveEntity(entity);
			}
		}
	}

}