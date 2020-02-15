/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Application.cpp
Purpose			: Game application class. Creates an instance of the game application which plugs into the engine.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#include "EngineExport.h"
#include "ecs/componentSystem/PlayerControllerComSys.h"
#include "ecs/componentSystem/SceneComSys.h"
#include "ecs/componentSystem/SpriteComSys.h"
#include "ecs/componentSystem/AnimationComSys.h"


#include <sstream>

using namespace gswy;

class Application : public Engine {

public:

	Application() 
		: m_CameraController(1280.0f / 720.0f)
	{
		gswy::Renderer2D::Init();
		// Texture Test
		ResourceAllocator<Texture2D>::GetInstance()->Init();
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/GAM541_Char1_Moving_Upper_Unarmed.png", "GAM541_Char1_Moving_Upper_Unarmed");
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/background3.png", "Background3");
		// Animation Test
		ResourceAllocator<Animation>::GetInstance()->Init();
		auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation1.txt", "PlayerAnimation1");
		for (int i = 0; i < 8; ++i)
		{
			playerAnim1->AddFrame("GAM541_Char1_Moving_Upper_Unarmed", 59*i, 32 * 0, 59, 32, 1.0 / 15.0);
		}	
	}

	virtual ~Application() {
	}

	virtual void Run() override
	{
		AudioManager::GetInstance()->PlaySound("./asset/breakout.mp3", AudioVector3{ 0, 0, 0 }, 1);

		FramerateController* rateController = FramerateController::GetInstance(60);
		InputManager* input = InputManager::GetInstance();

		///////// EXAMPLE SETUP FOR TESTING ECS /////////////

		//std::shared_ptr<gswy::EntityManager<GameObjectType>> entityManager = MemoryManager::Make_shared<gswy::EntityManager<GameObjectType>>();
		std::shared_ptr<gswy::EntityManager<GameObjectType>> entityManager = MemoryManager::Make_shared<gswy::EntityManager<GameObjectType>>();
		std::shared_ptr<GameWorld<GameObjectType>> world = MemoryManager::Make_shared<gswy::GameWorld<GameObjectType>>(entityManager);

		// Add systems
		world->RegisterSystem(MemoryManager::Make_shared<PlayerControllerComSys>());
		world->RegisterSystem(MemoryManager::Make_shared<SceneComSys>());
		world->RegisterSystem(MemoryManager::Make_shared<SpriteComSys>());
		world->RegisterSystem(MemoryManager::Make_shared<AnimationComSys>());

		// Initialize game
		world->Init();

		auto background = world->GenerateEntity(GameObjectType::ENEMY);
		background.AddComponent(TransformCom(0, 0, 0));
		auto sprite0 = SpriteCom();
		sprite0.SetTexture("Background3");
		background.AddComponent(sprite0);

		auto player = world->GenerateEntity(GameObjectType::PLAYER);
		player.AddComponent(TransformCom(0,0,1));
		player.AddComponent(SpriteCom());
		auto animCom = AnimationCom();
		animCom.Add("PlayerAnimation1", "Move");
		animCom.SetCurrentAnimationState("Move");
		player.AddComponent(animCom);

		auto enemy = world->GenerateEntity(GameObjectType::ENEMY);
		enemy.AddComponent(TransformCom(1, 0, 1));
		enemy.AddComponent(SpriteCom());
		auto animCom2 = AnimationCom();
		animCom2.Add("PlayerAnimation1", "Move");
		animCom2.SetCurrentAnimationState("Move");
		enemy.AddComponent(animCom2);

		auto entity = player.GetEntity();

		while (m_isRunning) {

			rateController->FrameStart();
			{
#ifdef _DEBUG
				std::stringstream stream;
				stream << "Frame Time: " << rateController->GetFrameTime() * 1000 << "ms";
				m_window->UpdateTitle(stream.str());
#endif
				{
					// Engine update
					Update(rateController->GetFrameTime());
				}
				{
					// world update
					world->Update(rateController->GetFrameTime());
				}

				{
					// Manger update
					InputManager::GetInstance()->Update(rateController->GetFrameTime());
				}

				{
					// Draw Update
					// Setting camera position as the player position (TODO : 1, Making gameworld a singleton 2, making gameworld be able to get entity by ID or something)
					ComponentDecorator<TransformCom, GameObjectType> position;
					world->Unpack(entity, position);
					m_CameraController.SetPosition(glm::vec3(position->m_x, position->m_y, position->m_z));
					m_CameraController.OnUpdate(rateController->GetFrameTime());

					gswy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
					gswy::RenderCommand::Clear();

					gswy::Renderer2D::BeginScene(m_CameraController.GetCamera());
					// world render
					world->Render();

					gswy::Renderer2D::EndScene();
				}
				m_isRunning = !m_window->ShouldExit();
			}
			rateController->FrameEnd();
		}
	}

	virtual void Update(double ts) {
		Engine::Update(ts);
	}

protected:

private:
	gswy::OrthographicCameraController m_CameraController;
};

Engine* gswy::CreateEngineApplication() {
	return new Application();
}

