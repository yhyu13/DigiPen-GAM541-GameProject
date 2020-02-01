/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Input.cpp
Purpose			: Contains definitions for input-system.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/29/2020
- End Header ----------------------------*/

#include <memory>
#include <sstream>

#include "engine/EngineCore.h"
#include "Input.h"

namespace gswy {

	Input* Input::GetInstance() {
		static Input input;
		return &input;
	}

	Input::Input() : m_cursorPosition( {0.0f, 0.0f} ) {
		memset(m_previousKeyBoardState, false, sizeof(bool) * (KEY_LAST + 1));
		memset(m_currentKeyBoardState, false, sizeof(bool) * (KEY_LAST + 1));

		memset(m_previousMouseButtonState, false, sizeof(bool) * (MOUSE_BUTTON_LAST + 1));
		memset(m_currentMouseButtonState, false, sizeof(bool) * (MOUSE_BUTTON_LAST + 1));
	}

	Input::~Input() {
	}

	void Input::UpdateKeyboardState(const int& key, const bool& state, const bool& repeat) {

		if (!state) { // if released
			m_previousKeyBoardState[key] = m_currentKeyBoardState[key];
			m_currentKeyBoardState[key] = false;
		}
		else if (state && repeat) { // if pressed
			m_previousKeyBoardState[key] = true;
			m_currentKeyBoardState[key] = true;
		}
		else if (state && !repeat) { // if triggered
			m_previousKeyBoardState[key] = false;
			m_currentKeyBoardState[key] = true;
		}
	}

	void Input::UpdateMouseButtonState(const int& button, const bool& state) {
		if (!state) { // if released
			m_previousMouseButtonState[button] = m_currentMouseButtonState[button];
			m_currentMouseButtonState[button] = false;
		}
		else { // if pressed
			m_previousMouseButtonState[button] = false;
			m_currentMouseButtonState[button] = true;
		}
	}


	bool Input::IsKeyPressed(const int& keyCode) {
		ASSERT(keyCode > KEY_LAST, "Invalid key-code.");
		return m_currentKeyBoardState[keyCode];
	}

	bool Input::IsKeyTriggered(const int& keyCode) {
		ASSERT(keyCode > KEY_LAST, "Invalid key-code.");

		if (m_currentKeyBoardState[keyCode] && !m_previousKeyBoardState[keyCode]) {
			m_currentKeyBoardState[keyCode] = false; // reset the state once the input has been polled
			return true;
		}
		return false;
	}

	bool Input::IsKeyReleased(const int& keyCode) {
		ASSERT(keyCode > KEY_LAST, "Invalid key-code.");

		if (!m_currentKeyBoardState[keyCode] && m_previousKeyBoardState[keyCode]) {
			m_previousKeyBoardState[keyCode] = false; // reset the state once the input has been polled
			return true;
		}
		return false;
	}

	bool Input::IsMouseButtonPressed(const int& mouseButton) {
		ASSERT(mouseButton > MOUSE_BUTTON_LAST, "Invalid mouse-button.");
		return m_currentMouseButtonState[mouseButton];
	}

	bool Input::IsMouseButtonReleased(const int& mouseButton) {
		ASSERT(mouseButton > MOUSE_BUTTON_LAST, "Invalid mouse-button.");

		if (!m_currentMouseButtonState[mouseButton] && m_previousMouseButtonState[mouseButton]) {
			m_previousMouseButtonState[mouseButton] = false; // reset the state once the input has been polled
			return true;
		}
		return false;
	}

	void Input::UpdateCursorPosition(const double& positionX, const double& positionY) {
		m_cursorPosition.first = positionX;
		m_cursorPosition.second = positionY;
	}

	const std::pair<double, double>& Input::GetCursorPosition() {
		return m_cursorPosition;
	}

	const double& Input::GetMousePositionX() {
		return m_cursorPosition.first;
	}

	const double& Input::GetMousePositionY() {
		return m_cursorPosition.second;
	}
}