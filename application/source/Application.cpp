/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Application.cpp
Purpose			: Game application class. Creates an instance of the game application which plugs into the engine.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519), 
				  Hang Yu (hang.yu@digipen.edu | 60001119), 
				  Kyle Wang (kyle.wang@digipen.edu | 60000719),
				  Taksh Goyal (taksh.goyal@digipen.edu | 60001319)
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
		OpenGLDebugDraw::Init();
		// Texture loader
		ResourceAllocator<Texture2D>::GetInstance()->Init();
		// Animation loader
		ResourceAllocator<Animation>::GetInstance()->Init();

		LoadResources();
	}

	void LoadResources()
	{
		TIME("Loading Resources");

		// Texture loader
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/PlayerMovingUnarmed.png", "PlayerMovingUnarmed");
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/background3.png", "Background3");
		std::string fireBallPath = "./asset/TopDownGunPack/TopDownGunPack/Sprites/Flames/Fireball\ 2/";
		ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0001.png", "ffireball_0001");
		ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0002.png", "ffireball_0002");
		ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0003.png", "ffireball_0003");
		ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0004.png", "ffireball_0004");
		ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0005.png", "ffireball_0005");
		ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0006.png", "ffireball_0006");

		// Animation loader
		auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation1.txt", "PlayerAnimation1");
		for (int i = 0; i < 8; ++i)
		{
			playerAnim1->AddFrame("PlayerMovingUnarmed", 59 * i, 32 * 0, 59, 32, 1.0 / 15.0);
		}
		auto fireBallAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/fireBallAnim1.txt", "fireBallAnim1");
		for (int i = 1; i <= 6; ++i)
		{
			fireBallAnim1->AddFrame("ffireball_000" + Str(i), 0, 0, 64, 64, 1.0 / 15.0);
		}
		// Audio loader
		AudioManager::GetInstance()->LoadSound("./asset/breakout.mp3", true);
		AudioManager::GetInstance()->LoadSound("./asset/TopDownGunPack/TopDownGunPack/Audio/FootSteps/footstep02.ogg", false);
	}

	void InitGameWorld()
	{
		TIME("Initializing Game World");

		///////// EXAMPLE SETUP FOR TESTING ECS /////////////
		m_world = MemoryManager::Make_shared<GameWorld<GameObjectType>>();

		// Add systems
		m_world->RegisterSystem(MemoryManager::Make_shared<PlayerControllerComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<Mob1ControllerComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<SceneComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<SpriteComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<AnimationComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<PhysicsComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<WeaponComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<LifeTimeComSys>());

		// Initialize game
		m_world->Init();

		LoadGameWorld();
	}

	void LoadGameWorld()
	{
		auto background = m_world->GenerateEntity(GameObjectType::BACKGROUND);
		background.AddComponent(TransformCom(0, 0, Z_ORDER(0)));
		auto sprite0 = SpriteCom();
		sprite0.SetTexture("Background3");
		sprite0.SetScale(vec2(5));
		background.AddComponent(sprite0);

		auto player = m_world->GenerateEntity(GameObjectType::PLAYER);
		player.AddComponent(OwnershiptCom<GameObjectType>());
		player.AddComponent(TransformCom(0, 0, Z_ORDER(1)));
		auto sprite1 = SpriteCom();
		sprite1.SetScale(vec2(0.25, 0.25 / 59 *32));
		player.AddComponent(sprite1);
		auto animCom1 = AnimationCom();
		animCom1.Add("PlayerAnimation1", "Move");
		animCom1.SetCurrentAnimationState("Move");
		player.AddComponent(animCom1);
		auto aabb1 = BodyCom();
		aabb1.ChooseShape("AABB", 0.25, 0.25 / 59 * 32);
		player.AddComponent(aabb1);

		auto enemy = m_world->GenerateEntity(GameObjectType::ENEMY);
		enemy.AddComponent(OwnershiptCom<GameObjectType>());
		enemy.AddComponent(TransformCom(1, 0, Z_ORDER(2)));
		enemy.AddComponent(SpriteCom());
		auto animCom2 = AnimationCom();
		animCom2.Add("PlayerAnimation1", "Move");
		animCom2.SetCurrentAnimationState("Move");
		enemy.AddComponent(animCom2);
		auto aabb2 = BodyCom();
		aabb2.ChooseShape("AABB", 1,1);
		enemy.AddComponent(aabb2);
	}

	void BeforeRun()
	{
		AudioManager::GetInstance()->PlaySound("breakout", AudioVector3{ 0, 0, 0 }, 1, 1);
	}

	void AfterRun()
	{
		OpenGLDebugDraw::End();
	}

	void BeforeFrame()
	{
	}

	void AfterFrame()
	{
	}

	virtual void Update(double ts) 
	{
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
			auto cursor = InputManager::GetInstance()->GetCursorPosition();
			auto center = InputManager::GetInstance()->GetCursorMaxPosition() * 0.5f;
			auto len = glm::length(cursor - center);
			auto delta = glm::normalize(cursor - center) * (float)ts * ((len > 30.0f) ? 30.0f: len);
			auto targetPos = position->GetPos() + vec3(delta.x, -delta.y, 0.0f);
			auto newPos = m_CameraController.GetPosition() + (targetPos - m_CameraController.GetPosition()) * m_CameraController.GetCameraMoveSpeed() * (float)ts;
			m_CameraController.SetPosition(newPos);
		}
		m_CameraController.OnUpdate(ts);
	}


	void Render(double ts)
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
			TIME("System Update");
			Update(ts);
		}
		{
			TIME("Camera Update");
			UpdateCamera(ts);
		}
		{
			TIME("Render Update");
			Render(ts);
		}
		AfterFrame();
	}

	virtual void OnImGuiRender() override
	{
#ifdef _DEBUG

		Instrumentor* instrumentor = Instrumentor::GetInstance();
		ImGui::SetNextWindowBgAlpha(0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, 0);
		ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, 0);
		ImGui::Begin("Instrumenting Profiling");
		for (auto& result : instrumentor->GetResults()) {
			char entry[100];
			strcpy(entry, "%10.3f %s\t");
			strcat(entry, result.first);
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), entry, result.second.m_time, result.second.m_timeUnit);
		}
		ImGui::End();
		ImGui::PopStyleVar(1);
		ImGui::PopStyleColor(3);
#endif
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