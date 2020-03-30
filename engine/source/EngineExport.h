/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: c++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: EngineExport.h
Purpose			: This header file must be imported by the game applications (clients of the engine library)
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519), Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#pragma once
/*
	 This file must be imported by game application
*/


///// Must be inherited by the client application /////
#include "engine/Engine.h"
#include "engine/window/Window.h"
#include "engine/audio/AudioManager.h"
#include "engine/framerate-controller/FramerateController.h"
#include "engine/input/InputManager.h"

///// Entry point /////
//#include "engine/Main.h"

///// Memory point /////
#include "engine/allocator/MemoryManager.h"

///// InputManager system /////
#include "engine/input/InputManager.h"

///// rendering /////
#include "engine/renderer/Animation.h"
#include "engine/renderer/Renderer2D.h"
#include "engine/renderer/Shader.h"
#include "engine/renderer/Sprite.h"
#include "engine/renderer/ParticleSystem.h"
#include "engine/renderer/OrthographicCameraController.h"
#include "engine/platform/OpenGL/OpenGLDebugDraw.h"
#include "engine/platform/OpenGL/OpenGLPostProcessing.h"

///// UI /////
#include "engine/ui/Widget.h"

///// Resource Allocator /////
#include "engine/allocator/ResouceAllocator.h"

///// TileMapManager /////
#include "engine/tilemap/TileMapManager.h"

///// Entity Component System /////
#include "engine/ecs/BaseComponentSystem.h"
#include "engine/ecs/BaseComponent.h"
#include "engine/ecs/ComponentDecorator.h"
#include "engine/ecs/GameWorld.h"
#include "engine/ecs/EntityManager.h"
#include "engine/ecs/EntityDecorator.h"

///// Event System /////
#include "engine/events/Event.h"
#include "engine/events/EventHandler.h"
#include "engine/events/EventQueue.h"

///// Physics /////
#include "engine/physics/Collisions.h"

///// Math /////
#include "engine/math/MathHelper.h"

///// Instrumenting Profiling ////
#include "engine/profiling/InstrumentorCore.h"

//// Object Factory ////
#include "engine/object-factory/ObjectFactory.h"

//// Utility ////
#include "engine/utility/Timer.h"