
#include <memory>
#include <glfw3.h>
#include <sstream>

#include "engine/EngineCore.h"
#include "InputManager.h"

namespace gswy {

	InputManager* InputManager::GetInstance() {
		static InputManager manager;
		return &manager;
	}

	InputManager::InputManager() {
		memset(m_previousKeyBoardState, false, sizeof(bool) * 348);
		memset(m_currentKeyBoardState, false, sizeof(bool) * 348);

		//std::cout << "current state A  :   " << m_currentKeyBoardState[GLFW_KEY_A];
	}

	InputManager::~InputManager() {

	}

	void InputManager::UpdateKeyboardState(int key, bool state, bool repeat) {
		std::stringstream stream;
		stream << "Key: " << key << " state: " << state << " repeat: " << repeat;
		DEBUG_PRINT(stream.str());

		if (!state) { // if released
			m_previousKeyBoardState[key] = m_currentKeyBoardState[key];
			m_currentKeyBoardState[key] = false;
		}
		else if (state && repeat) { // if pressed and repeat
			m_previousKeyBoardState[key] = true;
			m_currentKeyBoardState[key] = true;
		}
		else if (state && !repeat) { // if pressed , i.e, triggered
			m_previousKeyBoardState[key] = false;
			m_currentKeyBoardState[key] = true;
		}
	}


	bool InputManager::IsKeyPressed(unsigned int keyScanCode) {
		if (keyScanCode > 348) {
			return false;
		}

		//std::cout << "IS KEY PRESSED : " << keyScanCode << "  state: " << m_currentKeyBoardState[keyScanCode] << std::endl;

		if (m_currentKeyBoardState[keyScanCode]) {
			return true;
		}

		return false;
	}

	bool InputManager::IsKeyTriggered(unsigned int keyScanCode) {
		if (keyScanCode > 348) {
			return false;
		}

		if (m_currentKeyBoardState[keyScanCode] && !m_previousKeyBoardState[keyScanCode]) {
			m_currentKeyBoardState[keyScanCode] = false;
			return true;
		}
		return false;
	}

	bool InputManager::IsKeyReleased(unsigned int keyScanCode) {
		if (keyScanCode > 348) {
			return false;
		}

		if (!m_currentKeyBoardState[keyScanCode] && m_previousKeyBoardState[keyScanCode]) {
			m_previousKeyBoardState[keyScanCode] = false;
			return true;
		}

		return false;
	}


}