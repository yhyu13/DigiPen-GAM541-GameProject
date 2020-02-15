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
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/SpriteSheetExample.png", "SpriteSheetExample");
		// Animation Test
		ResourceAllocator<Animation>::GetInstance()->Init();
		auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation1.txt", "PlayerAnimation1");
		for (int i = 0; i < 8; ++i)
		{
			playerAnim1->AddFrame("SpriteSheetExample", 24*i, 32 * 0, 24, 32, 1.0 / 15.0);
		}
		auto playerAnim2 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation2.txt", "PlayerAnimation2");
		for (int i = 0; i < 8; ++i)
		{
			playerAnim2->AddFrame("SpriteSheetExample", 24 * i, 32*1, 24, 32, 1.0 / 15.0);
		}
		auto playerAnim3 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation3.txt", "PlayerAnimation3");
		for (int i = 0; i < 8; ++i)
		{
			playerAnim3->AddFrame("SpriteSheetExample", 24 * i, 32 * 2, 24, 32, 1.0 / 15.0);
		}
		auto playerAnim4 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation4.txt", "PlayerAnimation4");
		for (int i = 0; i < 8; ++i)
		{
			playerAnim4->AddFrame("SpriteSheetExample", 24 * i, 32 * 3, 24, 32, 1.0 / 15.0);
		}	
	}

	virtual ~Application() {
	}

	virtual void Run() override
	{
		AudioManager::GetInstance()->PlaySound("./asset/breakout.mp3", AudioVector3{ 0, 0, 0 }, 1);

		FramerateController* rateController = FramerateController::GetInstance(60);
		Input* input = Input::GetInstance();

		///////// EXAMPLE SETUP FOR TESTING ECS /////////////

		std::shared_ptr<gswy::EntityManager<GameObjectType>> entityManager = std::make_shared<gswy::EntityManager<GameObjectType>>();
		std::shared_ptr<GameWorld<GameObjectType>> world = std::make_shared<gswy::GameWorld<GameObjectType>>(entityManager);

		// Add systems
		world->RegisterSystem(std::make_shared<PlayerControllerComSys>());
		world->RegisterSystem(std::make_shared<SceneComSys>());
		world->RegisterSystem(std::make_shared<SpriteComSys>());
		world->RegisterSystem(std::make_shared<AnimationComSys>());

		// Initialize game
		world->Init();

		auto player = world->GenerateEntity(GameObjectType::PLAYER);
		player.AddComponent(TransformCom(0,0,0));
		player.AddComponent(SpriteCom());
		auto animCom = AnimationCom();
		animCom.Add("PlayerAnimation1", "MoveRight");
		animCom.Add("PlayerAnimation2", "MoveLeft");
		animCom.Add("PlayerAnimation3", "MoveUp");
		animCom.Add("PlayerAnimation4", "MoveDown");
		animCom.SetCurrentAnimationState("MoveUp");
		player.AddComponent(animCom);

		auto enemy = world->GenerateEntity(GameObjectType::ENEMY);
		enemy.AddComponent(TransformCom(1, 0, 0));
		enemy.AddComponent(SpriteCom());
		auto animCom2 = AnimationCom();
		animCom2.Add("PlayerAnimation1", "MoveRight");
		animCom2.SetCurrentAnimationState("MoveRight");
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
					Input::GetInstance()->Update(rateController->GetFrameTime());
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

