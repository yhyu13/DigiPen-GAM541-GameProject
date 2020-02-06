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
	/*
	Data class that stores references to the current animation and maintain an animation state map
	*/
	struct AnimationCom : BaseComponent<AnimationCom> {

		void Add(std::string name, std::string stateName)
		{
			m_animationStateMap[stateName] = ResourceAllocator<Animation>::GetInstance()->Get(name);
		}
		std::shared_ptr<Animation> GetCurrentAnimation()
		{
			if (auto temp = m_animation.lock())
			{
				return temp;
			}
			else
			{
				// TODO : Engine excpetion
				return nullptr;
			}
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
				if (auto temp = m_animation.lock())
				{
					temp->Reset();
				}
				else
				{
					// TODO : Engine excpetion
				}
			}
			else
			{
				// TODO : Engine excpetion
			}
		}
	private:
		std::weak_ptr<Animation> m_animation;
		std::string m_animationState;
		std::map<std::string, std::weak_ptr<Animation>> m_animationStateMap;
	};
}
