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

#include "Animation.h"

using namespace gswy;

std::shared_ptr<Animation> gswy::Animation::Create(const std::string& path)
{
	return std::make_shared<Animation>();
}

Animation::Animation() 
	: 
	frames(0), 
	currentFrameIndex(0), 
	currentFrameTime(0.0),
	m_IsIdle(true),
	m_IsPaused(false),
	m_IsLooping(true)
{
}

void Animation::AddFrame(std::string textureName, int x, int y, int width, int height, double frameTime)
{
	FrameData data;
	data.textureName = textureName;
	data.x = x;
	data.y = y;
	data.width = width;
	data.height = height;
	data.displayTimeSeconds = frameTime;

	frames.push_back(data);
}

const FrameData* Animation::GetCurrentFrame() const
{
	if (frames.size() > 0)
	{
		if (m_IsIdle)
		{
			return &frames[0];
		}
		else
		{
			return &frames[currentFrameIndex];
		}
	}
	return nullptr;
}

bool Animation::UpdateFrame(double deltaTime)
{
	if (m_IsPaused || m_IsIdle)
	{
		return true;
	}

	if (frames.size() > 0)
	{
		currentFrameTime += deltaTime;

		if (currentFrameTime >= frames[currentFrameIndex].displayTimeSeconds)
		{
			currentFrameTime = 0.f;
			IncrementFrame();
			return true;
		}
	}
	return false;
}

void Animation::IncrementFrame()
{
	// check if we reached the last frame
	if (currentFrameIndex == (frames.size() - 1))
	{
		if (m_IsLooping)
		{
			currentFrameIndex = 0;
		}
	}
	else
	{
		currentFrameIndex++;
	}
}

void Animation::Reset()
{
	currentFrameIndex = 0;
}
