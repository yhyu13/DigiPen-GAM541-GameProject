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
#include "ui/GameWidgetManager.h"

using namespace gswy;

namespace gswy
{
	float GSWY_GetPixel2WorldNumerator()
	{
		return GSWY_GetWindowHeight() * 0.5f;
	}
}

class GameLayer : public Layer {

public:

	GameLayer()
	{
		InitFramework();
		InitGameWorld();
	}

	virtual ~GameLayer() 
	{
		GameTileMapManager::GetInstance()->Shutdown();
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
		m_PostProcessing.SetScreenSize(1280, 720);
		m_PostProcessing.Init();
		GameTileMapManager::GetInstance()->Init();

		// Texture loader
		ResourceAllocator<Texture2D>::GetInstance()->Init();
		// Animation loader
		ResourceAllocator<Animation>::GetInstance()->Init();
		// TileMap loader
		ResourceAllocator<TileMap>::GetInstance()->Init();

		LoadResources();
#ifdef _DEBUG
		//TODO: Move to Component Init
		m_Particle.ColorBegin = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
		m_Particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 0.0f };
		m_Particle.SizeBegin = 0.01f, m_Particle.SizeVariation = 0.0f, m_Particle.SizeEnd = 0.0f;
		m_Particle.LifeTime = 0.1f;
		m_Particle.Velocity = { 0.0f, 0.0f, 0.0f };
		m_Particle.VelocityVariation = { 0.0f, 0.0f, 0.0f };
		m_Particle.Position = { 0.0f, 0.0f, 0.0f };
		m_Particle.Speed = { 1.0f, 1.0f, 0.0f };
#endif // _DEBUG
	}

	void LoadResources()
	{
		TIME("Loading Resources");

		GameObjectFactory* factory = GameObjectFactory::GetInstance();
		factory->LoadResources("./asset/archetypes/resources.json");
		// TODO : remove loading map test
		ResourceAllocator<TileMap>::GetInstance()->Create("./asset/untitled.json", "untitled");
	}

	void InitGameWorld()
	{
		TIME("Initializing Game World");

		///////// EXAMPLE SETUP FOR TESTING ECS /////////////
		m_world = MemoryManager::Make_shared<GameWorld<GameObjectType>>();

		GameObjectFactory* factory = GameObjectFactory::GetInstance();
		std::vector<std::string> systems = factory->GetSystems("./asset/archetypes/systems.json");

		for (int i = 0; i < systems.size(); ++i) {
			std::string system = systems[i];
			if (system._Equal("player-controller")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<PlayerControllerComSys>());
				continue;
			}
			if (system._Equal("PlayerAnimationController")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<PlayerAnimationControllerComSys>());
				continue;
			}
			if (system._Equal("mob-1-controller")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<Mob1ControllerComSys>());
				continue;
			}
			if (system._Equal("tower-controller")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<TowerControllerComSys>());
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
			if (system._Equal("fade")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<FadeComSys>());
				continue;
			}
			if (system._Equal("gc")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<GCComSys>());
				continue;
			}
			if (system._Equal("attached-movement")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<AttachedMovementComSys>());
				continue;
			}
			if (system._Equal("particle")) {
				m_world->RegisterSystem(MemoryManager::Make_shared<ParticleComSys>());
				continue;
			}
		}

		// Initialize game
		m_world->Init();

		LoadGameWorld();
	}

	void LoadGameWorld()
	{
		ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/untitled.png","untitled");

		// TODO : use proper reflection to handle map loading and background creation
		auto obj = m_world->GenerateEntity(GameObjectType::BACKGROUND);
		auto sprite = SpriteCom();
		auto m_sprite = sprite.Get();
		m_sprite->SetSpriteScale(vec2(100.f * 32.f / GSWY_GetPixel2WorldNumerator(), 100.f * 32.f / GSWY_GetPixel2WorldNumerator()));
		m_sprite->SetSpriteTexture(ResourceAllocator<Texture2D>::GetInstance()->Get("untitled"));
		m_sprite->SetSpritePosition(vec3(49.5f * 32.f / GSWY_GetPixel2WorldNumerator(), -49.5f * 32.f / GSWY_GetPixel2WorldNumerator(), -0.5));
		m_sprite->SetSpriteRotation(0);
		obj.AddComponent(sprite);

		// call load level here
		// object factory must be an abstract class in the engine and must be implemented in application
		GameObjectFactory* factory = GameObjectFactory::GetInstance();
		factory->LoadLevel("./asset/archetypes/levels/sample-level.json", m_world);

		//MOUSE SETUP
		auto mouse = m_world->GenerateEntity(GameObjectType::MOUSE);
		auto mouseTr = TransformCom();
		mouseTr.SetPos(InputManager::GetInstance()->GetCursorViewPosition());
		mouse.AddComponent(mouseTr);
		auto mousebody = BodyCom();
		mousebody.SetMass(0);
		mousebody.ChooseShape("AABB", 0.05, 0.05);
		mouse.AddComponent(mousebody);
		auto ownership = OwnershiptCom<GameObjectType>();
		mouse.AddComponent(ownership);

		GameTileMapManager::GetInstance()->AddTileMap("untitled");
		GameTileMapManager::GetInstance()->SetCurrentMapName("untitled");
		GameTileMapManager::GetInstance()->LoadCurrentTileMap(m_world);
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


	virtual void PreRenderUpdate(double ts)
	{
		{
			m_world->PreRenderUpdate(ts);
		}
	}

	virtual void PostRenderUpdate(double ts)
	{
		{
			m_world->PostRenderUpdate(ts);
		}
	}

	void UpdateCamera(double ts)
	{
		{
			// Floating camera that centered around the player character
			ComponentDecorator<TransformCom, GameObjectType> transform;
			m_world->Unpack(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0], transform);
			auto cursor = InputManager::GetInstance()->GetCursorPosition();
			auto center = InputManager::GetInstance()->GetCursorMaxPosition() * 0.5f;
			auto delta = vec2(0);
			auto len = glm::length(cursor - center);
			if (len > 1e-2)
			{
				delta = glm::normalize(cursor - center) * (float)ts * ((len > 30.0f) ? 30.0f : len);
			}
			auto targetPos = transform->GetPos3D() + vec3(delta.x, -delta.y, 0.0f);
			auto newPos = m_CameraController.GetPosition() + (targetPos - m_CameraController.GetPosition()) * m_CameraController.GetCameraMoveSpeed() * (float)ts;
			m_CameraController.SetPosition(newPos);
			m_CameraController.SetZoomLevel(2);
		}
		{
			// Update cursor world position
			ComponentDecorator<TransformCom, GameObjectType> position;
			m_world->Unpack(m_world->GetAllEntityWithType(GameObjectType::MOUSE)[0], position);
			auto cameraPos = m_CameraController.GetPosition();
			auto mouseRelativePos = InputManager::GetInstance()->GetCursorViewPosition();
			// Caution: 
			// It could be wrong, using debug draw to make sure the mouse entity is attached to the cursor
			auto zoomLevel = m_CameraController.GetZoomLevel();
			position->SetPos(vec2(cameraPos.x + zoomLevel *mouseRelativePos.x, cameraPos.y + zoomLevel *mouseRelativePos.y));
		}
		AudioManager::GetInstance()->Set3dListenerAndOrientation(m_CameraController.GetPosition());
		m_CameraController.OnUpdate(ts);
	}

	void Render(double ts)
	{
		//if(m_PP) m_PostProcessing.Bind();
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		RenderCommand::Clear();

		Renderer2D::BeginBatch(m_CameraController.GetCamera());
		//Renderer2D::AddBatch(glm::vec3(0.0f, 0.0f, 0.0f), { 0.1f, 0.1f }, { 1.0f, 0.0f, 0.0f, 1.0f });
		Renderer2D::DrawQuad(glm::vec3(0.5f), glm::vec2(0.1f), 0.0f, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		Renderer2D::EndBatch();
		Renderer2D::DrawBatch();
		Renderer2D::BeginScene(m_CameraController.GetCamera());
		m_world->Render(ts);
		// m_world render
#ifdef _DEBUG

		//TODO: Move to Component Update
		//if (m_ParticleActive)
		//{
		//	for (int i = 0; i < 5; i++)
		//		m_ParticleSystem.Emit(m_Particle);
		//}
		//
		//m_ParticleSystem.Update(ts);
		//m_ParticleSystem.Render();

#endif // _DEBUG
		//if (m_PP)
		//{
		//	m_PostProcessing.Unbind();
		//	m_PostProcessing.Render(ts);
		//}
		Renderer2D::EndScene();
	}

	virtual void OnUpdate(double ts) override
	{
		BeforeFrame();
		if (InputManager::GetInstance()->IsKeyTriggered(KEY_F2)) m_PP = !m_PP;
		{
			TIME("Camera Update");
			UpdateCamera(ts);
		}
		std::future<void> update = std::async(std::launch::async, [this, ts]()
		{

		});	

		{
			TIME("System Update");
			Update(ts);
		}
		{
			TIME("PreRender Update");
			PreRenderUpdate(ts);
		}
		{
			TIME("Render Update");
			Render(ts);
		}
		update.wait();
		EventQueue<GameObjectType, EventType>::GetInstance()->Update(ts);
		{
			TIME("PostRender Update");
			PostRenderUpdate(ts);
		}
		AfterFrame();
	}

	virtual void OnImGuiRender() override
	{
		//Rendering all widgets
		WidgetManager::GetInstance()->RenderUI();

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
#ifdef _DEBUG
		//TODO: Debug only
		//ImGui::Begin("Settings");
		//ImGui::Checkbox("ParticleActive", &m_ParticleActive);
		//ImGui::SliderFloat("LifeTime", &m_Particle.LifeTime, 0.0f, 1.0f);
		//ImGui::ColorEdit4("Birth Color", glm::value_ptr(m_Particle.ColorBegin));
		//ImGui::ColorEdit4("End Color", glm::value_ptr(m_Particle.ColorEnd));
		//ImGui::SliderFloat("SizeBegin", &m_Particle.SizeBegin, 0.0f, 1.0f);
		//ImGui::SliderFloat("SizeEnd", &m_Particle.SizeEnd, 0.0f, 1.0f);
		//ImGui::SliderFloat("SizeVariation", &m_Particle.SizeVariation, 0.0f, 1.0f);
		//ImGui::SliderFloat3("Velocity", glm::value_ptr(m_Particle.Velocity), -1.0f, 1.0f);
		//ImGui::SliderFloat3("VelocityVariation", glm::value_ptr(m_Particle.VelocityVariation), -1.0f, 1.0f);
		//ImGui::SliderFloat2("Speed", glm::value_ptr(m_Particle.Speed), 0.0f, 1.0f);
		//ImGui::End();
#endif // _DEBUG

	}

	static const vec3& GetCameraPosition()
	{
		return m_CameraController.GetPosition();
	}

protected:

private:
	static OrthographicCameraController m_CameraController;
	std::shared_ptr<GameWorld<GameObjectType>> m_world;
	gswy::OpenGLPostProcessing m_PostProcessing;
	bool m_PP = false;
#ifdef _DEBUG

	//TODO Move to component
	gswy::ExplosionParticle m_ParticleSystem;
	gswy::Particle m_Particle;
	bool m_ParticleActive = true;
#endif // _DEBUG

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