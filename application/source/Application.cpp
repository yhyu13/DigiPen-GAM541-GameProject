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
#include "engine/renderer/Renderer2D.h"

#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/EntityDecorator.h"

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
		m_Texture = gswy::Texture2D::Create("./asset/container.png");
		gswy::Renderer2D::Init();
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
		auto entityManager = std::make_unique<gswy::EntityManager>();
		auto world = std::make_unique<gswy::GameWorld>(std::move(entityManager));

		// Add systems
		std::unique_ptr<gswy::BaseComponentSystem> wind = std::make_unique<Wind>();
		world->RegisterSystem(std::move(wind));

		// Initialize game
		world->Init();

		// Add an entity with a position
		auto tumbleweed = world->GenerateEntity();
		tumbleweed.AddComponent(Position(0));

		// Run game for "1 second at 50fps"
		for (int i = 0; i < 50; i++) {
			world->Update(20);
		}
		/////////// END OF ECS TEST //////////////

		while (m_isRunning) {
			rateController->FrameStart();

#ifdef _DEBUG
			std::stringstream stream;
			stream << "Frame Time: " << rateController->GetFrameTime() * 1000 << "ms";
			m_window->UpdateTitle(stream.str());
#endif
			m_window->Update();

			/*if (input->IsKeyTriggered(GLFW_KEY_A)) {
				PRINT("KEY A TRIGGERED!");
			}*/

			if (input->IsKeyPressed(GLFW_KEY_A)) {
				PRINT("KEY A PRESSED!");
			}

			/*if (input->IsKeyTriggered(GLFW_KEY_SPACE)) {
				PRINT("KEY SPACE TRIGGERED!");
			}

			if (input->IsKeyReleased(GLFW_KEY_A)) {
				PRINT("KEY A RELEASED!");
			}

			if (input->IsMouseButtonPressed(GLFW_MOUSE_BUTTON_1)) {
				PRINT("Mouse button 1 PRESSED!");
			}

			if (input->IsMouseButtonReleased(GLFW_MOUSE_BUTTON_1)) {
				PRINT("Mouse button 1 RELEASED!");
			}

			std::stringstream stream1;
			stream1 << "cursor-x: " << input->GetMousePositionX() << "\t";
			stream1 << "cursor-y: " << input->GetMousePositionY();
			PRINT(stream1.str());*/

			Update(rateController->GetFrameTime());

			m_isRunning = !m_window->ShouldExit();
			rateController->FrameEnd();
		}
	}

	virtual void Update(double ts) {
		m_CameraController.OnUpdate(ts);

		gswy::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		gswy::RenderCommand::Clear();

		gswy::Renderer2D::BeginScene(m_CameraController.GetCamera());
		gswy::Renderer2D::DrawQuad(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec2(1.0f, 1.0f), 0.0f, m_Texture);
		gswy::Renderer2D::EndScene();
	}

protected:

private:
	gswy::OrthographicCameraController m_CameraController;
	std::shared_ptr<gswy::Texture2D> m_Texture;
};

Engine* gswy::CreateEngineApplication() {
	return new Application();
}

