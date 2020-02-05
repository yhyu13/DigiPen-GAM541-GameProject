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

using namespace gswy;

struct Position : gswy::BaseComponent<Position> {
	Position() = default;
	Position(float x) : x(x) {};
	float x;
};

class Wind : public gswy::BaseComponentSystem {
public:
	Wind() {
		m_systemSignature.AddComponent<Position>();
	}

	virtual void Update(double dt) override {
		for (auto& entity : m_registeredEntities) {
			gswy::ComponentDecorator<Position> position;
			m_parentWorld->Unpack(entity, position);

			// Move 1 every second
			position->x += 1.0f * (dt / 1000.0f);

			// Print entity position
			std::cout << "Entity " << entity.m_id << ": " << position->x << std::endl; // have to override -> operator
		}
	}
};

class Application : public Engine {

public:


	Application() 
		: m_CameraController(1280.0f / 720.0f)
	{
		gswy::Renderer2D::Init();
		// Texture Test
		gswy::Texture2D::Create("./asset/container.png");
		// Sprite Test
		ResourceAllocator<Sprite>::GetInstance()->Init();
		ResourceAllocator<Sprite>::GetInstance()->Add("./asset/SpriteSheetExample.png", "SpriteSheetExample")->SetAnimationKeyFactors(4, 8, 1000 / 15);
	}

	virtual ~Application() {
	}

	virtual void Run() override
	{
		// TODO : remove audio test code below
		AudioManager::GetInstance()->PlaySound("./asset/breakout.mp3", AudioVector3{ 0, 0, 0 }, 1);

		FramerateController* rateController = FramerateController::GetInstance(60);
		Input* input = Input::GetInstance();

		///////// EXAMPLE SETUP FOR TESTING ECS /////////////
		//auto entityManager = std::make_unique<gswy::EntityManager>();
		//auto world = std::make_unique<gswy::GameWorld>(std::move(entityManager));

		//// Add systems
		//std::unique_ptr<gswy::BaseComponentSystem> wind = std::make_unique<Wind>();
		//world->RegisterSystem(std::move(wind));

		//// Initialize game
		//world->Init();

		//// Add an entity with a position
		//auto tumbleweed = world->GenerateEntity();
		//tumbleweed.AddComponent(Position(0));

		//// Run game for "1 second at 50fps"
		//for (int i = 0; i < 50; i++) {
		//	world->Update(20);
		//}
		/////////// END OF ECS TEST //////////////

		auto entityManager = std::make_unique<gswy::EntityManager>();
		auto world = std::make_unique<gswy::GameWorld>(std::move(entityManager));
		// Add systems
		std::unique_ptr<gswy::BaseComponentSystem> playerControllerComSys = std::make_unique<PlayerControllerComSys>();
		std::unique_ptr<gswy::BaseComponentSystem> sceneComSys = std::make_unique<SceneComSys>();
		std::unique_ptr<gswy::BaseComponentSystem> spriteComSys = std::make_unique<SpriteComSys>();
		world->RegisterSystem(std::move(playerControllerComSys));
		world->RegisterSystem(std::move(sceneComSys));
		world->RegisterSystem(std::move(spriteComSys));
		// Initialize game
		world->Init();
		// Add an entity with a position
		auto player = world->GenerateEntity();
		player.AddComponent(TransformCom(0,0,0));
		player.AddComponent(SpriteCom("SpriteSheetExample"));

		while (m_isRunning) {
			rateController->FrameStart();

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

			////Control Sprite Trigger
			//if (input->IsKeyTriggered(GLFW_KEY_W)) {
			//	PRINT("KEY W TRIGGERED!");
			//	m_ControlSprite->SetAnimSequence(16, 8);
			//	m_ControlSprite->SetAnimLooping(true);
			//}
			//else if (input->IsKeyTriggered(GLFW_KEY_S)) {
			//	PRINT("KEY S TRIGGERED!");
			//	m_ControlSprite->SetAnimSequence(24, 8);
			//	m_ControlSprite->SetAnimLooping(true);
			//}
			//else if (input->IsKeyTriggered(GLFW_KEY_A)) {
			//	PRINT("KEY A TRIGGERED!");
			//	m_ControlSprite->SetAnimSequence(8, 8);
			//	m_ControlSprite->SetAnimLooping(true);
			//}
			//else if (input->IsKeyTriggered(GLFW_KEY_D)) {
			//	PRINT("KEY D TRIGGERED!");
			//	m_ControlSprite->SetAnimSequence(0, 8);
			//	m_ControlSprite->SetAnimLooping(true);
			//}

			////Control Sprite KeyPress
			//if (input->IsKeyPressed(GLFW_KEY_W)) {
			//	PRINT("KEY W PRESSED!");
			//	m_ControlSpritePos.x += -sin(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
			//	m_ControlSpritePos.y += cos(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
			//	m_ControlSprite->SetSpritePosition(m_ControlSpritePos);
			//}
			//else if (input->IsKeyPressed(GLFW_KEY_S)) {
			//	PRINT("KEY S PRESSED!");
			//	m_ControlSpritePos.x -= -sin(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
			//	m_ControlSpritePos.y -= cos(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
			//	m_ControlSprite->SetSpritePosition(m_ControlSpritePos);
			//}
			//
			//if (input->IsKeyPressed(GLFW_KEY_A)) {
			//	PRINT("KEY A PRESSED!");
			//	m_ControlSpritePos.x -= cos(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
			//	m_ControlSpritePos.y -= sin(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
			//	m_ControlSprite->SetSpritePosition(m_ControlSpritePos);
			//}
			//else if (input->IsKeyPressed(GLFW_KEY_D)) {
			//	PRINT("KEY D PRESSED!");
			//	m_ControlSpritePos.x += cos(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
			//	m_ControlSpritePos.y += sin(glm::radians(0.0f)) * m_ControlSpriteMoveSpeed * rateController->GetFrameTime();
			//	m_ControlSprite->SetSpritePosition(m_ControlSpritePos);
			//}

			//Update(rateController->GetFrameTime());
			//Input::GetInstance()->Update();
			
			m_isRunning = !m_window->ShouldExit();
			rateController->FrameEnd();
		}
	}

	virtual void Update(double ts) {
		m_CameraController.OnUpdate(ts);

		gswy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		gswy::RenderCommand::Clear();

		gswy::Renderer2D::BeginScene(m_CameraController.GetCamera());
		
		//Draw Quad with Texture
		//gswy::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, m_Texture);
		
		//Draw Sprite Sheet
		//m_ControlSprite->Update(ts * 1000.0f);
		//m_ControlSprite->Draw();

		gswy::Renderer2D::EndScene();
	}

protected:

private:
	gswy::OrthographicCameraController m_CameraController;
	//std::shared_ptr<gswy::Texture2D> m_Texture;

	////Control Sprite
	//std::shared_ptr<gswy::Sprite> m_ControlSprite;
	//glm::vec3 m_ControlSpritePos = glm::vec3(0.0f, 0.0f, 1.0f);
	//float m_ControlSpriteMoveSpeed = 5.0f;
};

Engine* gswy::CreateEngineApplication() {
	return new Application();
}

