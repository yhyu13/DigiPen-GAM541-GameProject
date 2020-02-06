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

#include "engine/ecs/BaseComponent.h"
#include "engine/allocator/ResouceAllocator.h"
#include "engine/renderer/Animation.h"

namespace gswy
{
	struct AnimationCom : BaseComponent<AnimationCom> {
		AnimationCom() = default;

		AnimationCom& operator=(const AnimationCom& rhs)
		{
			if (this != &rhs)
			{
				m_animation = rhs.m_animation;
				m_animationState = rhs.m_animationState;
				m_animationStateMap.clear();
				for (auto it = rhs.m_animationStateMap.begin(); it != rhs.m_animationStateMap.end(); ++it)
				{
					m_animationStateMap[it->first] = it->second;
				}
			}
			return *this;
		}
		void Add(std::string name, std::string stateName = "")
		{
			m_animation = ResourceAllocator<Animation>::GetInstance()->Get(name);
			if (stateName.compare("") == 0)
			{
				m_animationStateMap[name] = m_animation;
			}
			else
			{
				m_animationStateMap[stateName] = m_animation;
			}
		}
		Animation* GetCurrentAnimation()
		{
			return m_animation;
		}
		std::string GetCurrentAnimationState()
		{
			return m_animationState;
		}
		void setCurrentAnimationState(const std::string state)
		{
			if (m_animationStateMap.find(state) != m_animationStateMap.end())
			{
				m_animationState = state;
				m_animation = m_animationStateMap[state];
				m_animation->Reset();
			}
			else
			{
				// TODO : Engine excpetion
			}
		}
	private:
		Animation* m_animation;
		std::string m_animationState;
		std::map<std::string, Animation*> m_animationStateMap;
	};
}
