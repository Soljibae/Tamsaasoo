#pragma once
#include "Actor.h"
#include <map>
#include <unordered_map>
#include <array>
#include <iostream>
#include <vector>
namespace InGame
{
	enum SFXType
	{
		SYSTEM = 0,
		ENEMY = 1,
		PLAYER = 2,
		SFX = 3,
		BGM = 4,
		UI = 5,
		SFXType_LAST = UI
	};

	struct SFXData
	{
	public:
		SFXType type;
		AEAudio audio;
		f32 volume{ 1.f };
		f32 pitch{ 1.f };
		std::string name;
		bool play{ false };
	};

	class SFXManager : public Actor
	{
	public:
		~SFXManager() {};
		void Init() override;
		void Update() override;
		void Destroy() override;
		void AddNewSFX(SFXType NewSFXType, const char* Path, std::string name);
		void Play(std::string name);
		std::map<SFXType, AEAudioGroup> sound_group;
		std::unordered_map<std::string, SFXData> SFXList;
	};
}