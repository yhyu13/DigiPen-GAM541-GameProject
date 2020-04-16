/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/15/2020
- End Header ----------------------------*/
#pragma once

///// Client application lib /////
#include <random>
#include <future>
#include <glm/gtc/type_ptr.hpp>
#include "imgui/imgui.h"

///// Client application gameplay /////
#include "ecs/componentSystem/Audio/SoundComSys.h"
#include "ecs/componentSystem/Controller/StartUpControllerComSysy.h"
#include "ecs/componentSystem/Controller/MainMenuControllerComSys.h"
#include "ecs/componentSystem/Controller/PlayerControllerComSys.h"
#include "ecs/componentSystem/Controller/PlayerAnimationControllerComSys.h"
#include "ecs/componentSystem/Controller/Mob1ControllerComSys.h"
#include "ecs/componentSystem/Controller/Mob2ControllerComSys.h"
#include "ecs/componentSystem/Controller/Mob1BossControllerComSys.h"
#include "ecs/componentSystem/Controller/Mob2BossControllerComSys.h"
#include "ecs/componentSystem/Controller/MobPortalControllerComSys.h"
#include "ecs/componentSystem/Controller/TowerControllerComSys.h"
#include "ecs/componentSystem/Controller/BaseControllerComSys.h"
#include "ecs/componentSystem/Graphics/SceneComSys.h"
#include "ecs/componentSystem/Graphics/SpriteComSys.h"
#include "ecs/componentSystem/Graphics/AnimationComSys.h"
#include "ecs/componentSystem/Graphics/MiniMapSceneComSys.h"
#include "ecs/componentSystem/Graphics/ParticleComSys.h"
#include "ecs/componentSystem/Physics/PhysicsComSys.h"
#include "ecs/componentSystem/Physics/AttachedMovementComSys.h"
#include "ecs/componentSystem/Gameplay/BuffComSys.h"
#include "ecs/componentSystem/Gameplay/WeaponComSys.h"
#include "ecs/componentSystem/Gameplay/LifeTimeComSys.h"
#include "ecs/componentSystem/Gameplay/SpawningComSys.h"
#include "ecs/componentSystem/Gameplay/HitPointComSys.h"
#include "ecs/componentSystem/Gameplay/DeathComSys.h"
#include "ecs/componentSystem/Gameplay/FadeComSys.h"
#include "ecs/componentSystem/GC/GCComSys.h"
#include "ecs/componentSystem/Gameplay/PlayerSkillSystem.h"
#include "ecs/componentSystem/Gameplay/TargetEntityComponentSystem.h"
#include "ecs/componentSystem/Controller/CoinControllerComSys.h"

///// Client application framework /////
#include "object-factory/GameObjectFactory.h"
#include "level/GameLevelMapManager.h"
#include "ui/GameWidgetManager.h"
#include "inventory-manager/InventoryManager.h"
#include "skill-manager/SkillManager.h"
