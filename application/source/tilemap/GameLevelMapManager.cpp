/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Language: c++ 11
Platform: Windows 10 (X64)
Project: GAM541
Author: Hang Yu (hang.yu@digipen.edu | 60001119)
Creation date: 03/02/2020
- End Header ----------------------------*/

#include "GameLevelMapManager.h"
#include <json/json.h>
#include <fstream>

/*
Start the level (start the count down, mainly)
*/

void gswy::GameLevelMapManager::StartWave()
{
	PRINT("Start Wave " + Str(m_currentWave));
	m_waveStart = true;
	m_timeOut = false;
	m_timeRemaining = m_timePerLevel;
}

/*
Check if the level is started
*/

bool gswy::GameLevelMapManager::IsWaveStarted()
{
	return m_waveStart;
}

/*
Check time out
*/

bool gswy::GameLevelMapManager::IsTimeOut()
{
	return m_timeOut;
}

/*
Check if in game or not
*/

bool gswy::GameLevelMapManager::IsInGame()
{
	return m_isAnyLevelLoaded;
}

/*
Add coins
*/

void gswy::GameLevelMapManager::AddCoins(int c)
{
	m_coins += c;
}

/*
Get coins
*/

int gswy::GameLevelMapManager::GetCoins()
{
	return m_coins;
}


/*
Try Spend Coins
*/

bool gswy::GameLevelMapManager::TrySpendCoins(int amount)
{
	if (m_coins >= amount)
	{
		m_coins -= amount;
		return true;
	}
	return false;
}

void gswy::GameLevelMapManager::Update(double dt)
{
	if (!m_isAnyLevelLoaded)
	{
		return;
	}

	auto& hud = WidgetManager::GetInstance()->GetHUD();
	int min = (int)m_timeRemaining / 60;
	int sec = (int)m_timeRemaining - min * 60;
	hud.SetTimeMinute(min);
	hud.SetTimeSecond(sec);
	hud.SetCoinNum(m_coins);
	hud.SetWave(m_currentWave);
	hud.SetLevel(m_currentLevel);

	// Check level is running and do count down
	if (m_waveStart && !m_timeOut)
	{
		m_timeRemaining -= dt;
		if (m_timeRemaining < 0)
		{
			m_timeOut = true;
			m_timeRemaining = 0;
			PRINT("Time's up, kill all reminaing monsters to finish the level!");
		}
	}
	// Check if a level is finished
	else if (IsWaveFinsihed())
	{
		// Check level remains started
		if (m_waveStart)
		{
			m_waveStart = false;
			// Stop wave and advance wave
			if (AdvanceWave())
			{
				PRINT("Load new wave");
				LoadLevel(m_world, m_currentWave);
			}
			else
			{
				// TODO: load new level
				PRINT("You have beat this level!");
				if (++m_currentLevel <= m_maxLevel)
				{
					PRINT("Load new level in 2 sec!");
					auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
					auto e = MemoryManager::Make_shared<LoadGameWorldEvent>(m_currentLevel);
					queue->Publish(e, 2);
				}
				else
				{
					// TODO : proper handle beating the game
					PRINT("You have beat the game!");
				}
			}
		}
	}
}

void gswy::GameLevelMapManager::ResetLevelData()
{
	PRINT("Reset level!");
	m_isAnyLevelLoaded = false;
	m_timeOut = false;
	m_waveStart = false;

	Json::Value root;
	std::ifstream file("./asset/archetypes/levels/sample-level.json", std::ifstream::binary);
	file >> root;
	file.close();
	Json::Value items = root["data"];

	m_coins = items["coins"].asInt();

	m_currentWave = items["wave_start"].asInt();
	m_maxWave = items["wave_max"].asInt();

	m_currentLevel = items["level_start"].asInt();
	m_maxLevel = items["level_max"].asInt();

	m_timePerLevel = items["time_per_wave"].asInt();
	m_timeRemaining = m_timePerLevel;
}

/*
A level is finished when there is no remaining enemy, this level remains started, and time is out.
*/

bool gswy::GameLevelMapManager::IsWaveFinsihed()
{
	return m_world->GetAllEntityWithType(GameObjectType::ENEMY_1).empty()
		&& m_world->GetAllEntityWithType(GameObjectType::ENEMY_2).empty()
		&& m_world->GetAllEntityWithType(GameObjectType::ENEMY_BOSS_1).empty()
		&& IsWaveStarted() && m_timeOut;
}

/*
Advance level if is possible
*/

bool gswy::GameLevelMapManager::AdvanceWave()
{
	if (++m_currentWave <= m_maxWave)
	{
		return true;
	}
	else
	{
		return false;
	}
}

gswy::GameLevelMapManager::GameLevelMapManager()
{
	ResetLevelData();
}
