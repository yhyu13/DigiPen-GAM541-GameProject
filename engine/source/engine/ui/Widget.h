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
 
namespace gswy {

	class Widget
	{
	public:
		virtual ~Widget() {}
		virtual void Init() {}
		virtual void Render() = 0;
		inline void SetVisible(bool vis) { IsVisible = vis; }
		inline bool GetVisible() const { return IsVisible; }
	protected:
		bool IsVisible = true;
		unsigned int m_WindowSize_X = 1920;
		unsigned int m_WindowSize_Y = 1080;
	};
}