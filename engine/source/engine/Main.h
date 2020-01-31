/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Main.h
Purpose			: Entry-point into the application
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#pragma once

extern gswy::Engine* gswy::CreateEngineApplication();

int main(int argc, char** argv) {
	auto application = gswy::CreateEngineApplication();
	application->Run();
	delete application;
}