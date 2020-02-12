/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: RendererCommand.cpp
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "RenderCommand.h"
#include "../platform/OpenGL/OpenGLRendererAPI.h"

namespace gswy {
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}