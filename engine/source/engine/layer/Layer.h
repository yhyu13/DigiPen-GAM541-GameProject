/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Layer.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/05/2020
- End Header ----------------------------*/

#pragma once
#include "engine/EngineCore.h"

namespace gswy {

	class ENGINE_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double ts) {}
		virtual void OnImGuiRender() {}

		inline const std::string GetDebugName() { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};
}
