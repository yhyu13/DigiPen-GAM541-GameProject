/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Widget.h
Author: Kyle Wang (kyle.wang@digipen.edu | 60000719)
Creation date: 02/26/2020
- End Header ----------------------------*/
#pragma once
#include "imgui.h"
#include "engine/Engine.h"
#include "engine/window/Window.h"
 
namespace gswy {

	class Widget
	{
	public:
		Widget() {}
		virtual ~Widget() {}
		virtual void Init() {}
		virtual void Update() {};
		virtual void Render() = 0;
		inline void SetVisible(bool vis) { IsVisible = vis; }
		inline bool GetVisible() const { return IsVisible; }
		inline ImVec4 GetStyle() const { return m_styleColor; }
		inline void SetStyle(int r, int g, int b, int a) { m_styleColor = (ImVec4)ImColor::ImColor(r, g, b, a); }
		inline ImVec2 ScaleBy1080p(const ImVec2& vec, const ImVec2& windowSize)
		{
			return ImVec2(vec.x / 1920.0 * windowSize.x, vec.y / 1080.0 * windowSize.y);
		}


		inline unsigned int GetWindowSize_X() 
		{
			Engine& engine = Engine::Get();
			return engine.GetWindow().GetWidth();
		}
		inline unsigned int GetWindowSize_Y() 
		{
			Engine& engine = Engine::Get();
			return engine.GetWindow().GetHeight();
		}
	protected:
		bool IsVisible = true;
		ImVec4 m_styleColor;
	};
}