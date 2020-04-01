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
#include "engine/Main.h"

using namespace gswy;

namespace gswy
{
	float GSWY_GetPixel2WorldNumerator()
	{
		return GSWY_GetWindowHeight() * 0.75f;
	}

	class GameLayer : public Layer {

	public:

		GameLayer()
			:
			m_CameraController(GSWY_GetWindowWidth() / GSWY_GetWindowHeight()),
			m_miniMapCameraController(GSWY_GetWindowWidth() / GSWY_GetWindowHeight())
		{
			InitFramework();
			InitGameWorld();
			StartUp();
		}

		virtual ~GameLayer() 
		{
			GameLevelMapManager::GetInstance()->Shutdown();
		}

		virtual void OnAttach() 
		{
			BeforeRun();
		}
		virtual void OnDetach() 
		{
			AfterRun();
		}

		void LoadResources()
		{
			TIME("Loading Resources");

			GameObjectFactory* factory = GameObjectFactory::GetInstance();
			factory->LoadResources("./asset/archetypes/resources.json");
			// TODO: remove Load resources and entities
			ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/SampleLevel1.png", "SampleLevel1");
			ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/SampleLevel2.png", "SampleLevel2");
			ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/SampleLevel3.png", "SampleLevel3");
			// TODO : remove loading map test
			ResourceAllocator<TileMap>::GetInstance()->Create("./asset/SampleLevel1.json", "SampleLevel1");
			ResourceAllocator<TileMap>::GetInstance()->Create("./asset/SampleLevel2.json", "SampleLevel2");
			ResourceAllocator<TileMap>::GetInstance()->Create("./asset/SampleLevel3.json", "SampleLevel3");
			// TODO : consider to move the allocation of minimap texture elsewhere.
			m_miniMapTexture = Texture2D::Create(GSWY_GetWindowWidth(),GSWY_GetWindowHeight());

			InventoryManager* inventoryManager = InventoryManager::GetInstance();
			inventoryManager->LoadInventory("./asset/archetypes/levels/inventory-level-1.json");

			GameLevelMapManager::GetInstance()->ResetLevelData();
			GameLevelMapManager::GetInstance()->AddTileMap("SampleLevel1");
			GameLevelMapManager::GetInstance()->AddTileMap("SampleLevel2");
			GameLevelMapManager::GetInstance()->AddTileMap("SampleLevel3");
		}

		void InitFramework()
		{
			// Renderer
			Renderer2D::Init();
			OpenGLDebugDraw::Init();
			m_PostProcessing.SetScreenSize(1280, 720);
			m_PostProcessing.Init();

			GameLevelMapManager::GetInstance()->Init();

			// Texture loader
			ResourceAllocator<Texture2D>::GetInstance()->Init();
			// Animation loader
			ResourceAllocator<Animation>::GetInstance()->Init();
			// TileMap loader
			ResourceAllocator<TileMap>::GetInstance()->Init();

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

		void InitGameWorld()
		{
			TIME("Initializing Game World");

			///////// EXAMPLE SETUP FOR TESTING ECS /////////////
			m_world = MemoryManager::Make_shared<GameWorld<GameObjectType>>();

			GameObjectFactory* factory = GameObjectFactory::GetInstance();
			factory->LoadSystem("./asset/archetypes/systems.json", m_world);
			
			// Initialize game
			m_world->Init();
		}

		void StartUp()
		{
			// Load logo
			auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/DigiPen_RED_1024px.png", "Logo");
			auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
			auto active = ActiveCom();
			logo.AddComponent(active);
			auto sprite = SpriteCom();
			auto m_sprite = sprite.Get();
			m_sprite->SetSpriteTexture(logoTexture);
			m_sprite->SetSpriteScale(vec2(2, 2.0 / 1024 * 237));
			m_sprite->SetSpritePosition(vec3(0));
			logo.AddComponent(sprite);

			// Load all resources
			LoadResources();

			// Publish start events
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Subscribe<GameLayer>(this, EventType::LOAD_MAIN_MENU, &GameLayer::OnLoadMainMenuWorld);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_GAME_WORLD, &GameLayer::OnLoadGameWorld);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_TEAM_LOGO, &GameLayer::OnLoadTeamLogo);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_GAME_LOGO, &GameLayer::OnLoadGameLogo);

