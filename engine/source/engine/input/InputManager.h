/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: InputManager.h
Purpose			: Contains declarations for input-system.
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
				  Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date	: 01/29/2020
- End Header ----------------------------*/

#pragma once
#include "engine/math/MathHelper.h"
#include "engine/input/MouseButton.h"
#include "engine/input/KeyboardKeys.h"
#include "engine/interface/IRunTimeModule.h"

namespace gswy {

	class ENGINE_API InputManager : public IRunTimeModule {

	public:

		static InputManager* GetInstance();
		~InputManager();
		virtual void Init() override {};
		virtual void Update(double deltaTime) override;
		virtual void LateUpdate(double deltaTime) override;
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
		bool IsMouseButtonTriggered(const int& mouseButton);
		bool IsMouseButtonPressed(const int& mouseButton);
		bool IsMouseButtonReleased(const int& mouseButton);

		void UpdateCursorPosition(const double& positionX, const double& positionY);
		const glm::vec2& const GetCursorPosition() const;
		const double GetMousePositionX();
		const double GetMousePositionY();
		void SetMouseMaxPositions(double x, double y);
		const glm::vec2& const GetCursorMaxPosition() const;
		const double GetMouseMaxPositionX();
		const double GetMouseMaxPositionY();

		const glm::vec2 GetCursorViewPosition(float _x, float _y);
		const glm::vec2 GetCursorViewPosition();
		float lerp(float x, float y, float t);

	private:

		InputManager();

		bool m_previousKeyBoardState[KEY_LAST + 1];
		bool m_currentKeyBoardState[KEY_LAST + 1];

		bool m_previousMouseButtonState[MOUSE_BUTTON_LAST + 1];
		bool m_currentMouseButtonState[MOUSE_BUTTON_LAST + 1];

		glm::vec2 m_cursorPosition;
		glm::vec2 m_cursorMaxPosition;
	};

}
