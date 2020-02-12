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

enum EventType {
	A,
	B,
	C
};

struct CollisionEvent : Event<GameObjectType, EventType> {

	float a;
	int b;
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

EventQueue<GameObjectType, EventType> queue;

class Wind : public gswy::BaseComponentSystem<GameObjectType> {
public:
	Wind() {
		m_systemSignature.AddComponent<Position>();
		m_systemSignature.AddComponent<Transform>();
	}

	virtual void Init() {
		queue.Subscribe<Wind>(this, EventType::A, &Wind::OnEvent);
		queue.Subscribe<Wind>(this, EventType::B, &Wind::OnEvent);
		queue.Subscribe<Wind>(this, EventType::C, &Wind::OnEvent);
	}

	virtual void Update(double dt) override {
		for (auto& entity : m_registeredEntities) {
			gswy::ComponentDecorator<Position, GameObjectType> position;
			gswy::ComponentDecorator<Transform, GameObjectType> transform;
			m_parentWorld->Unpack(entity, position);
			m_parentWorld->Unpack(entity, transform);

			// Move 1 every second
			position->x += 1.0f * (dt / 1000.0f);

			// Print entity information
			//APP_DEBUG("Entity: {0}   {1}: {2} : {3} : {4}", entity.m_type, entity.m_id, position->x, transform->x, transform->y);
		}
	}

	void OnEvent(Event<GameObjectType, EventType>* collision) {

		CollisionEvent* e = dynamic_cast<CollisionEvent*> (collision);
		if (e) {
			APP_DEBUG("a: {0}", e->a);
			APP_DEBUG("b: {0}", e->b);
		}

		if (collision->m_type == EventType::C) {
			CollisionEvent* event = static_cast<CollisionEvent*> (collision);
			APP_ERROR("a: {0}", event->a);
			APP_CRITICAL("b: {0}", event->b);
		}
		APP_TRACE("event-type: {0}", collision->m_type);
		APP_TRACE("entity-1 type: {0}", collision->m_entityA.m_type);
		APP_TRACE("entity-2 type: {0}", collision->m_entityB.m_type);

	}
};

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
		world->RegisterSystem(std::make_shared<Wind>());

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
		animCom.setCurrentAnimationState("MoveUp");
		player.AddComponent(animCom);
		auto entity = player.GetEntity();
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

		APP_DEBUG("\n\nTesting event queue.... start\n");
		Event<GameObjectType, EventType> e;
		e.m_entityA = tumbleweed.GetEntity();
		e.m_entityB = tumbleweed2.GetEntity();
		e.m_type = EventType::A;
		queue.Publish(&e);

		Event<GameObjectType, EventType> e1;
		e1.m_entityA = tumbleweed.GetEntity();
		e1.m_entityB = tumbleweed2.GetEntity();
		e1.m_type = EventType::B;
		queue.Publish(&e1);

		CollisionEvent e2;
		e2.m_entityA = tumbleweed.GetEntity();
		e2.m_entityB = tumbleweed2.GetEntity();
		e2.m_type = EventType::C;
		e2.a = 10.8f;
		e2.b = 10;
		queue.Publish(&e2);

		APP_DEBUG("\n\nTesting event queue.... finished\n");

		tumbleweed2.RemoveComponent<Transform>();

		APP_DEBUG("\n\n\n\n\n");

		// Run game for "1 second at 50fps"
		for (int i = 0; i < 50; i++) {
			world->Update(20);
		}

		while (!m_window->ShouldExit()) {

			rateController->FrameStart();
			{
#ifdef _DEBUG
				std::stringstream stream;
				stream << "Frame Time: " << rateController->GetFrameTime() * 1000 << "ms";
				m_window->UpdateTitle(stream.str());
				ENGINE_DEBUG("Frame Time: {0}", stream.str());
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

