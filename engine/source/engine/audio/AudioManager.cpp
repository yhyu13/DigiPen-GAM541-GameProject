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

#include "engine-precompiled-header.h"
#include "AudioManager.h"
#include "engine/exception/EngineException.h"

gswy::FMODInstance::FMODInstance()
	:
	mnNextChannelId(0)
{
	mpStudioSystem = NULL;
	AudioManager::ErrorCheck(FMOD::Studio::System::create(&mpStudioSystem));
	AudioManager::ErrorCheck(mpStudioSystem->initialize(32, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_PROFILE_ENABLE, NULL));

	mpSystem = NULL;
	AudioManager::ErrorCheck(mpStudioSystem->getCoreSystem(&mpSystem));
}

gswy::FMODInstance::~FMODInstance()
{
	AudioManager::ErrorCheck(mpStudioSystem->unloadAll());
	AudioManager::ErrorCheck(mpStudioSystem->release());
}

void gswy::FMODInstance::Update()
{
	vector<ChannelMap::iterator> pStoppedChannels;
	for (auto it = mChannels.begin(), itEnd = mChannels.end(); it != itEnd; ++it)
	{
		bool bIsPlaying = false;
		it->second->isPlaying(&bIsPlaying);
		if (!bIsPlaying)
		{
			pStoppedChannels.push_back(it);
		}
	}
	for (auto& it : pStoppedChannels)
	{
		mChannels.erase(it);
	}
	AudioManager::ErrorCheck(mpStudioSystem->update());
}

gswy::AudioManager* gswy::AudioManager::GetInstance()
{
	static AudioManager instance = AudioManager();
	return &instance;
}

void gswy::AudioManager::Init()
{
	m_fmodInstance = new FMODInstance();
}

void gswy::AudioManager::Update(double deltaTime)
{
	m_fmodInstance->Update();
}

void gswy::AudioManager::Shutdown()
{
	delete m_fmodInstance;
}

int gswy::AudioManager::ErrorCheck(FMOD_RESULT result)
{
	if (result != FMOD_OK) {
		cout << "FMOD ERROR " << result << endl;
		return 1;
	}
	// cout << "FMOD all good" << endl;
	return 0;
}

int gswy::AudioManager::GetSoundChannel(const string& strSoundName)
{
	if (m_fmodInstance->mSound2Channels.find(strSoundName) != m_fmodInstance->mSound2Channels.end())
	{
		m_fmodInstance->mSound2Channels[strSoundName];
	}
	else
	{
		throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Sound at " + str2wstr(strSoundName) + L" has failed to load!");
	}
}

void gswy::AudioManager::LoadBank(const string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags)
{
	auto tFoundIt = m_fmodInstance->mBanks.find(strBankName);
	if (tFoundIt != m_fmodInstance->mBanks.end())
		return;
	FMOD::Studio::Bank* pBank;
	AudioManager::ErrorCheck(m_fmodInstance->mpStudioSystem->loadBankFile(strBankName.c_str(), flags, &pBank));
	if (pBank) {
		m_fmodInstance->mBanks[strBankName] = pBank;
	}
}

void gswy::AudioManager::LoadEvent(const string& strEventName)
{
	auto tFoundit = m_fmodInstance->mEvents.find(strEventName);
	if (tFoundit != m_fmodInstance->mEvents.end())
		return;
	FMOD::Studio::EventDescription* pEventDescription = NULL;
	AudioManager::ErrorCheck(m_fmodInstance->mpStudioSystem->getEvent(strEventName.c_str(), &pEventDescription));
	if (pEventDescription) {
		FMOD::Studio::EventInstance* pEventInstance = NULL;
		AudioManager::ErrorCheck(pEventDescription->createInstance(&pEventInstance));
		if (pEventInstance) {
			m_fmodInstance->mEvents[strEventName] = pEventInstance;
		}
	}
}

void gswy::AudioManager::LoadSound(const string& strSoundName, bool bLooping, bool b3d, bool bStream)
{
	auto tFoundIt = m_fmodInstance->mSounds.find(strSoundName);
	if (tFoundIt != m_fmodInstance->mSounds.end())
		return;

	FMOD_MODE eMode = FMOD_DEFAULT;
	eMode |= b3d ? FMOD_3D : FMOD_2D;
	eMode |= bLooping ? FMOD_LOOP_NORMAL : FMOD_LOOP_OFF;
	eMode |= bStream ? FMOD_CREATESTREAM : FMOD_CREATECOMPRESSEDSAMPLE;

	FMOD::Sound* pSound = nullptr;
	AudioManager::ErrorCheck(m_fmodInstance->mpSystem->createSound(strSoundName.c_str(), eMode, nullptr, &pSound));
	if (pSound) {
		m_fmodInstance->mSounds[strSoundName] = pSound;
	}
}

void gswy::AudioManager::UnLoadSound(const string& strSoundName)
{
	auto tFoundIt = m_fmodInstance->mSounds.find(strSoundName);
	if (tFoundIt == m_fmodInstance->mSounds.end())
		return;

	AudioManager::ErrorCheck(tFoundIt->second->release());
	m_fmodInstance->mSounds.erase(tFoundIt);
}

void gswy::AudioManager::Set3dListenerAndOrientation(const AudioVector3& vPos, float fVolumedB)
{
}

