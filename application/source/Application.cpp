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
#include "ExplosionParticle.h"
#include "object-factory/GameObjectFactory.h"

using namespace gswy;

class GameLayer : public Layer {

public:

	GameLayer()
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

		//TODO: Move to Component Init
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
		m_Particle.SizeBegin = 0.01f, m_Particle.SizeVariation = 0.0f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 0.1f;
		m_Particle.Velocity = { 0.0f, 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 0.0f, 0.0f, 0.0f };
		m_Particle.Position = { 0.0f, 0.0f, 0.0f };
		m_Particle.Speed = { 1.0f, 1.0f, 0.0f };
	}

	void LoadResources()
	{
		TIME("Loading Resources");

		//ObjectFactory* factory = ObjectFactory::GetInstance();
		GameObjectFactory* factory = GameObjectFactory::GetInstance();
		factory->LoadResources("./asset/archetypes/resources.json");

		// Texture loader
		//ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/PlayerMovingUnarmed.png", "PlayerMovingUnarmed");
		//ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/MobMovingUnarmed.png", "MobMovingUnarmed");
		//ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/background3.png", "Background3");
		//std::string fireBallPath = "./asset/TopDownGunPack/TopDownGunPack/Sprites/Flames/Fireball\ 2/";
		//ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0001.png", "ffireball_0001");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0002.png", "ffireball_0002");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0003.png", "ffireball_0003");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0004.png", "ffireball_0004");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0005.png", "ffireball_0005");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(fireBallPath + "ffireball_0006.png", "ffireball_0006");

		//std::string boltPath = "./asset/TopDownGunPack/TopDownGunPack/Sprites/Lightning/bolt/";
		//ResourceAllocator<Texture2D>::GetInstance()->Create(boltPath + "bolt_tesla_0001.png", "bolt_tesla_0001");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(boltPath + "bolt_tesla_0002.png", "bolt_tesla_0002");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(boltPath + "bolt_tesla_0003.png", "bolt_tesla_0003");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(boltPath + "bolt_tesla_0004.png", "bolt_tesla_0004");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(boltPath + "bolt_tesla_0005.png", "bolt_tesla_0005");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(boltPath + "bolt_tesla_0006.png", "bolt_tesla_0006");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(boltPath + "bolt_tesla_0007.png", "bolt_tesla_0007");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(boltPath + "bolt_tesla_0008.png", "bolt_tesla_0008");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(boltPath + "bolt_tesla_0009.png", "bolt_tesla_0009");

		//std::string iceBallPath = "./asset/TopDownGunPack/TopDownGunPack/Sprites/Ice/Ice\ Ball/";
		//ResourceAllocator<Texture2D>::GetInstance()->Create(iceBallPath + "iceball_0001.png", "iceball_0001");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(iceBallPath + "iceball_0002.png", "iceball_0002");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(iceBallPath + "iceball_0003.png", "iceball_0003");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(iceBallPath + "iceball_0004.png", "iceball_0004");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(iceBallPath + "iceball_0005.png", "iceball_0005");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(iceBallPath + "iceball_0006.png", "iceball_0006");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(iceBallPath + "iceball_0007.png", "iceball_0007");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(iceBallPath + "iceball_0008.png", "iceball_0008");
		//ResourceAllocator<Texture2D>::GetInstance()->Create(iceBallPath + "iceball_0009.png", "iceball_0009");


		//// Animation loader
		//auto playerAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation1.txt", "PlayerAnimation1");
		//for (int i = 0; i < 8; ++i)
		//{
		//	playerAnim1->AddFrame("PlayerMovingUnarmed", 59 * i, 32 * 0, 59, 32, 1.0 / 15.0);
		//}
		//auto mobAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/PlayerAnimation1.txt", "MobAnimation1");
		//for (int i = 0; i < 8; ++i)
		//{
		//	mobAnim1->AddFrame("MobMovingUnarmed", 0, 50 * i, 70, 50, 1.0 / 15.0);
		//}
		//auto fireBallAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/fireBallAnim1.txt", "fireBallAnim1");
		//for (int i = 1; i <= 6; ++i)
		//{
		//	fireBallAnim1->AddFrame("ffireball_000" + Str(i), 0, 0, 64, 64, 1.0 / 15.0);
		//}
		//auto boltAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/fireBallAnim1.txt", "boltAnim1");
		//for (int i = 1; i <= 9; ++i)
		//{
		//	boltAnim1->AddFrame("bolt_tesla_000" + Str(i), 0, 0, 512, 512, 1.0 / 30.0);
		//}
		//auto iceBallAnim1 = ResourceAllocator<Animation>::GetInstance()->Create("./asset/fireBallAnim1.txt", "iceBallAnim1");
		//for (int i = 1; i <= 9; ++i)
		//{
		//	iceBallAnim1->AddFrame("iceball_000" + Str(i), 0, 0, 64, 64, 1.0 / 15.0);
		//}
		//// Audio loader
		//AudioManager::GetInstance()->LoadSound("./asset/breakout.mp3", true);
		//AudioManager::GetInstance()->LoadSound("./asset/TopDownGunPack/TopDownGunPack/Audio/FootSteps/footstep02.ogg", false);
	}

	void InitGameWorld()
	{
		TIME("Initializing Game World");

		///////// EXAMPLE SETUP FOR TESTING ECS /////////////
		m_world = MemoryManager::Make_shared<GameWorld<GameObjectType>>();

		//ObjectFactory* factory = ObjectFactory::GetInstance();
		GameObjectFactory* factory = GameObjectFactory::GetInstance();
		std::vector<std::string> systems = factory->GetSystems("./asset/archetypes/systems.json");

		for (int i = 0; i < systems.size(); ++i) {
			std::string system = systems[i];
			if (system._Equal("player-controller")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<PlayerControllerComSys>());
				continue;
			}
			if (system._Equal("mob-1-controller")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<Mob1ControllerComSys>());
				continue;
			}
			if (system._Equal("scene")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<SceneComSys>());
				continue;
			}
			if (system._Equal("sprite")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<SpriteComSys>());
				continue;
			}
			if (system._Equal("animation")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<AnimationComSys>());
				continue;
			}
			if (system._Equal("physics")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<PhysicsComSys>());
				continue;
			}
			if (system._Equal("weapon")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<WeaponComSys>());
				continue;
			}
			if (system._Equal("lifetime")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<LifeTimeComSys>());
				continue;
			}
			if (system._Equal("spawn")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<SpawningComSys>());
				continue;
			}
			if (system._Equal("sound")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<SoundComSys>());
				continue;
			}
			if (system._Equal("hitpoint")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<HitPointComSys>());
				continue;
			}
			if (system._Equal("death")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<DeathComSys>());
				continue;
			}
			if (system._Equal("gcc")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<GCComSysy>());
				continue;
			}
			if (system._Equal("attached-movement")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<AttachedMovementComSys>());
				continue;
			}
		}

		// Add systems
		/*m_world->RegisterSystem(MemoryManager::Make_shared<PlayerControllerComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<Mob1ControllerComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<SceneComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<SpriteComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<AnimationComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<PhysicsComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<WeaponComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<LifeTimeComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<SpawningComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<SoundComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<HitPointComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<DeathComSys>());
		m_world->RegisterSystem(MemoryManager::Make_shared<GCComSysy>());
		m_world->RegisterSystem(MemoryManager::Make_shared<AttachedMovementComSys>());*/

		// Initialize game
		m_world->Init();

		LoadGameWorld();
	}

	void LoadGameWorld()
	{
		// call load level here
		// object factory must be an abstract class in the engine and must be implemented in application

		auto background = m_world->GenerateEntity(GameObjectType::BACKGROUND);
		background.AddComponent(TransformCom(0, 0, -0.5));
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
		player.AddComponent(HitPointCom());
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
			auto targetPos = position->GetPos3D() + vec3(delta.x, -delta.y, 0.0f);
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

		//TODO: Move to Component Update
		if (m_ParticleActive)
		{
			for (int i = 0; i < 5; i++)
				m_ParticleSystem.Emit(m_Particle);
		}

		m_ParticleSystem.Update(ts);
		m_ParticleSystem.Render();

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

		ImGui::Begin("Settings");
		ImGui::Checkbox("ParticleActive", &m_ParticleActive);
		ImGui::SliderFloat("LifeTime", &m_Particle.LifeTime, 0.0f, 1.0f);
		ImGui::ColorEdit4("Birth Color", glm::value_ptr(m_Particle.ColorBegin));
		ImGui::ColorEdit4("End Color", glm::value_ptr(m_Particle.ColorEnd));
		ImGui::SliderFloat("SizeBegin", &m_Particle.SizeBegin, 0.0f, 1.0f);
		ImGui::SliderFloat("SizeEnd", &m_Particle.SizeEnd, 0.0f, 1.0f);
		ImGui::SliderFloat("SizeVariation", &m_Particle.SizeVariation, 0.0f, 1.0f);
		ImGui::SliderFloat3("Velocity", glm::value_ptr(m_Particle.Velocity), -1.0f, 1.0f);
		ImGui::SliderFloat3("VelocityVariation", glm::value_ptr(m_Particle.VelocityVariation), -1.0f, 1.0f);
		ImGui::SliderFloat2("Speed", glm::value_ptr(m_Particle.Speed), 0.0f, 1.0f);
		ImGui::End();
	}

	static const vec3& GetCameraPosition()
	{
		return m_CameraController.GetPosition();
	}

protected:

private:
	static OrthographicCameraController m_CameraController;
	std::shared_ptr<GameWorld<GameObjectType>> m_world;

	//TODO Move to component
	gswy::ExplosionParticle m_ParticleSystem;
	gswy::Particle m_Particle;
	bool m_ParticleActive = true;
};

OrthographicCameraController GameLayer::m_CameraController(1280.0f / 720.0f);

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