/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: ImGuiLayer.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/15/2020
- End Header ----------------------------*/

#pragma once
#include "../layer/Layer.h"

namespace gswy {
	
	class ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
	
		void Begin();
		void End();

	private:
		float m_Time = 0.0f;
	};
	
}

