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
#include "Import.h"

using namespace gswy;

class Application : public Engine {

public:

	Application() 
		: m_CameraController(1280.0f / 720.0f)
	{
		InitFramework();
		InitGameWorld();
	}

	virtual ~Application() {
	}

	void InitFramework()
	{
		// Renderer
		Renderer2D::Init();
		// Texture loader
		ResourceAllocator<Texture2D>::GetInstance()->Init();
		// Animation loader
		ResourceAllocator<Animation>::GetInstance()->Init();
		// Frame rate controller
		m_rateController = FramerateController::GetInstance(60);

		LoadResources();
	}

	void LoadResources()
	{
		// Texture loader
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/GAM541_Char1_Moving_Upper_Unarmed.png", "PlayerMovingUnarmed");
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/background3.png", "Background3");
		// Animation loader
		auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation1.txt", "PlayerAnimation1");
		for (int i = 0; i < 8; ++i)
		{
			playerAnim1->AddFrame("PlayerMovingUnarmed", 59 * i, 32 * 0, 59, 32, 1.0 / 15.0);
		}
		AudioManager::GetInstance()->LoadSound("./asset/breakout.mp3", true, true, false);
	}

	void InitGameWorld()
	{
		///////// EXAMPLE SETUP FOR TESTING ECS /////////////
		m_world = MemoryManager::Make_shared<GameWorld<GameObjectType>>();

		// Add systems
		m_world->RegisterSystem(MemoryManager::Make_shared<PlayerControllerComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<SceneComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<SpriteComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<AnimationComSys>());

		// Initialize game
		m_world->Init();

		LoadGameWorld();
	}

	void LoadGameWorld()
	{
		auto background = m_world->GenerateEntity(GameObjectType::ENEMY);
		background.AddComponent(TransformCom(0, 0, 0));
		auto sprite0 = SpriteCom();
		sprite0.SetTexture("Background3");
		background.AddComponent(sprite0);

		auto player = m_world->GenerateEntity(GameObjectType::PLAYER);
		player.AddComponent(TransformCom(0, 0, 1));
		player.AddComponent(SpriteCom());
		auto animCom = AnimationCom();
		animCom.Add("PlayerAnimation1", "Move");
		animCom.SetCurrentAnimationState("Move");
		player.AddComponent(animCom);

		auto enemy = m_world->GenerateEntity(GameObjectType::ENEMY);
		enemy.AddComponent(TransformCom(1, 0, 1));
		enemy.AddComponent(SpriteCom());
		auto animCom2 = AnimationCom();
		animCom2.Add("PlayerAnimation1", "Move");
		animCom2.SetCurrentAnimationState("Move");
		enemy.AddComponent(animCom2);
	}

	void BeforeRun()
	{
		AudioManager::GetInstance()->PlaySound("./asset/breakout.mp3", AudioVector3{ 0, 0, 0 }, 1);
	}

	void AfterRun()
	{

	}

	void BeforeFrame()
	{
		m_rateController->FrameStart();
	}

	void AfterFrame()
	{
		m_rateController->FrameEnd();
	}

	bool IsRunning()
	{
		return isRunning;
	}

	void UpdateRunning()
	{
		if (isRunning)
			isRunning = !window->ShouldExit();
	}

	virtual void Update(double ts) {
		{
			Engine::Update(ts); // Do not delete this line!
		}
		{
			// Manger update
			InputManager::GetInstance()->Update(ts);
		}
		{
			// m_world update
			m_world->Update(ts);
		}
	}

	void UpdateCamera(double ts)
	{
		{
			// Floating camera that centered around the player character
			ComponentDecorator<TransformCom, GameObjectType> position;
			m_world->Unpack(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0], position);
			auto cursor = vec2(InputManager::GetInstance()->GetMousePositionX(), InputManager::GetInstance()->GetMousePositionY());
			auto center = vec2(InputManager::GetInstance()->GetMouseMaxPositionX() / 2, InputManager::GetInstance()->GetMouseMaxPositionY() / 2);
			auto len = glm::length(cursor - center);
			auto delta = glm::normalize(cursor - center) * ((len > 50) ? 50 : len) * 0.01f;
			m_CameraController.SetPosition(position->GetPos() + glm::vec3(delta.x, -delta.y, 0));
		}
		m_CameraController.OnUpdate(ts);
	}

	void Render()
	{
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		Renderer2D::BeginScene(m_CameraController.GetCamera());
		// m_world render
		m_world->Render();

		Renderer2D::EndScene();
	}

	virtual void Run() override
	{
		BeforeRun();
		while (IsRunning()) {
			BeforeFrame();
			{
				{
					// Engine update
					Update(m_rateController->GetFrameTime());
				}
				{
					UpdateCamera(m_rateController->GetFrameTime());
				}
				{
					Render();
				}
				UpdateRunning();
			}
			AfterFrame();
		}
		AfterRun();
	}

protected:

private:
	OrthographicCameraController m_CameraController;
	FramerateController* m_rateController;
	std::shared_ptr<GameWorld<GameObjectType>> m_world;
};

namespace gswy
{
	Engine* CreateEngineApplication() {
		return new Application();
	}
}

