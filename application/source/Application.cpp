/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Filename: Application.cpp
Purpose: Game application class. Creates an instance of the game application which plugs into the engine.
Author: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date: 01/26/2020
- End Header ----------------------------*/

#include "EngineExport.h"

using namespace gswy;

class Application : public Engine {

public:

	Application() {
		auto audioManager = AudioManager::GetInstance();
		audioManager->Init();
	}

	virtual ~Application() {
		auto audioManager = AudioManager::GetInstance();
		audioManager->Shutdown();
	}

	virtual void Run() override
	{
		auto audioManager = AudioManager::GetInstance();
		while (m_isRunning) {
			std::cout << "App is running..." << std::endl;

			static int bgm_channel = audioManager->PlaySound("./asset/breakout.mp3", AudioVector3{ 0, 0, 0 }, 1);

			audioManager->Update(0);
			m_window->Update();

			m_isRunning = !m_window->ShouldExit();
		}
	}

protected:

private:
};

Engine* gswy::CreateEngineApplication() {
	return new Application();
}

