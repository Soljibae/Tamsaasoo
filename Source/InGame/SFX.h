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
		SFX = 0,
		BGM = 1,
		UI = 2,
		SFXType_LAST = UI
	};

	struct SFXData
	{
	public:
		SFXType type;
		AEAudio audio;
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
		f32 BGMOriginVol{ 1.0f };
		f32 BGMReduceVol{ 0.5f };
		f32 SFXOriginVol{ 1.0f };
		f32 SFXReduceVol{ 0.5f };
		f32 UIOriginVol{ 1.0f };
	};
}