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
		inline ImVec2 PosScaleBySize(const ImVec2& vec, const ImVec2& windowSize)
		{
			Engine& engine = Engine::Get();
			auto width = engine.GetWindow().GetWindowProperties().m_monitorWidth;
			auto height = engine.GetWindow().GetWindowProperties().m_monitorHeight;
			return ImVec2(vec.x / width * windowSize.x, vec.y / height * windowSize.y);
		}
		inline ImVec2 ScaleSize(const ImVec2& itemSize) {
			Engine& engine = Engine::Get();
			auto monitorWidth = engine.GetWindow().GetWindowProperties().m_monitorWidth;
			auto monitorHeight = engine.GetWindow().GetWindowProperties().m_monitorHeight;
			auto winWidth = engine.GetWindow().GetWindowProperties().m_width;
			auto winHeight = engine.GetWindow().GetWindowProperties().m_height;
			return ImVec2(itemSize.x * winWidth / monitorWidth, itemSize.y * winHeight / monitorHeight);
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