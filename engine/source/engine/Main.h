/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language		: C++ 11
Platform		: Windows 10 (X64)
Project			: GAM541
Filename		: Main.h
Purpose			: Entry-point into the application
Author			: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519),
				  Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date	: 01/26/2020
- End Header ----------------------------*/

#pragma once
#include "engine/exception/EngineException.h"

#include "rttr/registration.h"

static void f() { std::cout << "Hello World" << std::endl; }
using namespace rttr;
RTTR_REGISTRATION
{
	using namespace rttr;
	registration::method("f", &f);
}

extern gswy::Engine* gswy::CreateEngineApplication();

int main(int argc, char** argv) {
	try
	{
		auto application = gswy::CreateEngineApplication();
		type::invoke("f", {});
		application->Run();
		delete application;
	}
	catch (const EngineException & e)
	{
		const std::wstring eMsg = e.GetFullMessage() +
			L"\n\nException caught at main window creation.";
		std::wcerr << e.GetExceptionType().c_str() << std::endl;
		std::wcerr << eMsg.c_str() << std::endl;
	}
	catch (const std::exception & e)
	{
		const std::string whatStr(e.what());
		const std::wstring eMsg = std::wstring(whatStr.begin(), whatStr.end()) +
			L"\n\nException caught at main window creation.";
		std::wcerr << "Unhandled STL Exception" << std::endl;
		std::wcerr << eMsg.c_str() << std::endl;
	}
	catch (...)
	{
		std::wcerr << L"Unhandled Non-STL Exception" << std::endl;
		std::wcerr << L"Exception caught at main window creation." << std::endl;
	}
}