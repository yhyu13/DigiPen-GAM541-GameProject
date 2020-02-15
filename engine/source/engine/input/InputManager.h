/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: InputManager.h
Purpose			: Contains declarations for input-system.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/29/2020
- End Header ----------------------------*/

#pragma once

#include "MouseButton.h"
#include "KeyboardKeys.h"
#include "engine/interface/IRunTimeModule.h"

namespace gswy {

	class ENGINE_API InputManager : public IRunTimeModule {

	public:

		static InputManager* GetInstance();
		~InputManager();
		virtual void Init() override {};
		virtual void Update(double deltaTime) override;
		virtual void Shutdown() override {};

		void UpdateKeyboardState(const int& key, const bool& state, const bool& repeat);
		bool IsKeyPressed(const int& keyCode);
		bool IsKeyTriggered(const int& keyCode);
		bool IsKeyReleased(const int& keyCode);

		template<class ...Args>
		bool IsAllKeyNotPressed(const Args&... args)
		{
			bool ret = false;
			std::vector<int> vec = { args... };
			for (unsigned i = 0; i < vec.size(); ++i) 
			{
				ret = ret || IsKeyPressed(vec[i]);
			}
			return !ret;
		}

		void UpdateMouseButtonState(const int& button, const bool& state);
		bool IsMouseButtonPressed(const int& mouseButton);
		bool IsMouseButtonReleased(const int& mouseButton);

		void UpdateCursorPosition(const double& positionX, const double& positionY);
		const std::pair<double, double>& GetCursorPosition();
		const double& GetMousePositionX();
		const double& GetMousePositionY();

	private:

		InputManager();

		bool m_previousKeyBoardState[KEY_LAST + 1];
		bool m_currentKeyBoardState[KEY_LAST + 1];

		bool m_previousMouseButtonState[MOUSE_BUTTON_LAST + 1];
		bool m_currentMouseButtonState[MOUSE_BUTTON_LAST + 1];

		std::pair<double, double> m_cursorPosition;
	};

}
