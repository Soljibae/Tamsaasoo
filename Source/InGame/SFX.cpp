#include "SFX.h"

namespace InGame
{
	void SFXManager::Init()
	{
		sound_group[SFX] = AEAudioCreateGroup();
		sound_group[BGM] = AEAudioCreateGroup();
		sound_group[UI] = AEAudioCreateGroup();
		AddNewSFX(SFX, "Assets/SFX/Attack/LaserWarning.wav", "LaserWarning");
		AddNewSFX(SFX, "Assets/SFX/Attack/LaserFire.wav", "LaserFire");
	}

	void SFXManager::Update()
	{
		for (auto it = SFXList.begin(); it != SFXList.end(); it++)
		{
			auto& sfx = *it;
			if (sfx.second.play)
			{
				switch(sfx.second.type)
				{
				case SFX:
					AEAudioPlay(sfx.second.audio, sound_group[sfx.second.type], SFXOriginVol, sfx.second.pitch, 0);
					break;
				case UI:
					AEAudioPlay(sfx.second.audio, sound_group[sfx.second.type], UIOriginVol, sfx.second.pitch, 0);
					break;
				case BGM:
					AEAudioPlay(sfx.second.audio, sound_group[sfx.second.type], BGMOriginVol, sfx.second.pitch, -1);
					break;
				}
				sfx.second.play = false;
			}
		}
	}

	void SFXManager::Destroy()
	{
		std::cout << __FUNCTION__ << std::endl;
		for (auto& sfx : SFXList)
		{
			AEAudioUnloadAudio(sfx.second.audio);
		}
		for (auto group : sound_group)
		{
			AEAudioUnloadAudioGroup(group.second);
		}
		SFXList.clear();
		std::unordered_map<std::string, SFXData>().swap(SFXList);
	}

	void SFXManager::AddNewSFX(SFXType NewSFXType, const char* Path, std::string name)
	{
		std::cout << __FUNCTION__ << std::endl;
		SFXData sfx;
		sfx.name = name;
		sfx.type = NewSFXType;
		if (sfx.type == BGM)
		{
			sfx.audio = AEAudioLoadMusic(Path);
			std::cout << __FUNCTION__ << __LINE__ << " " << Path << " " << name << " "  << sfx.audio.fmod_sound << std::endl;
		}
		else
		{
			sfx.audio = AEAudioLoadSound(Path);
			std::cout << __FUNCTION__ << __LINE__ << " " << Path << " " << name << " " << sfx.audio.fmod_sound << std::endl;
		}
		
		SFXList.insert({ sfx.name, sfx });
	}
	void SFXManager::Play(std::string name)
	{
		SFXList.find(name)->second.play = true;
	}
}