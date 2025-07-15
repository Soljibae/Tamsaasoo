#include "BossAppearScene.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include "GameManager.h"
namespace Manager
{
	static f32 w = global::ScreenWidth;
	static f32 h = global::ScreenHeight;
	static enum LetterboxPos { UP, DOWN };
	void BossAppearScene::Init(InGame::EnemyBoss* InBoss)
	{
		Boss = InBoss;
		updateFrame = 0;
		for (s8 i = 0; i < Letterboxs.size(); i++)
		{
			Letterboxs[i].size = { w, 100.f };
			Letterboxs[i].position.x = 0.f;
			switch (i)
			{
			case UP:
				Letterboxs[i].position.y = (h/2.f) + (Letterboxs[i].size.y / 2.f);
				break;
			case DOWN:
				Letterboxs[i].position.y = -(h/2.f) - (Letterboxs[i].size.y / 2.f);
				break;
			}
			Letterboxs[i].Mesh = Utils::CreateMesh();
			Letterboxs[i].Texture = AEGfxTextureLoad("Assets/black.png");
		}
		AEAudioSetGroupVolume(SFXManager.sound_group[InGame::BGM], SFXManager.BGMReduceVol);
		AEAudioSetGroupVolume(SFXManager.sound_group[InGame::SFX], SFXManager.SFXReduceVol);
	}
	void BossAppearScene::Update()
	{
		if (updateFrame < 3)
		{
			updateFrame++;
			return;
		}
		Boss->bossApearTime += global::DeltaTime;
		if (Boss->bossApearTime < Boss->bossApearDuration - 0.5f)
		{
			for (s8 i = 0; i < Letterboxs.size(); i++)
			{
				switch (i)
				{
				case UP:
					if (Letterboxs[i].position.y > h / 2.f - Letterboxs[i].size.y / 2.f)
						Letterboxs[i].position.y -= global::DeltaTime * 100.f;
					else if (Letterboxs[i].position.y < h / 2.f + Letterboxs[i].size.y / 2.f)
					{
						Letterboxs[i].position.y = h / 2.f - Letterboxs[i].size.y / 2.f;
					}
					break;
				case DOWN:
					if (Letterboxs[i].position.y < -h / 2.f + Letterboxs[i].size.y / 2.f)
						Letterboxs[i].position.y += global::DeltaTime * 100.f;
					else if (Letterboxs[i].position.y > -h / 2.f + Letterboxs[i].size.y / 2.f)
					{
						Letterboxs[i].position.y = -h / 2.f + Letterboxs[i].size.y / 2.f;
					}
					break;
				}
			}
		}
		else
		{
			for (s8 i = 0; i < Letterboxs.size(); i++)
			{
				switch (i)
				{
				case UP:
					Letterboxs[i].position.y += global::DeltaTime * 200.f;
					break;
				case DOWN:
					Letterboxs[i].position.y -= global::DeltaTime * 200.f;
					break;
				}
			}
		}
		if (Boss->bossApearTime > Boss->bossApearDuration)
		{
			Boss->bossApearing = false;
			AEAudioSetGroupVolume(SFXManager.sound_group[InGame::BGM], SFXManager.BGMOriginVol);
			AEAudioSetGroupVolume(SFXManager.sound_group[InGame::SFX], SFXManager.SFXOriginVol);
		}
	}
	void BossAppearScene::Draw()
	{
		for (s8 i = 0; i < Letterboxs.size(); i++)
		{
			Utils::DrawObject(Letterboxs[i], false);
		}
	}
	void BossAppearScene::Destroy()
	{
		for (s8 i = 0; i < Letterboxs.size(); i++)
		{
			AEGfxMeshFree(Letterboxs[i].Mesh);
			AEGfxTextureUnload(Letterboxs[i].Texture);
		}
	}
}