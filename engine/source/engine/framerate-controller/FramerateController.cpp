/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: FramerateController.h
Purpose			: Includes definitions for FrameRateController
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/28/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "FramerateController.h"
#include "engine/profiling/InstrumentorCore.h"

#include <GLFW/glfw3.h>

namespace gswy {

	/*
		FramerateController will always have only one instance throughout the execution of the application.
	*/
	FramerateController* FramerateController::GetInstance(unsigned int maxFrameRate) {
		static FramerateController frameRateController(maxFrameRate);
		return &frameRateController;
	}

	FramerateController::FramerateController(unsigned int maxFrameRate) : m_maxFramerate(maxFrameRate), m_tickStart(0), m_tickEnd(0), m_ticksPerFrame(0.0f) {

		if (0 >= m_maxFramerate) {
			m_maxFramerate = UINT_MAX;
		}
		m_ticksPerFrame = 1000.0f / m_maxFramerate;
		m_frameTime = 1.0 / m_maxFramerate;
	}

	FramerateController::~FramerateController() {
	}

	void FramerateController::FrameStart() {
		m_tickStart = glfwGetTime() * 1000; // glfwGetTime() returns time in seconds passed
	}

	void FramerateController::FrameEnd() {
		do {
			m_tickEnd = glfwGetTime() * 1000;
		} while ((m_tickEnd - m_tickStart) < m_ticksPerFrame);

		m_frameTime = (m_tickEnd - m_tickStart);

		//#ifdef _DEBUG
		Instrumentor::GetInstance()->AddInstrumentorResult({ "Frame Time", m_frameTime, "ms" });
		Instrumentor::GetInstance()->AddInstrumentorResult({ "FPS", 1000.0f / m_frameTime, "  " });
		//#endif

		if (m_frameTime > 16.999) { // for handling game pauses or break points
			m_frameTime = 16.667;
		}
		m_frameTime /= 1000; // converting milliseconds to seconds
	}

	const double& FramerateController::GetFrameTime() const {
		return m_frameTime;
	}
}