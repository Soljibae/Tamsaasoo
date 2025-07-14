#include "SFX.h"

namespace InGame
{
	void SFXManager::Init()
	{
		sound_group[SYSTEM] = AEAudioCreateGroup();
		sound_group[ENEMY] = AEAudioCreateGroup();
		sound_group[PLAYER] = AEAudioCreateGroup();
		sound_group[SFX] = AEAudioCreateGroup();
		sound_group[BGM] = AEAudioCreateGroup();
		sound_group[UI] = AEAudioCreateGroup();
		for (auto it = SFXList.begin(); it != SFXList.end(); it++)
		{
			auto& sfx = *it;
			if (sfx.second.type == BGM)
			{
				AEAudioPlay(sfx.second.audio, sound_group[sfx.second.type], BGMOriginVol, sfx.second.pitch, -1);
			}
		}
		AddNewSFX(SFX, "Assets/SFX/LaserWarning.mp3", "LaserWarning");
		AddNewSFX(SFX, "Assets/SFX/LaserFire.mp3", "LaserFire");
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
				case BGM:
					AEAudioPlay(sfx.second.audio, sound_group[sfx.second.type], BGMOriginVol, sfx.second.pitch, -1);
					break;
				case PLAYER:
					AEAudioPlay(sfx.second.audio, sound_group[sfx.second.type], PLAYEROriginVol, sfx.second.pitch, -1);
				}
				sfx.second.play = false;
			}
		}
	}

	void SFXManager::Destroy()
	{
		for (auto& sfx : SFXList)
		{
			AEAudioUnloadAudio(sfx.second.audio);
		}
		for (auto& group : sound_group)
		{
			AEAudioUnloadAudioGroup(group.second);
		}
		SFXList.clear();
		std::unordered_map<std::string, SFXData>().swap(SFXList);
	}

	void SFXManager::AddNewSFX(SFXType NewSFXType, const char* Path, std::string name)
	{
		SFXData sfx;
		sfx.name = name;
		sfx.type = NewSFXType;
		if (sfx.type == BGM)
		{
			sfx.audio = AEAudioLoadMusic(Path);
		}
		else
		{
			sfx.audio = AEAudioLoadSound(Path);
		}
		
		SFXList.insert({ sfx.name, sfx });
	}
	void SFXManager::Play(std::string name)
	{
		SFXList.find(name)->second.play = true;
	}
}