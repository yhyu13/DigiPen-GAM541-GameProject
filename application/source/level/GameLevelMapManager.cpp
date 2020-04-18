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
#include "ecs/components/HitPointCom.h"
#include "ecs/components/ActiveCom.h"
#include <json/json.h>
#include <fstream>

/*
Start the level (start the count down, mainly)
*/

bool gswy::GameLevelMapManager::IsLoading()
{
	return m_bIsLoading;
}

void gswy::GameLevelMapManager::SetIsLoading(bool b)
{
	m_bIsLoading = b;
}

void gswy::GameLevelMapManager::StartWave()
{
	DEBUG_PRINT("Start Wave " + Str(m_currentWave));
	m_waveStart = true;
	m_timeOut = false;
	m_timeRemaining = m_timePerWave;

	if (m_world)
	{
		auto indicator = m_world->GetAllEntityWithType(GameObjectType::START_WAVE_INDICATOR);
		if (!indicator.empty())
		{
			ComponentDecorator<ActiveCom, GameObjectType> active;
			m_world->Unpack(indicator[0], active);
			active->SetActive(false);
		}
	}

	// Show final wave indicator on the screen
	if (m_currentWave == m_maxWave)
	{
		auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
		auto e2 = MemoryManager::Make_shared<LoadFinalWaveEvent>(m_currentWave);
		queue->Publish(e2);
	}
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
	if (!IsInGame())
	{
		return;
	}

	// Set UI widgets with game data
	auto& hud = WidgetManager::GetInstance()->GetHUD();
	int min = (int)m_timeRemaining / 60;
	int sec = (int)m_timeRemaining - min * 60;
	hud.SetTimeMinute(min);
	hud.SetTimeSecond(sec);
	hud.SetCoinNum(m_coins);
	hud.SetWave(m_currentWave);
	hud.SetLevel(m_currentLevel);
	hud.SetKill(m_Kill);

	if (m_world)
	{
		auto player = m_world->GetAllEntityWithType(GameObjectType::PLAYER);
		if (!player.empty())
		{
			ComponentDecorator<HitPointCom, GameObjectType> hp;
			m_world->Unpack(player[0], hp);
			hud.SetPlayerHP(hp->GetPercentageHP());
		}
		auto base = m_world->GetAllEntityWithType(GameObjectType::BASE);
		if (!base.empty())
		{
			ComponentDecorator<HitPointCom, GameObjectType> hp;
			m_world->Unpack(base[0], hp);
			hud.SetBaseHP(hp->GetPercentageHP());
		}
	}

	if (!IsWaveStarted() && !IsLoading())
	{
		// Show wave start indicator
		if (m_world)
		{
			auto indicator = m_world->GetAllEntityWithType(GameObjectType::START_WAVE_INDICATOR);
			if (!indicator.empty())
			{
				ComponentDecorator<ActiveCom, GameObjectType> active;
				m_world->Unpack(indicator[0], active);
				active->SetActive(true);
			}
		}
	}

	// Check level is running and do count down
	if (m_waveStart && !m_timeOut)
	{
		m_timeRemaining -= dt;
		if (m_timeRemaining < 0)
		{
			m_timeOut = true;
			m_timeRemaining = 0;
			DEBUG_PRINT("Time's up, kill all reminaing monsters to finish the level!");
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
				DEBUG_PRINT("Load new wave");
				LoadLevel(m_world, m_currentWave);

				// Show wave clear indicator
				auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
				auto e1 = MemoryManager::Make_shared<LoadWaveClearEvent>(m_currentWave);
				queue->Publish(e1);
			}
			else
			{
				// TODO: load new level
				DEBUG_PRINT("You have beat this level!");
				if (AdvanceLevel())
				{
					DEBUG_PRINT("Load new level in 5 sec!");
					// Show new level indicator
					auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
					auto e1 = MemoryManager::Make_shared<LoadLevelClearEvent>(m_currentLevel);
					queue->Publish(e1);
					SetIsLoading(true);
				}
				else
				{
					// TODO : proper handle beating the game
					DEBUG_PRINT("You have beat the game!");
					// Show won indicator
					auto queue = EventQueue<GameObjectType, EventType>::GetInstance();
					auto e1 = MemoryManager::Make_shared<LoadWonEvent>();
					queue->Publish(e1);
					SetIsLoading(true);
				}
			}
		}
	}
}

void gswy::GameLevelMapManager::ResetLevelData()
{
	DEBUG_PRINT("Reset level!");
	m_isAnyLevelLoaded = false;
	m_timeOut = false;
	m_waveStart = false;
	m_bIsLoading = false;
	m_Kill = 0;

	Json::Value root;
	std::ifstream file("./asset/archetypes/levels/sample-level.json", std::ifstream::binary);
	file >> root;
	file.close();
	Json::Value items = root["data"];

	m_coins = items["coins"].asInt();
	ASSERT(!(m_coins >= 0), "Coin must be positive or zero!");

	m_currentLevel = items["level_start"].asInt();
	m_maxLevel = items["level_max"].asInt();
	ASSERT(!(m_maxLevel >= m_currentLevel && m_currentLevel >= 0 && m_maxLevel<=3), "Level setup error!");

	m_levelData.clear();
	// Load all four levels
	for (int i = 0; i < 4; i++)
	{
		LevelData data;
		Json::Value data_ = items["Level_"+Str(i)];
		data.wave_start = data_["wave_start"].asInt();
		data.wave_max = data_["wave_max"].asInt();
		ASSERT(!(data.wave_max >= data.wave_start && data.wave_start >= 0), "Wave setup error!");

		data.time_per_wave = data_["time_per_wave"].asDouble();
		ASSERT(!(data.time_per_wave >= 0), "Time setup error!");

		m_levelData.push_back(data);
	}

	m_currentWave = m_levelData[m_currentLevel].wave_start;
	m_maxWave = m_levelData[m_currentLevel].wave_max;
	m_timePerWave = m_levelData[m_currentLevel].time_per_wave;
}

/*
A level is finished when there is no remaining enemy, this level remains started, and time is out.
*/

bool gswy::GameLevelMapManager::IsWaveFinsihed()
{
	return m_world->GetAllEntityWithType(GameObjectType::ENEMY_1).empty()
		&& m_world->GetAllEntityWithType(GameObjectType::ENEMY_2).empty()
		&& m_world->GetAllEntityWithType(GameObjectType::ENEMY_BOSS_1).empty()
		&& m_world->GetAllEntityWithType(GameObjectType::ENEMY_BOSS_2).empty()
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

bool gswy::GameLevelMapManager::AdvanceLevel()
{
	if (++m_currentLevel <= m_maxLevel)
	{
		m_currentWave = m_levelData[m_currentLevel].wave_start;
		m_maxWave = m_levelData[m_currentLevel].wave_max;
		m_timePerWave = m_levelData[m_currentLevel].time_per_wave;
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
