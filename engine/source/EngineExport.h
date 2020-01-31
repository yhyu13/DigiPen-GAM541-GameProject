/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: c++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: EngineExport.h
Purpose			: This header file must be imported by the game applications (clients of the engine library)
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#pragma once

// This file must be imported by game application
#include "engine/Engine.h"

///// entry point /////
#include "engine/Main.h"

///// rendering /////
#include "engine/renderer/Renderer2D.h"
#include "engine/renderer/Shader.h"
#include "engine/renderer/OrthographicCameraController.h"
