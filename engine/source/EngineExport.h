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
#include <iostream>
#include <glfw3.h>
#include <sstream>
#include "engine/Engine.h"
#include "engine/window/Window.h"
#include "engine/audio/AudioManager.h"
#include "engine/framerate-controller/FramerateController.h"
#include "engine/input/Input.h"

///// Entry point /////
#include "engine/Main.h"


///// Input system /////
#include "engine/input/Input.h"
