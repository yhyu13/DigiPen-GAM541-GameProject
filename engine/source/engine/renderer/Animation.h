/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 02/04/2020
- End Header ----------------------------*/

#pragma once
#include "engine/EngineCore.h"
#include <vector>


namespace gswy {


	struct ENGINE_API FrameData
	{
		std::string textureName;
		int x;
		int y;
		int width;
		int height;
		double displayTimeSeconds;
	};

	class ENGINE_API Animation
	{
	public:

		static std::shared_ptr<Animation> Create(const std::string& path);

		Animation();

		void AddFrame(std::string textureName, int x, int y, int width, int height, double frameTime);
		const FrameData* GetCurrentFrame() const;
		bool UpdateFrame(double deltaTime);
		void Reset();

		void SetAnimLooping(bool b) { m_IsLooping = b; };
		void SetAnimIdle(bool b) { m_IsIdle = b; };
		void SetAnimPaused(bool b) { m_IsPaused = b; };

	private:
		void IncrementFrame();

		std::vector<FrameData> frames;
		int currentFrameIndex;
		double currentFrameTime;

		bool m_IsIdle;
		bool m_IsPaused;
		bool m_IsLooping;
	};
}
