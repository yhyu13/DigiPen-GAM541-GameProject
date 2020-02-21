/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Dushyant Shukla (dushyant.shukla@digipen.edu | 60000519)
Creation date: 02/19/2020
- End Header ----------------------------*/

#include "GameObjectFactory.h"


namespace gswy {

	GameObjectFactory* GameObjectFactory::GetInstance() {
		static GameObjectFactory objectFactory;
		return &objectFactory;
	}

	GameObjectFactory::~GameObjectFactory() {
	}

	GameObjectFactory::GameObjectFactory() {
	}
}