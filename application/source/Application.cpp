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
#include "ecs/componentSystem/Audio/SoundManager.h"

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

		/*
		Load all game resources
		*/
		void LoadResources()
		{
			TIME("Loading Resources");

			GameObjectFactory* factory = GameObjectFactory::GetInstance();
			factory->LoadResources("./asset/archetypes/resources.json");
			// TODO: remove Load resources and entities
			ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/SampleLevel0.png", "SampleLevel0");
			ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/SampleLevel1.png", "SampleLevel1");
			ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/SampleLevel2.png", "SampleLevel2");
			ResourceAllocator<Texture2D>::GetInstance()->Create("./asset/SampleLevel3.png", "SampleLevel3");
			// TODO : remove loading map test
			ResourceAllocator<TileMap>::GetInstance()->Create("./asset/SampleLevel0.json", "SampleLevel0");
			ResourceAllocator<TileMap>::GetInstance()->Create("./asset/SampleLevel1.json", "SampleLevel1");
			ResourceAllocator<TileMap>::GetInstance()->Create("./asset/SampleLevel2.json", "SampleLevel2");
			ResourceAllocator<TileMap>::GetInstance()->Create("./asset/SampleLevel3.json", "SampleLevel3");
			// TODO : consider to move the allocation of minimap texture elsewhere.
			m_miniMapTexture = Texture2D::Create(GSWY_GetWindowWidth(),GSWY_GetWindowHeight());

			InventoryManager::GetInstance()->LoadInventory("./asset/archetypes/levels/inventory-level-1.json");

			GameLevelMapManager::GetInstance()->ResetLevelData();
			GameLevelMapManager::GetInstance()->AddTileMap("SampleLevel0");
			GameLevelMapManager::GetInstance()->AddTileMap("SampleLevel1");
			GameLevelMapManager::GetInstance()->AddTileMap("SampleLevel2");
			GameLevelMapManager::GetInstance()->AddTileMap("SampleLevel3");
		}

		/*
		Load engine frameworks
		*/
		void InitFramework()
		{
			// Renderer
			Renderer2D::Init();
			OpenGLDebugDraw::Init();
			// Level manager
			GameLevelMapManager::GetInstance()->Init();
			// Texture loader
			ResourceAllocator<Texture2D>::GetInstance()->Init();
			// Animation loader
			ResourceAllocator<Animation>::GetInstance()->Init();
			// TileMap loader
			ResourceAllocator<TileMap>::GetInstance()->Init();
		}

		/*
		Load start up screen
		*/
		void InitGameWorld()
		{
			TIME("Initializing Game World");

			// Load all resources
			LoadResources();
			LoadGameWorldAndInit("./asset/archetypes/systems-startUp.json");
			
			Json::Value engineConfiguration;
			std::ifstream file("./asset/engine-configuration/engine-config.json", std::ifstream::binary);
			file >> engineConfiguration;
			file.close();

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			if (engineConfiguration["startup-screen"].asBool())
			{
				// Load logo
				auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("DigiPen-logo");
				auto m_DigipenLogo = MemoryManager::Make_shared<EntityDecorator<GameObjectType>>(m_world->GenerateEntity(GameObjectType::DIGIPEN_LOGO));
				auto active = ActiveCom();
				m_DigipenLogo->AddComponent(active);
				auto sprite = SpriteCom();
				auto m_sprite = sprite.Get();
				m_sprite->SetSpriteTexture(logoTexture);
				m_sprite->SetSpriteScale(vec2(2, 2.0 / logoTexture->GetWidth() * logoTexture->GetHeight()));
				m_sprite->SetSpritePosition(vec3(0));
				m_DigipenLogo->AddComponent(sprite);
				auto digipenLogoTransform = TransformCom(0, 0, Z_ORDER(1));
				m_DigipenLogo->AddComponent(digipenLogoTransform);

				auto teamLogoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("Team-Logo");
				auto m_teamLogo = MemoryManager::Make_shared<EntityDecorator<GameObjectType>>(m_world->GenerateEntity(GameObjectType::TEAM_LOGO));
				auto teamLogoActive = ActiveCom(false);
				m_teamLogo->AddComponent(teamLogoActive);
				auto teamLogoSpriteCom = SpriteCom();
				auto teamLogoSprite = teamLogoSpriteCom.Get();
				teamLogoSprite->SetSpriteTexture(teamLogoTexture);
				teamLogoSprite->SetSpriteScale(vec2(1, 1.0f / teamLogoTexture->GetWidth() * teamLogoTexture->GetHeight()));
				teamLogoSprite->SetSpritePosition(vec3(0));
				m_teamLogo->AddComponent(teamLogoSpriteCom);
				auto teamLogoTransform = TransformCom(0, 0, Z_ORDER(1000));
				m_teamLogo->AddComponent(teamLogoTransform);

				auto gameLogoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("Game-Logo");
				auto m_gameLogo = MemoryManager::Make_shared<EntityDecorator<GameObjectType>>(m_world->GenerateEntity(GameObjectType::GAME_LOGO));
				auto gameLogoActive = ActiveCom(false);
				m_gameLogo->AddComponent(gameLogoActive);
				auto gameLogoSpriteCom = SpriteCom();
				auto gameLogoSprite = gameLogoSpriteCom.Get();
				gameLogoSprite->SetSpriteTexture(gameLogoTexture);
				gameLogoSprite->SetSpriteScale(vec2(2, 2.0f / gameLogoTexture->GetWidth() * gameLogoTexture->GetHeight()));
				gameLogoSprite->SetSpritePosition(vec3(0));
				m_gameLogo->AddComponent(gameLogoSpriteCom);
				auto gameLogoTransform = TransformCom(0, 0, Z_ORDER(2000));
				m_gameLogo->AddComponent(gameLogoTransform);

				// Fading logo
				
				auto _e = MemoryManager::Make_shared<FadeEvent>(m_DigipenLogo->GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
				queue->Publish(_e, 3.0f);
				auto teamLogoEvent = MemoryManager::Make_shared<Event<GameObjectType, EventType>>(EventType::LOAD_TEAM_LOGO);
				queue->Publish(teamLogoEvent, 4.0f);
				auto gameLogoEvent = MemoryManager::Make_shared<Event<GameObjectType, EventType>>(EventType::LOAD_GAME_LOGO);
				queue->Publish(gameLogoEvent, 6.0f);
				auto _e1 = MemoryManager::Make_shared<LoadMainMenuEvent>();
				queue->Publish(_e1, 10.0f);

				{
					AudioManager::GetInstance()->StopAllChannels();
					AudioManager::GetInstance()->Update(0);
					AudioManager::GetInstance()->PlaySound("Menu_Track", AudioVector3{ 0,0,0 }, -10, 1);
				}
			}
			else
			{
				LoadMainMenuWorld();
			}
		}

		/*
		Helper function that load game world and its component systems
		*/
		void LoadGameWorldAndInit(const std::string& system_file)
		{
			// Event queue clearing and initialization must happen before game world initialization
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			queue->Clear();

			m_world = MemoryManager::Make_shared<GameWorld<GameObjectType>>();
			GameObjectFactory::GetInstance()->LoadSystem(system_file, m_world);
			// Initialize world and all system event subscribing
			m_world->Init();

			queue->Subscribe<GameLayer>(this, EventType::LOAD_MAIN_MENU, &GameLayer::OnLoadMainMenuWorld);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_GAME_WORLD, &GameLayer::OnLoadGameWorld);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_TEAM_LOGO, &GameLayer::OnLoadTeamLogo);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_GAME_LOGO, &GameLayer::OnLoadGameLogo);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_CREDIT_SCREEN, &GameLayer::OnLoadCreditScreen);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_HOW_TO_PLAY, &GameLayer::OnLoadHowToPlay);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_LEVEL_LOGO, &GameLayer::OnLoadLevelLogo);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_WAVE_CLEAR_LOGO, &GameLayer::OnLoadWaveClearLogo);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_LEVEL_CLEAR_LOGO, &GameLayer::OnLoadLevelClearLogo);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_WON_LOGO, &GameLayer::OnLoadWonLogo);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_LOST_LOGO, &GameLayer::OnLoadLostLogo);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_DIED_LOG, &GameLayer::OnLoadDiedLogo);
			queue->Subscribe<GameLayer>(this, EventType::LOAD_FINAL_WAVE, &GameLayer::OnLoadFinalWaveLogo);
			queue->Subscribe<GameLayer>(this, EventType::SOUND_AT_CAMERA, &GameLayer::OnPlaySoundAtCamera);
		}

		void OnLoadMainMenuWorld(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			LoadMainMenuWorld();
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto e1 = MemoryManager::Make_shared<OnSplashStateChangeEvent>((unsigned int)SplashScreenState::END);
			queue->Publish(e1);
		}
		void OnLoadGameWorld(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = dynamic_pointer_cast<LoadGameWorldEvent>(e))
			{
				LoadGameWorld(event->m_level, event->m_reload);
			}
		}
		void OnLoadTeamLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			ComponentDecorator<ActiveCom, GameObjectType> activeCom;
			auto m_teamLogo = m_world->GetAllEntityWithType(GameObjectType::TEAM_LOGO)[0];
			m_world->Unpack(m_teamLogo, activeCom);
			activeCom->SetActive(true);

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto e1 = MemoryManager::Make_shared<OnSplashStateChangeEvent>((unsigned int)SplashScreenState::TEAM_LOGO);
			queue->Publish(e1);
			auto e2 = MemoryManager::Make_shared<FadeEvent>(m_teamLogo, 1.f, -0.5f, 1.f, EventType::GC);
			queue->Publish(e2, 1.0f);
		}
		void OnLoadGameLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			ComponentDecorator<ActiveCom, GameObjectType> activeCom;
			auto m_gameLogo = m_world->GetAllEntityWithType(GameObjectType::GAME_LOGO)[0];
			m_world->Unpack(m_gameLogo, activeCom);
			activeCom->SetActive(true);

			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto e1 = MemoryManager::Make_shared<OnSplashStateChangeEvent>((unsigned int)SplashScreenState::GAME_LOGO);
			queue->Publish(e1);
			auto e2 = MemoryManager::Make_shared<FadeEvent>(m_gameLogo, 1.f, -0.5f, 1.f, EventType::GC);
			queue->Publish(e2, 2.0f);
		}
		void OnLoadCreditScreen(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = std::dynamic_pointer_cast<LoadCreditScreenEvent>(e))
			{
				auto creditsTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("credits");
				auto credit = m_world->GenerateEntity(GameObjectType::CREDITS);
				auto creditsActive = ActiveCom();
				credit.AddComponent(creditsActive);
				auto creditsSpriteCom = SpriteCom();
				auto creditsSprite = creditsSpriteCom.Get();
				creditsSprite->SetSpriteTexture(creditsTexture);
				creditsSprite->SetSpriteScale(vec2(3.6f, 3.6f / creditsTexture->GetWidth() * creditsTexture->GetHeight()));
				creditsSprite->SetSpritePosition(vec3(0));
				credit.AddComponent(creditsSpriteCom);
				auto cameraPos = event->m_cameraPosition;
				auto creditsTransform = TransformCom(cameraPos.x, cameraPos.y, Z_ORDER(9500));
				credit.AddComponent(creditsTransform);

				WidgetManager::GetInstance()->GetHUD().SetVisible(false);
			}
		}
		void OnLoadHowToPlay(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = std::dynamic_pointer_cast<LoadHowToPlayEvent>(e))
			{
				auto howToPlayTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("htp_1");
				auto m_HowToPlayLogo = (m_world->GenerateEntity(GameObjectType::HOW_TO_PLAY));
				m_HowToPlayLogo.AddComponent(ActiveCom());
				auto howToPlaySpriteCom = SpriteCom();
				auto howToPlaySprite = howToPlaySpriteCom.Get();
				howToPlaySprite->SetSpriteTexture(howToPlayTexture);
				howToPlaySprite->SetSpriteScale(vec2(3.6f, 3.6f / howToPlayTexture->GetWidth() * howToPlayTexture->GetHeight()));
				howToPlaySprite->SetSpritePosition(vec3(0));
				m_HowToPlayLogo.AddComponent(howToPlaySpriteCom);
				auto cameraPos = event->m_cameraPosition;
				auto howToPlayTransform = TransformCom(cameraPos.x, cameraPos.y, Z_ORDER(9500));
				m_HowToPlayLogo.AddComponent(howToPlayTransform);
			}
		}
		void OnLoadLevelLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = dynamic_pointer_cast<LoadLevelLogoEvent>(e))
			{
				// Level 0 does not have a level logo
				if (event->m_level == 0)
				{
					return;
				}
				auto lvl = Str(event->m_level);
				// Level Start logo
				auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("Level_" + lvl);
				auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
				auto active = ActiveCom();
				logo.AddComponent(active);
				auto sprite = SpriteCom();
				auto m_sprite = sprite.Get();
				m_sprite->SetSpriteTexture(logoTexture);
				m_sprite->SetSpriteScale(vec2(1, 1.0 / logoTexture->GetWidth() * logoTexture->GetHeight()));
				m_sprite->SetSpritePosition(vec3(0));
				logo.AddComponent(sprite);
				auto transform = TransformCom(0, 0, Z_ORDER(9000));
				logo.AddComponent(transform);
				auto body = BodyCom();
				body.SetPos(transform.GetPos());
				logo.AddComponent(body);
				auto attch = AttachedMovementCom();
				attch.followPos = true;
				attch.rPos = vec2(0, 0.25);
				logo.AddComponent(attch);
				auto owner = OwnershiptCom<GameObjectType>(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
				logo.AddComponent(owner);
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
				queue->Publish(_e, 2.0);
			}
		}
		void OnLoadWaveClearLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			// Wave CLear logo
			auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("Wave_Clear");
			auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
			auto active = ActiveCom();
			logo.AddComponent(active);
			auto sprite = SpriteCom();
			auto m_sprite = sprite.Get();
			m_sprite->SetSpriteTexture(logoTexture);
			m_sprite->SetSpriteScale(vec2(1, 1.0 / logoTexture->GetWidth() * logoTexture->GetHeight()));
			m_sprite->SetSpritePosition(vec3(0));
			logo.AddComponent(sprite);
			auto transform = TransformCom(0, 0, Z_ORDER(9000));
			logo.AddComponent(transform);
			auto body = BodyCom();
			logo.AddComponent(body);
			auto attch = AttachedMovementCom();
			attch.followPos = true;
			attch.rPos = vec2(0, 0.25);
			logo.AddComponent(attch);
			auto owner = OwnershiptCom<GameObjectType>(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
			logo.AddComponent(owner);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
			queue->Publish(_e, 2.0f);
		}
		void OnLoadLevelClearLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = std::dynamic_pointer_cast<LoadLevelClearEvent>(e))
			{
				// Level CLear logo
				auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("Level_Clear");
				auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
				auto active = ActiveCom();
				logo.AddComponent(active);
				auto sprite = SpriteCom();
				auto m_sprite = sprite.Get();
				m_sprite->SetSpriteTexture(logoTexture);
				m_sprite->SetSpriteScale(vec2(1, 1.0 / logoTexture->GetWidth() * logoTexture->GetHeight()));
				m_sprite->SetSpritePosition(vec3(0));
				logo.AddComponent(sprite);
				auto transform = TransformCom(0, 0, Z_ORDER(9000));
				logo.AddComponent(transform);
				auto body = BodyCom();
				body.SetPos(transform.GetPos());
				logo.AddComponent(body);
				auto attch = AttachedMovementCom();
				attch.followPos = true;
				attch.rPos = vec2(0, 0.25);
				logo.AddComponent(attch);
				auto owner = OwnershiptCom<GameObjectType>(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
				logo.AddComponent(owner);
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
				queue->Publish(_e, 2.0f);
				auto e2 = MemoryManager::Make_shared<CanPlayerInputEvent>(false);
				queue->Publish(e2);
				auto e3 = MemoryManager::Make_shared<LoadGameWorldEvent>(event->m_level, false);
				queue->Publish(e3, 3.1f);
			}
		}
		void OnLoadWonLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("You_Won");
			auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
			auto active = ActiveCom();
			logo.AddComponent(active);
			auto sprite = SpriteCom();
			auto m_sprite = sprite.Get();
			m_sprite->SetSpriteTexture(logoTexture);
			m_sprite->SetSpriteScale(vec2(1, 1.0 / logoTexture->GetWidth() * logoTexture->GetHeight()));
			m_sprite->SetSpritePosition(vec3(0));
			logo.AddComponent(sprite);
			auto transform = TransformCom(0, 0, Z_ORDER(9000));
			logo.AddComponent(transform);
			auto body = BodyCom();
			body.SetPos(transform.GetPos());
			logo.AddComponent(body);
			auto attch = AttachedMovementCom();
			attch.followPos = true;
			attch.rPos = vec2(0, 0.25);
			logo.AddComponent(attch);
			auto owner = OwnershiptCom<GameObjectType>(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
			logo.AddComponent(owner);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
			queue->Publish(_e, 2.0f);
			auto e3 = MemoryManager::Make_shared<PlaySoundAtPlayerLocationEvent>("Victory", 1, 1);
			queue->Publish(e3);
			auto e4 = MemoryManager::Make_shared<LoadMainMenuEvent>();
			queue->Publish(e4, 3.1f);
			auto e2 = MemoryManager::Make_shared<CanPlayerInputEvent>(false);
			queue->Publish(e2);
			auto audio = AudioManager::GetInstance();
			audio->StopSound("Track_1");
		}
		void OnLoadLostLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("You_Lost");
			auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
			auto active = ActiveCom();
			logo.AddComponent(active);
			auto sprite = SpriteCom();
			auto m_sprite = sprite.Get();
			m_sprite->SetSpriteTexture(logoTexture);
			m_sprite->SetSpriteScale(vec2(1, 1.0 / logoTexture->GetWidth() * logoTexture->GetHeight()));
			m_sprite->SetSpritePosition(vec3(0));
			logo.AddComponent(sprite);
			auto transform = TransformCom(0, 0, Z_ORDER(9000));
			logo.AddComponent(transform);
			auto body = BodyCom();
			body.SetPos(transform.GetPos());
			logo.AddComponent(body);
			auto attch = AttachedMovementCom();
			attch.followPos = true;
			attch.rPos = vec2(0, 0.25);
			logo.AddComponent(attch);
			auto owner = OwnershiptCom<GameObjectType>(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
			logo.AddComponent(owner);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
			queue->Publish(_e, 2.0f);
			auto e3 = MemoryManager::Make_shared<PlaySoundAtPlayerLocationEvent>("Defeat", 1, 1);
			queue->Publish(e3);
			auto e4 = MemoryManager::Make_shared<LoadMainMenuEvent>();
			queue->Publish(e4, 3.1f);
			auto e2 = MemoryManager::Make_shared<CanPlayerInputEvent>(false);
			queue->Publish(e2);
			auto audio = AudioManager::GetInstance();
			audio->StopSound("Track_1");
		}
		void OnLoadDiedLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("You_Died");
			auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
			auto active = ActiveCom();
			logo.AddComponent(active);
			auto sprite = SpriteCom();
			auto m_sprite = sprite.Get();
			m_sprite->SetSpriteTexture(logoTexture);
			m_sprite->SetSpriteScale(vec2(1, 1.0 / logoTexture->GetWidth() * logoTexture->GetHeight()));
			m_sprite->SetSpritePosition(vec3(0));
			logo.AddComponent(sprite);
			auto transform = TransformCom(0, 0, Z_ORDER(9000));
			logo.AddComponent(transform);
			auto body = BodyCom();
			body.SetPos(transform.GetPos());
			logo.AddComponent(body);
			auto attch = AttachedMovementCom();
			attch.followPos = true;
			attch.rPos = vec2(0, 0.25);
			logo.AddComponent(attch);
			auto owner = OwnershiptCom<GameObjectType>(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
			logo.AddComponent(owner);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
			queue->Publish(_e, 2.0f);
			auto e3 = MemoryManager::Make_shared<PlaySoundAtPlayerLocationEvent>("Defeat", 1, 1);
			queue->Publish(e3);
			auto e4 = MemoryManager::Make_shared<LoadMainMenuEvent>();
			queue->Publish(e4, 3.1f);
			auto e2 = MemoryManager::Make_shared<CanPlayerInputEvent>(false);
			queue->Publish(e2);
			auto audio = AudioManager::GetInstance();
			audio->StopSound("Track_1");
		}
		void OnLoadFinalWaveLogo(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("Final_Wave");
			auto logo = m_world->GenerateEntity(GameObjectType::BACKGROUND);
			auto active = ActiveCom();
			logo.AddComponent(active);
			auto sprite = SpriteCom();
			auto m_sprite = sprite.Get();
			m_sprite->SetSpriteTexture(logoTexture);
			m_sprite->SetSpriteScale(vec2(1, 1.0 / logoTexture->GetWidth() * logoTexture->GetHeight()));
			m_sprite->SetSpritePosition(vec3(0));
			logo.AddComponent(sprite);
			auto transform = TransformCom(0, 0, Z_ORDER(9000));
			logo.AddComponent(transform);
			auto body = BodyCom();
			body.SetPos(transform.GetPos());
			logo.AddComponent(body);
			auto attch = AttachedMovementCom();
			attch.followPos = true;
			attch.rPos = vec2(0, 0.25);
			logo.AddComponent(attch);
			auto owner = OwnershiptCom<GameObjectType>(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
			logo.AddComponent(owner);
			auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
			auto _e = MemoryManager::Make_shared<FadeEvent>(logo.GetEntity(), 1.f, -0.5f, 1.f, EventType::GC);
			queue->Publish(_e, 2.0f);
		}
		void OnPlaySoundAtCamera(EventQueue<GameObjectType, EventType>::EventPtr e)
		{
			if (auto event = dynamic_pointer_cast<PlaySoundAtCameraLocationEvent>(e))
			{
				auto playerPos = m_CameraController.GetPosition();
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto e1 = MemoryManager::Make_shared<SoundEvent>(event->soundName, playerPos, event->m_vol, event->m_freq);
				queue->Publish(e1);
			}
		}

		void LoadMainMenuWorld()
		{
			// Re-load game world
			{
				LoadGameWorldAndInit("./asset/archetypes/systems-mainMenu.json");
			}
			// Clear level data
			{
				GameLevelMapManager::GetInstance()->UnloadCurrentTileMap(m_world);
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
				m_sprite->SetSpritePosition(vec3(0,0,-0.5));
				background.AddComponent(sprite);

				auto gameLogoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("Game-Logo");
				auto m_gameLogo = (m_world->GenerateEntity(GameObjectType::GAME_LOGO));
				auto gameLogoActive = ActiveCom();
				m_gameLogo.AddComponent(gameLogoActive);
				auto gameLogoSpriteCom = SpriteCom();
				auto gameLogoSprite = gameLogoSpriteCom.Get();
				gameLogoSprite->SetSpriteTexture(gameLogoTexture);
				gameLogoSprite->SetSpriteScale(vec2(2.f, 2.f / gameLogoTexture->GetWidth() * gameLogoTexture->GetHeight()));
				gameLogoSprite->SetSpritePosition(vec3(0, .5, Z_ORDER(9000)));
				m_gameLogo.AddComponent(gameLogoSpriteCom);
			}
			// Reset camera
			{
				m_CameraController.SetPosition(vec3(0));
				m_miniMapCameraController.SetPosition(vec3(0));
			}

			// Stop BGM
			{
				AudioManager::GetInstance()->StopAllChannels();
				AudioManager::GetInstance()->Update(0);
				AudioManager::GetInstance()->PlaySound("Menu_Track", AudioVector3{ 0,0,0 }, -10, 1);
			}
		}

		void LoadGameWorld(int level, bool reloadGameWorld = false)
		{
			auto sampleID = Str(level);
			DEBUG_PRINT("Loading map ID " + sampleID);
			// Re-load game world
			{
				if (reloadGameWorld)
				{
					LoadGameWorldAndInit("./asset/archetypes/systems-game.json");
					InventoryManager::GetInstance()->ReLoadInventory("./asset/archetypes/levels/inventory-level-1.json");
					SkillManager::GetInstance()->ResetSkills();
				}
				else
				{
					m_world->RemoveAllEntities();
				}
			}
			// Set widget
			{
				WidgetManager::GetInstance()->GetHUD().SetVisible(true);
				WidgetManager::GetInstance()->GetInventoryMenu().SetVisible(true);
				WidgetManager::GetInstance()->GetPauseMenu().SetVisible(false);
				WidgetManager::GetInstance()->GetShopMenu().SetVisible(false);
				WidgetManager::GetInstance()->GetMainMenu().SetVisible(false);
			}
			// Set background
			{
				auto size = ResourceAllocator<TileMap>::GetInstance()->Get("SampleLevel"+ sampleID)->GetMap()->getSize();
				auto background = m_world->GenerateEntity(GameObjectType::BACKGROUND);
				auto active = ActiveCom();
				background.AddComponent(active);
				auto sprite = SpriteCom();
				auto m_sprite = sprite.Get();
				// 32.f is the tilewidth and tileheight of each tile in the tilemap
				m_sprite->SetSpriteScale(vec2(size.x * 32.f / GSWY_GetPixel2WorldNumerator(), size.y * 32.f / GSWY_GetPixel2WorldNumerator()));
				m_sprite->SetSpriteTexture(ResourceAllocator<Texture2D>::GetInstance()->Get("SampleLevel"+ sampleID));
				m_sprite->SetSpritePosition(vec3((size.x/2-0.5)* 32.f / GSWY_GetPixel2WorldNumerator(), -(size.y / 2 - 0.5) * 32.f / GSWY_GetPixel2WorldNumerator(), -0.5));
				background.AddComponent(sprite);
			}
			// Call load level here
			// object factory must be an abstract class in the engine and must be implemented in application
			{
				GameObjectFactory::GetInstance()->LoadLevel("./asset/archetypes/levels/sample-level.json", m_world);
			}
			// Load level logo
			{
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto e = MemoryManager::Make_shared<LoadLevelLogoEvent>(level);
				queue->Publish(e);
			}
			// SPACE bar
			{
				auto logoTexture = ResourceAllocator<Texture2D>::GetInstance()->Get("Press_Space");
				auto logo = m_world->GenerateEntity(GameObjectType::START_WAVE_INDICATOR);
				auto active = ActiveCom();
				logo.AddComponent(active);
				auto sprite = SpriteCom();
				auto m_sprite = sprite.Get();
				m_sprite->SetSpriteTexture(logoTexture);
				m_sprite->SetSpriteScale(vec2(1.5, 1.5 / logoTexture->GetWidth() * logoTexture->GetHeight()));
				m_sprite->SetSpritePosition(vec3(0));
				logo.AddComponent(sprite);
				auto transform = TransformCom(0, 0, Z_ORDER(9000));
				logo.AddComponent(transform);
				auto body = BodyCom();
				body.SetPos(transform.GetPos());
				logo.AddComponent(body);
				auto attch = AttachedMovementCom();
				attch.followPos = true;
				attch.rPos = vec2(0, -0.5);
				logo.AddComponent(attch);
				auto owner = OwnershiptCom<GameObjectType>(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0]);
				logo.AddComponent(owner);
			}
			// Mouse
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
			// Minimap
			{
				auto miniMap = m_world->GenerateEntity(GameObjectType::MINIMAP);
				auto active = ActiveCom();
				miniMap.AddComponent(active);
				miniMap.AddComponent(TransformCom(0,0,Z_ORDER(9000)));
				auto sprite = SpriteCom();
				auto m_sprite = sprite.Get();
				m_sprite->SetSpriteTexture(m_miniMapTexture);
				miniMap.AddComponent(sprite);
			}
			// Load content of this level
			{
				
				GameLevelMapManager::GetInstance()->SetCurrentMapName("SampleLevel" + sampleID);
				GameLevelMapManager::GetInstance()->LoadCurrentTileMap(m_world);
				GameLevelMapManager::GetInstance()->SetIsLoading(false);
			}
			// Reset camera
			{
				
				ComponentDecorator<TransformCom, GameObjectType> transform;
				m_world->Unpack(m_world->GetAllEntityWithType(GameObjectType::PLAYER)[0], transform);
				m_CameraController.SetPosition(vec3(transform->GetPos(), 0));
				m_miniMapCameraController.SetPosition(vec3(transform->GetPos(), 0));
			}

			// Play BGM
			{
				AudioManager::GetInstance()->StopAllChannels();
				AudioManager::GetInstance()->Update(0);
				AudioManager::GetInstance()->PlaySound("Track_1", AudioVector3{ 0,0,0 }, -10, 1);
			}

			// Re-enable player input
			{
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto e2 = MemoryManager::Make_shared<CanPlayerInputEvent>(true);
				queue->Publish(e2);
			}
		}

		void BeforeRun()
		{
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
				auto targetPos = vec3(transform->GetPos(),0.0f) + vec3(delta.x, -delta.y, 0.0f);
				auto newPos = m_CameraController.GetPosition() + (targetPos - m_CameraController.GetPosition()) * m_CameraController.GetCameraMoveSpeed() * (float)ts;
				m_CameraController.SetPosition(newPos);
				m_CameraController.SetZoomLevel(1);
			}
		}

		void UpdateAudio(double ts)
		{
			// Set 3D sound
			auto audio = AudioManager::GetInstance();
			auto position = AudioVector3(m_CameraController.GetPosition());
			audio->Set3dListenerAndOrientation(position);
			audio->SetChannel3dPosition(audio->GetSoundChannel("Track_1"), position);
			audio->SetChannel3dPosition(audio->GetSoundChannel("Menu_Track"), position);
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
			if (!m_world->IsPaused())
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
				auto RelativePos = InputManager::GetInstance()->GetCursorViewPosition(11. / 12 * GSWY_GetWindowWidth(), 1.0 / 12 * GSWY_GetWindowHeight());
				auto zoomLevel = m_CameraController.GetZoomLevel();
				position->SetPos(vec2(cameraPos.x + zoomLevel * RelativePos.x, cameraPos.y + zoomLevel * RelativePos.y));
				// Use minimap camera for the scaling
				sprite->SetScale(vec2(0.33 * zoomLevel * m_miniMapCameraController.GetAspectRatio(), 0.33 * zoomLevel));
			}
		}

		void MiniMapRender(double ts)
		{
			if (!m_world->IsPaused())
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
		}

		void Render(double ts)
		{
			RenderCommand::SetClearColor({ 0.0f, 0.0f, 0.0f, 1 });
			RenderCommand::Clear();
			m_CameraController.OnUpdate(ts);
			Renderer2D::BeginBatch(m_CameraController.GetCamera());
			// m_world render
			m_world->Render(ts);
			Renderer2D::EndBatch();
			Renderer2D::DrawBatch();
		}

		virtual void OnUpdate(double ts) override
		{
			BeforeFrame();
			double dt = (!m_world->IsPaused())? ts: 0;
			{
				{
					TIME("Pre Update (Game)");
					if (IS_INGAME)
					{
						UpdateCamera(dt);
						UpdateCursor(dt);
						UpdateMiniMap(dt);
					}
					UpdateAudio(dt);
				}
				{
					TIME("System Update");
					Update(dt);
				}
				{
					TIME("PreRender Update");
					PreRenderUpdate(dt);
				}
				{
					TIME("Render Update");
					if (IS_INGAME)
					{
						MiniMapRender(dt);
					}
					Render(dt);
				}
				{
					TIME("Manager Update");
					EventQueue<GameObjectType, EventType>::GetInstance()->Update(dt);
					if (IS_INGAME)
					{
						GameLevelMapManager::GetInstance()->Update(dt);
					}
				}
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
			ImGui::Begin("Instrumenting Profiling", false, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize);
			for (auto& result : instrumentor->GetResults()) {
				char entry[100];
				strcpy(entry, "%10.3f %s\t");
				strcat(entry, result.first);
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), entry, result.second.m_time, result.second.m_timeUnit);
			}
			ImGui::End();
			ImGui::PopStyleVar(1);
			ImGui::PopStyleColor(3);
	#endif // _DEBUG

		}

	protected:

	private:
		OrthographicCameraController m_CameraController;
		OrthographicCameraController m_miniMapCameraController;
		std::shared_ptr<gswy::Texture2D> m_miniMapTexture;
		std::shared_ptr<GameWorld<GameObjectType>> m_world;

	/*
	ImGui call back
	*/
	public:
		void OnImGuiButtonClicke(const std::string& buttonName)
		{
			DEBUG_PRINT(buttonName);

			// Main menu
			if (buttonName.compare("New Game") == 0)
			{
				// TODO
				LoadGameWorld(GameLevelMapManager::GetInstance()->m_currentLevel, true);
			}
			if (buttonName.compare("How To Play") == 0)
			{
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto cameraPos = m_CameraController.GetPosition();
				auto e = MemoryManager::Make_shared<LoadHowToPlayEvent>(glm::vec2(cameraPos.x, cameraPos.y));
				queue->Publish(e);

				// Set widget
				{
					WidgetManager* manager = WidgetManager::GetInstance();
					manager->GetInventoryMenu().SetVisible(false);
					manager->GetPauseMenu().SetVisible(false);
					manager->GetShopMenu().SetVisible(false);
					manager->GetMainMenu().SetVisible(false);
				}

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
			if (buttonName.compare("Resume Game") == 0)
			{
				// TODO
				m_world->SetPause(!m_world->IsPaused());
				WidgetManager::GetInstance()->GetPauseMenu().SetVisible(m_world->IsPaused());
				AudioManager::GetInstance()->PauseAllChannels(m_world->IsPaused());
				WidgetManager::GetInstance()->GetHUD().SetVisible(true);
			}
			if (buttonName.compare("Main Menu") == 0)
			{
				// TODO
				LoadMainMenuWorld();
			}

			if (buttonName.compare("Credits") == 0)
			{
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto cameraPos = m_CameraController.GetPosition();
				auto e = MemoryManager::Make_shared<LoadCreditScreenEvent>(glm::vec2(cameraPos.x, cameraPos.y));
				queue->Publish(e);

				// Set widget
				{
					WidgetManager* manager = WidgetManager::GetInstance();
					manager->GetInventoryMenu().SetVisible(false);
					manager->GetPauseMenu().SetVisible(false);
					manager->GetShopMenu().SetVisible(false);
					manager->GetMainMenu().SetVisible(false);
				}
			}

			if (buttonName.compare("Full Screen") == 0)
			{
				//m_CameraController.SetAspectRatio(GSWY_GetWindowWidth() / GSWY_GetWindowHeight());
			}
			
			if (buttonName.compare("Change Resolution") == 0)
			{
				m_CameraController.SetAspectRatio(GSWY_GetWindowWidth() / GSWY_GetWindowHeight());
			}
		}

		void OnInterruption(const int& isFocussed)
		{
			if (IS_INGAME)
			{
				if (!isFocussed)
				{
					m_world->SetPause(true);
					// TODO : pause music
					AudioManager::GetInstance()->PauseAllChannels(true);
				}
				WidgetManager::GetInstance()->GetPauseMenu().SetVisible(m_world->IsPaused());
			}
		}
	};
}

class Application : public Engine {
public:
	Application()
	{
		auto gameLayer = new GameLayer();
		WidgetManager::ButtonInvokeFunction f = std::bind(&GameLayer::OnImGuiButtonClicke, gameLayer, std::placeholders::_1);
		WidgetManager::GetInstance()->SetButtonInvoker(f);

		InterruptHandler handler = std::bind(&GameLayer::OnInterruption, gameLayer, std::placeholders::_1);
		GetWindow().SetInterruptHandler(handler);

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