/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 01/28/2020
- End Header ----------------------------*/

#pragma once

namespace gswy {
	class ENGINE_API IRunTimeModule
	{
	public:
		virtual ~IRunTimeModule() {};
		virtual void Init() = 0;
		virtual void Update(double deltaTime) = 0;
		virtual void Shutdown() = 0;
	};

}