int gswy::AudioManager::PlaySound(const string& strSoundName, const AudioVector3& vPos, float fVolumedB, float frequency)
{
	int nChannelId = m_fmodInstance->mnNextChannelId++;
	auto tFoundIt = m_fmodInstance->mSounds.find(strSoundName);
	if (tFoundIt == m_fmodInstance->mSounds.end())
	{
		LoadSound(strSoundName);
		tFoundIt = m_fmodInstance->mSounds.find(strSoundName);
		if (tFoundIt == m_fmodInstance->mSounds.end())
		{
			throw EngineException(_CRT_WIDE(__FILE__), __LINE__, L"Sound at " + str2wstr(strSoundName) + L" has failed to load!");
		}
	}
	FMOD::Channel* pChannel = nullptr;
	AudioManager::ErrorCheck(m_fmodInstance->mpSystem->playSound(tFoundIt->second, nullptr, true, &pChannel));
	if (pChannel)
	{
		FMOD_MODE currMode;
		tFoundIt->second->getMode(&currMode);
		if (currMode & FMOD_3D) {
			FMOD_VECTOR position = VectorToFmod(vPos);
			AudioManager::ErrorCheck(pChannel->set3DAttributes(&position, nullptr));
		}
		AudioManager::ErrorCheck(pChannel->setVolume(dbToVolume(fVolumedB)));
		AudioManager::ErrorCheck(pChannel->setPaused(false));
		AudioManager::ErrorCheck(pChannel->setPitch(frequency));
		m_fmodInstance->mChannels[nChannelId] = pChannel;
	}
	return nChannelId;
}

void gswy::AudioManager::SetSoundFreqency(const string& strSoundName, float frequency)
{
	m_fmodInstance->mChannels[GetSoundChannel(strSoundName)]->setPitch(frequency);
}

void gswy::AudioManager::PlayEvent(const string& strEventName)
{
	auto tFoundit = m_fmodInstance->mEvents.find(strEventName);
	if (tFoundit == m_fmodInstance->mEvents.end()) {
		LoadEvent(strEventName);
		tFoundit = m_fmodInstance->mEvents.find(strEventName);
		if (tFoundit == m_fmodInstance->mEvents.end())
			return;
	}
	tFoundit->second->start();
}

void gswy::AudioManager::StopChannel(int nChannelId)
{
	auto tFoundIt = m_fmodInstance->mChannels.find(nChannelId);
	if (tFoundIt == m_fmodInstance->mChannels.end())
		return;

	AudioManager::ErrorCheck(tFoundIt->second->stop());
}

void gswy::AudioManager::StopEvent(const string& strEventName, bool bImmediate)
{
	auto tFoundIt = m_fmodInstance->mEvents.find(strEventName);
	if (tFoundIt == m_fmodInstance->mEvents.end())
		return;

	FMOD_STUDIO_STOP_MODE eMode;
	eMode = bImmediate ? FMOD_STUDIO_STOP_IMMEDIATE : FMOD_STUDIO_STOP_ALLOWFADEOUT;
	AudioManager::ErrorCheck(tFoundIt->second->stop(eMode));
}
	
void gswy::AudioManager::StopAllChannels()
{
	for (auto iter = m_fmodInstance->mChannels.begin(); iter != m_fmodInstance->mChannels.end(); ++iter)
	{
		AudioManager::ErrorCheck(iter->second->stop());
	}
}

void gswy::AudioManager::SetChannel3dPosition(int nChannelId, const AudioVector3& vPosition)
{
	auto tFoundIt = m_fmodInstance->mChannels.find(nChannelId);
	if (tFoundIt == m_fmodInstance->mChannels.end())
		return;

	FMOD_VECTOR position = VectorToFmod(vPosition);
	AudioManager::ErrorCheck(tFoundIt->second->set3DAttributes(&position, NULL));
}

void gswy::AudioManager::SetChannelvolume(int nChannelId, float fVolumedB)
{
	auto tFoundIt = m_fmodInstance->mChannels.find(nChannelId);
	if (tFoundIt == m_fmodInstance->mChannels.end())
		return;

	AudioManager::ErrorCheck(tFoundIt->second->setVolume(dbToVolume(fVolumedB)));
}

bool gswy::AudioManager::IsPlaying(int nChannelId) const
{
	auto tFoundIt = m_fmodInstance->mChannels.find(nChannelId);
	if (tFoundIt == m_fmodInstance->mChannels.end())
		return false;
	bool bIsPlaying;
	tFoundIt->second->isPlaying(&bIsPlaying);
	return bIsPlaying;
}

bool gswy::AudioManager::IsEventPlaying(const string& strEventName) const
{
	auto tFoundIt = m_fmodInstance->mEvents.find(strEventName);
	if (tFoundIt == m_fmodInstance->mEvents.end())
		return false;

	FMOD_STUDIO_PLAYBACK_STATE* state = NULL;
	if (tFoundIt->second->getPlaybackState(state) == FMOD_STUDIO_PLAYBACK_PLAYING) {
		return true;
	}
	return false;
}

float gswy::AudioManager::dbToVolume(float db)
{
	return powf(10.0f, 0.05f * db);
}

float gswy::AudioManager::VolumeTodb(float volume)
{
	return 20.0f * log10f(volume);
}

FMOD_VECTOR gswy::AudioManager::VectorToFmod(const AudioVector3& vPosition)
{
	FMOD_VECTOR fVec;
	fVec.x = vPosition.x;
	fVec.y = vPosition.y;
	fVec.z = vPosition.z;
	return fVec;
}
