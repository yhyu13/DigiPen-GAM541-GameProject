/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: FramerateController.h
Purpose			: Includes declarations for FrameRateController
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/28/2020
- End Header ----------------------------*/

#pragma once

#include "engine/EngineCore.h"

namespace gswy {

	class ENGINE_API FramerateController {

	public:

		static FramerateController* GetInstance(unsigned int maxFrameRate);
		~FramerateController();

		void FrameStart();
		void FrameEnd();

		/*
			Returns the frame-time in seconds
		*/
		const double& GetFrameTime() const;

	private:

		FramerateController(unsigned int maxFrameRate);

	private:

		unsigned int m_maxFramerate;
		double m_tickStart;
		double m_tickEnd;

		double m_ticksPerFrame;
		double m_frameTime;

	};

}