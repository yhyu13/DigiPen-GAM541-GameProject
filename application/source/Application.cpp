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
		auto entityManager = std::make_unique<gswy::EntityManager>();
		auto world = std::make_unique<gswy::GameWorld>(std::move(entityManager));
		// Add systems
		std::unique_ptr<gswy::BaseComponentSystem> playerControllerComSys = std::make_unique<PlayerControllerComSys>();
		std::unique_ptr<gswy::BaseComponentSystem> sceneComSys = std::make_unique<SceneComSys>();
		std::unique_ptr<gswy::BaseComponentSystem> spriteComSys = std::make_unique<SpriteComSys>();
		std::unique_ptr<gswy::BaseComponentSystem> animationComSys = std::make_unique<AnimationComSys>();
		world->RegisterSystem(std::move(playerControllerComSys));
		world->RegisterSystem(std::move(sceneComSys));
		world->RegisterSystem(std::move(spriteComSys));
		world->RegisterSystem(std::move(animationComSys));
		// Initialize game
		world->Init();
		// Add an entity with a position
		auto player = world->GenerateEntity();
		player.AddComponent(TransformCom(0,0,0));
		player.AddComponent(SpriteCom());
		auto animCom = AnimationCom();
		animCom.Add("PlayerAnimation1", "Move1");
		animCom.Add("PlayerAnimation2", "Move2");
		animCom.Add("PlayerAnimation3", "Move3");
		animCom.Add("PlayerAnimation4", "Move4");
		player.AddComponent(animCom);

		while (!m_window->ShouldExit()) {
			rateController->FrameStart();
			{
#ifdef _DEBUG
				std::stringstream stream;
				stream << "Frame Time: " << rateController->GetFrameTime() * 1000 << "ms";
				m_window->UpdateTitle(stream.str());
#endif
				{
					// window update
					m_window->Update(rateController->GetFrameTime());
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
					m_CameraController.OnUpdate(rateController->GetFrameTime());

					gswy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
					gswy::RenderCommand::Clear();

					gswy::Renderer2D::BeginScene(m_CameraController.GetCamera());
					// world render
					world->Render();

					gswy::Renderer2D::EndScene();
				}
			}
			rateController->FrameEnd();
		}
	}

	virtual void Update(double ts) {
	}

protected:

private:
	gswy::OrthographicCameraController m_CameraController;
};

Engine* gswy::CreateEngineApplication() {
	return new Application();
}

