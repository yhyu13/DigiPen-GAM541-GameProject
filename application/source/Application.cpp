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

enum GameObjectType {
	PLAYER,
	ENEMY
};

struct Position : gswy::BaseComponent<Position> {
	Position() = default;
	Position(float _x) : x(_x) {
		value = &x;
		x_ptr = std::make_shared<float>(x + 2);
	};
	float x;

	float* value;
	std::shared_ptr<float> x_ptr;
};

struct Transform : gswy::BaseComponent<Transform> {
	Transform() = default;
	Transform(float x, float y) : x(x), y(y) {};
	float x;
	float y;
};

class Wind : public gswy::BaseComponentSystem<GameObjectType> {
public:
	Wind() {
		m_systemSignature.AddComponent<Position>();
		m_systemSignature.AddComponent<Transform>();
	}

	virtual void Update(double dt) override {
		for (auto& entity : m_registeredEntities) {
			gswy::ComponentDecorator<Position, GameObjectType> position;
			gswy::ComponentDecorator<Transform, GameObjectType> transform;
			m_parentWorld->Unpack(entity, position);
			m_parentWorld->Unpack(entity, transform);

			// Move 1 every second
			position->x += 1.0f * (dt / 1000.0f);

			// Print entity position
			//std::cout << "Entity " << entity.m_id << ": " << position->x << "	" << *(position->value) << "	" << *(position->x_ptr) <<std::endl;
			std::cout << "Entity: " << entity.m_type <<"  " << entity.m_id << ": " << position->x << " : " << transform->x << " : " << transform->y <<std::endl; // have to override -> operator
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
		std::shared_ptr<gswy::EntityManager<GameObjectType>> entityManager = std::make_shared<gswy::EntityManager<GameObjectType>>();
		std::shared_ptr<GameWorld<GameObjectType>> world = std::make_shared<gswy::GameWorld<GameObjectType>>(entityManager);
		//auto world = std::make_unique<gswy::GameWorld<GameObjectType>>(std::move(entityManager));

		// Add systems
		std::shared_ptr<gswy::BaseComponentSystem<GameObjectType>> wind = std::make_shared<Wind>();
		world->RegisterSystem(wind);

		// Initialize game
		world->Init();

		// Add an entity with a position
		auto tumbleweed = world->GenerateEntity(GameObjectType::PLAYER);
		Position position(0);
		tumbleweed.AddComponent(position);
		tumbleweed.AddComponent(Transform(1, 2));

		auto tumbleweed2 = world->GenerateEntity(GameObjectType::ENEMY);
		Position position2(10);
		tumbleweed2.AddComponent(position2);
		tumbleweed2.AddComponent(Transform(10, 20));

		auto tumbleweed3 = world->GenerateEntity(GameObjectType::ENEMY);
		Position position3(100);
		tumbleweed3.AddComponent(position3);
		tumbleweed3.AddComponent(Transform(100, 200));

		// Run game for "1 second at 50fps"
		for (int i = 0; i < 50; i++) {
			world->Update(20);
		}
		tumbleweed2.RemoveComponent<Transform>();

		std::cout << "\n\n\n\n\n";

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