			// Fading logo
			auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, 0.f, 1.f, EventType::GC);
			queue->Publish(_e, 1.0f);
			auto teamLogoEvent = MemoryManager::Make_shared<Event<GameObjectType, EventType>>(EventType::LOAD_TEAM_LOGO);
			queue->Publish(teamLogoEvent, 2.0f);
			auto gameLogoEvent = MemoryManager::Make_shared<Event<GameObjectType, EventType>>(EventType::LOAD_GAME_LOGO);
			queue->Publish(gameLogoEvent, 4.0f);
			auto _e1 = MemoryManager::Make_shared<LoadMainMenuEvent>();
			queue->Publish(_e1, 7.0f);
		}

		void OnLoadTeamLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			// TEAM GSWY PRESENTS
			auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/team-logo.png", "Team-Logo");
			auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
			auto active = ActiveCom();
			logo.AddComponent(active);
			auto sprite = SpriteCom();
			auto m_sprite = sprite.Get();
			m_sprite->SetSpriteTexture(logoTexture);
			m_sprite->SetSpriteScale(vec2(4, 5.0 / 1891 * 822));
			m_sprite->SetSpritePosition(vec3(0));
			logo.AddComponent(sprite);
			auto transform = TransformCom(0, 0, Z_ORDER(1000));
			logo.AddComponent(transform);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
			queue->Publish(_e, 1.0f);
		}

		void OnLoadGameLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			// LIGHT OF EMPYRION
			auto logoTexture1 = ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/game-logo.png", "Game-Logo");
			auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
			auto active = ActiveCom();
			logo.AddComponent(active);
			auto sprite = SpriteCom();
			auto m_sprite = sprite.Get();
			m_sprite->SetSpriteTexture(logoTexture1);
			m_sprite->SetSpriteScale(vec2(4, 5.0 / 1891 * 822));
			m_sprite->SetSpritePosition(vec3(0));
			logo.AddComponent(sprite);
			auto transform = TransformCom(0, 0, Z_ORDER(2000));
			logo.AddComponent(transform);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
			queue->Publish(_e, 1.0f);
		}

		void OnLoadMainMenuWorld(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			LoadMainMenuWorld();
		}

		void LoadMainMenuWorld()
		{
			// Unload current tilemap
			{
				GameLevelMapManager::GetInstance()->UnloadCurrentTileMap(m_world);
			}
			// Clear all entities
			{
				m_world->RemoveAllEntities();
			}
			// Clear level data
			{
				GameLevelMapManager::GetInstance()->ResetLevelData();
			}
			// Set widget
			{
				WidgetManager::GetInstance()->GetHUD().SetVisible(false);
				WidgetManager::GetInstance()->GetInventoryMenu().SetVisible(false);
				WidgetManager::GetInstance()->GetPauseMenu().SetVisible(false);
				WidgetManager::GetInstance()->GetShopMenu().SetVisible(false);
				WidgetManager::GetInstance()->GetMainMenu().SetVisible(true);
			}
			// Load entities
			{
				auto background = m_world->GenerateEntity(GameObjectType::BACKGROUND);
				auto active = ActiveCom();
				background.AddComponent(active);
				auto sprite = SpriteCom();
				auto m_sprite = sprite.Get();
				m_sprite->SetSpriteTexture(ResourceAllocator<Texture2D>::GetInstance()->Get("SampleLevel1"));
				m_sprite->SetSpriteScale(vec2(10, 10));
				m_sprite->SetSpritePosition(vec3(0));
				background.AddComponent(sprite);
			}
			m_CameraController.SetPosition(vec3(0));
			m_miniMapCameraController.SetPosition(vec3(0));
		}

		void OnLoadGameWorld(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = dynamic_pointer_cast<LoadGameWorldEvent>(e))
			{
				LoadGameWorld(event->m_level);
			}
		}

		void LoadGameWorld(int level)
		{
			auto sampleID = Str(level);
			PRINT("Loading map ID " + sampleID);
			{
				m_world->SetPause(false);
			}
			// Clear all entities
			{
				m_world->RemoveAllEntities();
			}
			// Set widget
			{
				WidgetManager::GetInstance()->GetHUD().SetVisible(true);
				WidgetManager::GetInstance()->GetInventoryMenu().SetVisible(false);
				WidgetManager::GetInstance()->GetPauseMenu().SetVisible(false);
				WidgetManager::GetInstance()->GetShopMenu().SetVisible(false);
				WidgetManager::GetInstance()->GetMainMenu().SetVisible(false);
			}
			{
				auto size = ResourceAllocator<TileMap>::GetInstance()->Get("SampleLevel"+ sampleID)->GetMap()->getSize();
				auto background = m_world->GenerateEntity(GameObjectType::BACKGROUND);
				auto active = ActiveCom();
				background.AddComponent(active);
				auto sprite = SpriteCom();
				auto m_sprite = sprite.Get();
				m_sprite->SetSpriteScale(vec2(size.x * 32.f / GSWY_GetPixel2WorldNumerator(), size.y * 32.f / GSWY_GetPixel2WorldNumerator()));
				m_sprite->SetSpriteTexture(ResourceAllocator<Texture2D>::GetInstance()->Get("SampleLevel"+ sampleID));
				m_sprite->SetSpritePosition(vec3((size.x/2-0.5)* 32.f / GSWY_GetPixel2WorldNumerator(), -(size.y / 2 - 0.5) * 32.f / GSWY_GetPixel2WorldNumerator(), -0.5));
				background.AddComponent(sprite);
			}

			// call load level here
			// object factory must be an abstract class in the engine and must be implemented in application
			GameObjectFactory* factory = GameObjectFactory::GetInstance();
			factory->LoadLevel("./asset/archetypes/levels/sample-level.json", m_world);

			//MOUSE SETUP
			{
				auto mouse = m_world->GenerateEntity(GameObjectType::MOUSE);
				auto active = ActiveCom();
				mouse.AddComponent(active);
				mouse.AddComponent(TransformCom());
				auto mousebody = BodyCom();
				mousebody.SetMass(0);
				mousebody.ChooseShape("AABB", 0.05, 0.05);
				mouse.AddComponent(mousebody);
				mouse.AddComponent(OwnershiptCom<GameObjectType>());
			}

			{
				auto miniMap = m_world->GenerateEntity(GameObjectType::MINIMAP);
				auto active = ActiveCom();
				miniMap.AddComponent(active);
				miniMap.AddComponent(TransformCom());
				auto sprite = SpriteCom();
				auto m_sprite = sprite.Get();
				m_sprite->SetSpriteTexture(m_miniMapTexture);
				miniMap.AddComponent(sprite);
			}

			GameLevelMapManager::GetInstance()->SetCurrentMapName("SampleLevel" + sampleID);
			GameLevelMapManager::GetInstance()->LoadCurrentTileMap(m_world);

			ComponentDecorator<TransformCom, GameObjectType> transform;
			m_world->Unpack(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0], transform);
			m_CameraController.SetPosition(vec3(transform->GetPos(),0));
			m_miniMapCameraController.SetPosition(vec3(transform->GetPos(), 0));
		}

		void BeforeRun()
		{
			// Play BGM
			//AudioManager::GetInstance()->PlaySound("breakout", AudioVector3{ 0, 0, 0 }, 1, 1);
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
				Renderer2D::BeginScene(m_CameraController.GetCamera());
				// m_world render
				m_world->PostRenderUpdate(ts);
				Renderer2D::EndScene();
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

				// Enable it if you want have crusing camera effect
				//auto len = glm::length(cursor - center);
				//if (len > 1e-2)
				//{
				//	delta = glm::normalize(cursor - center) * (float)ts * ((len > 30.0f) ? 30.0f : len);
				//}
				auto targetPos = vec3(transform->GetPos(),0.0f) + vec3(delta.x, -delta.y, 0.0f);
				auto newPos = m_CameraController.GetPosition() + (targetPos - m_CameraController.GetPosition()) * m_CameraController.GetCameraMoveSpeed() * (float)ts;
				m_CameraController.SetPosition(newPos);
				m_CameraController.SetZoomLevel(1);
			}
			// Set 3D sound
			//AudioManager::GetInstance()->Set3dListenerAndOrientation(m_CameraController.GetPosition());
		}

		void UpdateCursor(double ts)
		{
			{
				// Update cursor world position
				ComponentDecorator<BodyCom, GameObjectType> position;
				m_world->Unpack(m_world->GetAllEntityWithType(GameObjectType::MOUSE)[0], position);
				auto cameraPos = m_CameraController.GetPosition();
				auto mouseRelativePos = InputManager::GetInstance()->GetCursorViewPosition();
				auto zoomLevel = m_CameraController.GetZoomLevel();
				position->SetPos(vec2(cameraPos.x + zoomLevel * mouseRelativePos.x, cameraPos.y + zoomLevel * mouseRelativePos.y));
			}
		}

		void UpdateMiniMap(double ts)
		{
			{
				// Set minimap camera to be 2x the zoom level of main camera
				m_miniMapCameraController.SetPosition(m_CameraController.GetPosition());
				m_miniMapCameraController.SetZoomLevel(m_CameraController.GetZoomLevel() * 2);
				
				// Update minimap world position
				ComponentDecorator<TransformCom, GameObjectType> position;
				ComponentDecorator<SpriteCom, GameObjectType> sprite;
				auto miniMap = m_world->GetAllEntityWithType(GameObjectType::MINIMAP)[0];
				m_world->Unpack(miniMap, position);
				m_world->Unpack(miniMap, sprite);
				// Use main camera position and zoom to display the minimap on the main camera
				auto cameraPos = m_CameraController.GetPosition();
				auto RelativePos = InputManager::GetInstance()->GetCursorViewPosition(11./12 * GSWY_GetWindowWidth(),1.0/12 * GSWY_GetWindowHeight());
				auto zoomLevel = m_CameraController.GetZoomLevel();
				position->SetPos(vec2(cameraPos.x + zoomLevel * RelativePos.x, cameraPos.y + zoomLevel * RelativePos.y));
				// Use minimap camera for the scaling
				sprite->SetScale(vec2(0.33 * zoomLevel * m_miniMapCameraController.GetAspectRatio(), 0.33 * zoomLevel));
			}
		}

		void MiniMapRender(double ts)
		{
			auto fbo = RenderCommand::CreateAndBindFBO();
			m_miniMapTexture->AttachToFrameBuffer();

			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
			m_miniMapCameraController.OnUpdate(ts);
			Renderer2D::BeginBatch(m_miniMapCameraController.GetCamera());
			//Renderer2D::BeginScene(m_miniMapCameraController.GetCamera());
			// m_world render
			m_world->Render2(ts);
			Renderer2D::EndBatch();
			Renderer2D::DrawBatch();
			//Renderer2D::EndScene();
			RenderCommand::DestoryAndUnBindFBO(fbo);
		}

		void Render(double ts)
		{
			if(m_PP) m_PostProcessing.Bind();
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
			RenderCommand::Clear();
			m_CameraController.OnUpdate(ts);
			Renderer2D::BeginBatch(m_CameraController.GetCamera());
			//Renderer2D::BeginScene(m_CameraController.GetCamera());
			// m_world render
			m_world->Render(ts);
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
			if (m_PP)
			{
				m_PostProcessing.Unbind();
				m_PostProcessing.Render(ts);
			}
			Renderer2D::EndBatch();
			Renderer2D::DrawBatch();
			//Renderer2D::EndScene();
		}

		virtual void OnUpdate(double ts) override
		{
			BeforeFrame();
		
			double dt = (!m_world->IsPaused())? ts: 0;

			{
				if (InputManager::GetInstance()->IsKeyTriggered(KEY_F2))
				{
					m_PP = !m_PP;
				}
				if (IS_INGAME)
				{
					TIME("Pre Update");
					UpdateCamera(dt);
					UpdateCursor(dt);
					UpdateMiniMap(dt);
				}
				//std::future<void> update = std::async(std::launch::async, [this, ts]()
				//{

				//});
				//update.wait();
				if (IS_INGAME)
				{
					TIME("System Update");
					Update(dt);
				}
				{
					TIME("PreRender Update");
					PreRenderUpdate(dt);
				}
				if (IS_INGAME)
				{
					TIME("MiniMap Update");
					MiniMapRender(dt);
				}
				{
					TIME("Render Update");
					Render(dt);
				}
				{
					TIME("Manager Update");
					EventQueue<GameObjectType, EventType>::GetInstance()->Update(dt);
					GameLevelMapManager::GetInstance()->Update(dt);
				}
				if (IS_INGAME)
				{
					TIME("PostRender Update");
					PostRenderUpdate(dt);
				}
			}
			AfterFrame();
		}

		virtual void OnImGuiRender() override
		{


			WidgetManager::GetInstance()->RenderUI();
	#ifdef _DEBUG
			Instrumentor* instrumentor = Instrumentor::GetInstance();
			ImGui::SetNextWindowBgAlpha(0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::PushStyleColor(ImGuiCol_ResizeGrip, 0);
			ImGui::PushStyleColor(ImGuiCol_TitleBgActive, 0);
			ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, 0);
			ImGui::Begin("Instrumenting Profiling", false, ImGuiWindowFlags_NoDecoration);
			for (auto& result : instrumentor->GetResults()) {
				char entry[100];
				strcpy(entry, "%10.3f %s\t");
				strcat(entry, result.first);
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), entry, result.second.m_time, result.second.m_timeUnit);
			}
			ImGui::End();
			ImGui::PopStyleVar(1);
			ImGui::PopStyleColor(3);

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

	protected:

	private:
		OrthographicCameraController m_CameraController;
		OrthographicCameraController m_miniMapCameraController;
		std::shared_ptr<gswy::Texture2D> m_miniMapTexture;

		std::shared_ptr<GameWorld<GameObjectType>> m_world;
		gswy::OpenGLPostProcessing m_PostProcessing;
		bool m_PP = false;
	#ifdef _DEBUG

		//TODO Move to component
		gswy::ExplosionParticle m_ParticleSystem;
		gswy::Particle m_Particle;
		bool m_ParticleActive = true;
	#endif // _DEBUG


	public:
		void OnImGuiButtonClicke(const std::string& buttonName)
		{
			PRINT(buttonName);

			// Main menu
			if (buttonName.compare("New Game") == 0)
			{
				// TODO
				LoadGameWorld(1);
			}
			if (buttonName.compare("How To Play") == 0)
			{
				// TODO
			}
			if (buttonName.compare("Option") == 0)
			{
				// TODO
			}
			if (buttonName.compare("Exit") == 0)
			{
				// TODO
				Engine::SetQuit(true);
			}

			// In game pause menu
			if (buttonName.compare("Resume") == 0)
			{
				// TODO
				m_world->SetPause(!m_world->IsPaused());
				WidgetManager::GetInstance()->GetPauseMenu().SetVisible(m_world->IsPaused());
			}
			if (buttonName.compare("Main Menu") == 0)
			{
				// TODO
				LoadMainMenuWorld();
			}

		}
	};
}

class Application : public Engine {
public:
	Application()
	{
		GameLayer* gameLayer = new GameLayer();
		WidgetManager::ButtonInvokeFunction f = std::bind(&GameLayer::OnImGuiButtonClicke, gameLayer, std::placeholders::_1);
		WidgetManager::GetInstance()->SetButtonInvoker(f);
		PushLayer(gameLayer);
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