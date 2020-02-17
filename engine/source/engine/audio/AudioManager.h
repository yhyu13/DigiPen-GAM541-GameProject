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

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

#include "engine/EngineCore.h"
#include "engine/interface/IRunTimeModule.h"

using namespace std;

namespace gswy {


	struct ENGINE_API AudioVector3 {
		float x;
		float y;
		float z;
	};

	struct ENGINE_API FMODInstance {
		FMODInstance();
		~FMODInstance();

		void Update();

		FMOD::Studio::System* mpStudioSystem;
		FMOD::System* mpSystem;

		int mnNextChannelId;

		typedef map<string, FMOD::Sound*> SoundMap;
		typedef map<string, int> Sound2ChannelMap;
		typedef map<int, FMOD::Channel*> ChannelMap;
		typedef map<string, FMOD::Studio::EventInstance*> EventMap;
		typedef map<string, FMOD::Studio::Bank*> BankMap;

		BankMap mBanks;
		EventMap mEvents;
		SoundMap mSounds;
		ChannelMap mChannels;
		Sound2ChannelMap mSound2Channels;
	};

	class ENGINE_API AudioManager : public IRunTimeModule{
	public:
		static AudioManager* GetInstance();
		virtual void Init() override;
		virtual void Update(double deltaTime) override;
		virtual void Shutdown() override;
		static int ErrorCheck(FMOD_RESULT result);

		int GetSoundChannel(const string& strSoundName);
		void LoadBank(const string& strBankName, FMOD_STUDIO_LOAD_BANK_FLAGS flags);
		void LoadEvent(const string& strEventName);
		/*
			Load sound from path
		*/
		void LoadSound(const string& strSoundName, bool bLooping = false, bool b3d = true, bool bStream = true);
		void UnLoadSound(const string& strSoundName);
		void Set3dListenerAndOrientation(const AudioVector3& vPos = AudioVector3{ 0, 0, 0 }, float fVolumedB = 0.0f);
		int PlaySound(const string& strSoundName, const AudioVector3& vPos = AudioVector3{ 0, 0, 0 }, float fVolumedB = 1.0f, float frequency = 1.0f);
		void SetSoundFreqency(const string& strSoundName, float frequency);
		void PlayEvent(const string& strEventName);
		void StopChannel(int nChannelId);
		void StopEvent(const string& strEventName, bool bImmediate = false);
		void StopAllChannels();
		void SetChannel3dPosition(int nChannelId, const AudioVector3& vPosition);
		void SetChannelvolume(int nChannelId, float fVolumedB);
		bool IsPlaying(const string& strSoundName);
		bool IsPlaying(int nChannelId);
		bool IsEventPlaying(const string& strEventName) const;
		float dbToVolume(float db);
		float VolumeTodb(float volume);
		FMOD_VECTOR VectorToFmod(const AudioVector3& vPosition);
	private:
		FMODInstance* m_fmodInstance;
	};

}