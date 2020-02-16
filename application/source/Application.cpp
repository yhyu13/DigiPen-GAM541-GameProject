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
#include "imgui/imgui.h"
#include <glm/gtc/type_ptr.hpp>

using namespace gswy;

class GameLayer : public Layer {

public:

	GameLayer()
		: m_CameraController(1280.0f / 720.0f)
	{
		InitFramework();
		InitGameWorld();
	}

	virtual ~GameLayer() 
	{
	}

	virtual void OnAttach() 
	{
		BeforeRun();
	}
	virtual void OnDetach() 
	{
		AfterRun();
	}

	void InitFramework()
	{
		// Renderer
		Renderer2D::Init();
		// Texture loader
		ResourceAllocator<Texture2D>::GetInstance()->Init();
		// Animation loader
		ResourceAllocator<Animation>::GetInstance()->Init();

		LoadResources();
	}

	void LoadResources()
	{
		// Texture loader
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/GAM541_Char1_Moving_Upper_Unarmed.png", "GAM541_Char1_Moving_Upper_Unarmed");
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/background3.png", "Background3");
		// Animation loader
		auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation1.txt", "PlayerAnimation1");
		for (int i = 0; i < 8; ++i)
		{
			playerAnim1->AddFrame("GAM541_Char1_Moving_Upper_Unarmed", 59 * i, 32 * 0, 59, 32, 1.0 / 15.0);
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
	}

	void AfterFrame()
	{
	}

	virtual void Update(double ts) {
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
		ComponentDecorator<TransformCom, GameObjectType> position;
		m_world->Unpack(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0], position);
		m_CameraController.SetPosition(glm::vec3(position->m_x, position->m_y, position->m_z));
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

	virtual void OnUpdate(double ts) override
	{
		
		BeforeFrame();
		{
			{
				// Engine update
				Update(ts);
			}
			{
				UpdateCamera(ts);
			}
			{
				Render();
			}
		}
		AfterFrame();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_Color));
		ImGui::End();
	}

protected:

private:
	OrthographicCameraController m_CameraController;
	std::shared_ptr<GameWorld<GameObjectType>> m_world;
	glm::vec3 m_Color;
};

class Application : public Engine {
public:
	Application()
	{
		PushLayer(new GameLayer());
	}

	~Application()
	{
	}
};

namespace gswy
{
	Engine* CreateEngineApplication() {
		return new Application();
	}
}

