/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: InputManager.cpp
Purpose			: Contains definitions for input-system.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/29/2020
- End Header ----------------------------*/

#include "engine-precompiled-header.h"
#include "engine/EngineCore.h"
#include "engine/input/InputManager.h"

using namespace glm;

namespace gswy {

	InputManager* InputManager::GetInstance() {
		static InputManager input;
		return &input;
	}

	InputManager::InputManager() 
		: m_cursorPosition( glm::vec2(0.0f, 0.0f) ), 
		m_cursorMaxPosition(glm::vec2(0.0f, 0.0f))
	{
		memset(m_previousKeyBoardState, false, sizeof(bool) * (KEY_LAST + 1));
		memset(m_currentKeyBoardState, false, sizeof(bool) * (KEY_LAST + 1));

		memset(m_previousMouseButtonState, false, sizeof(bool) * (MOUSE_BUTTON_LAST + 1));
		memset(m_currentMouseButtonState, false, sizeof(bool) * (MOUSE_BUTTON_LAST + 1));
	}

	InputManager::~InputManager() {
	}

	void InputManager::Update(double deltaTime)
	{
	}

	void InputManager::LateUpdate(double deltaTime)
	{
		for (int i = 0; i < KEY_LAST + 1; ++i)
		{
			m_previousKeyBoardState[i] = m_currentKeyBoardState[i];
		}
		for (int i = 0; i < MOUSE_BUTTON_LAST + 1; ++i)
		{
			m_previousMouseButtonState[i] = m_currentMouseButtonState[i];
		}
	}

	void InputManager::UpdateKeyboardState(const int& key, const bool& state, const bool& repeat) {
		m_currentKeyBoardState[key] = state;
	}

	void InputManager::UpdateMouseButtonState(const int& button, const bool& state) {
		m_currentMouseButtonState[button] = state;
	}

	bool InputManager::IsKeyPressed(const int& keyCode) {
		ASSERT(keyCode > KEY_LAST, "Invalid key-code.");
		return m_currentKeyBoardState[keyCode];
	}

	bool InputManager::IsKeyTriggered(const int& keyCode) {
		ASSERT(keyCode > KEY_LAST, "Invalid key-code.");

		if (m_currentKeyBoardState[keyCode] && !m_previousKeyBoardState[keyCode]) {
			return true;
		}
		return false;
	}

	bool InputManager::IsKeyReleased(const int& keyCode) {
		ASSERT(keyCode > KEY_LAST, "Invalid key-code.");

		if (!m_currentKeyBoardState[keyCode] && m_previousKeyBoardState[keyCode]) {
			return true;
		}
		return false;
	}

	bool InputManager::IsMouseButtonTriggered(const int& mouseButton)
	{
		ASSERT(mouseButton > MOUSE_BUTTON_LAST, "Invalid mouse-button.");
		if (m_currentMouseButtonState[mouseButton] && !m_previousMouseButtonState[mouseButton]) {
			return true;
		}
		return false;
	}


	bool InputManager::IsMouseButtonPressed(const int& mouseButton) {
		ASSERT(mouseButton > MOUSE_BUTTON_LAST, "Invalid mouse-button.");
		return m_currentMouseButtonState[mouseButton];
	}

	bool InputManager::IsMouseButtonReleased(const int& mouseButton) {
		ASSERT(mouseButton > MOUSE_BUTTON_LAST, "Invalid mouse-button.");

		if (!m_currentMouseButtonState[mouseButton] && m_previousMouseButtonState[mouseButton]) {
			return true;
		}
		return false;
	}

	void InputManager::UpdateCursorPosition(const double& positionX, const double& positionY) {
		m_cursorPosition.x = positionX;
		m_cursorPosition.y = positionY;
	}

	const glm::vec2& const InputManager::GetCursorPosition() const 
	{
		return m_cursorPosition;
	}

	const double InputManager::GetMousePositionX() 
	{
		return m_cursorPosition.x;
	}

	const double InputManager::GetMousePositionY() 
	{
		return m_cursorPosition.y;
	}
	void InputManager::SetMouseMaxPositions(double x, double y)
	{
		m_cursorMaxPosition.x = x;
		m_cursorMaxPosition.y = y;
	}
	const glm::vec2& const InputManager::GetCursorMaxPosition() const
	{
		return m_cursorMaxPosition;
	}
	const double InputManager::GetMouseMaxPositionX()
	{
		return m_cursorMaxPosition.x;
	}
	const double InputManager::GetMouseMaxPositionY()
	{
		return m_cursorMaxPosition.y;
	}
}