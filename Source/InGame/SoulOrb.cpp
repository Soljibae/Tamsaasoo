#include "SoulOrb.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
#include "../Manager/Playing.h"
#include "../Manager/GameManager.h"
#include <algorithm>

namespace InGame
{
	AEGfxTexture* SoulOrb::SoulOrbTexture;
	AEGfxVertexList* SoulOrb::SoulOrbMesh;

	void SoulOrb::Init(InGame::EnemyCharacter* target)
	{
		f32 w = static_cast<f32>(global::ScreenWidth);
		f32 h = static_cast<f32>(global::ScreenHeight);
		bIsPandingKill = false;
		bIsArrived = false;

		AEMtx33MultVec(&position, &(Manager::CAM->translate_matrix), &target->position);
		size = { 32.f , 32.f };
		lifeTime = 3.f;
		TargetPostion = { -(w / 2) + 100.f, h / 2 - 100.f };
		TargetPostion.x += Utils::GetRandomFloat(-10.f, 10.f);
		TargetPostion.y += Utils::GetRandomFloat(-10.f, 10.f);
		TargetSize = { 70.f, 70.f };
		AEVec2Sub(&direction, &TargetPostion, &position);
		AEVec2Normalize(&direction, &direction);
		//distance = AEVec2Distance(&TargetPostion, &position);
		distance = (w + h) / 2.f;
		Speed = distance / lifeTime * global::DeltaTime;
		maxSpeed = distance / lifeTime * global::DeltaTime * 2;
		alpha = 0.4f;
		Manager::SFXManager.AddNewSFX(SFX, "Assets/SFX/orb.wav", "orb");
	}
	void SoulOrb::Update()
	{
		
		f32 distanceSq = AEVec2Distance(&position, &TargetPostion);
		const f32 arrival_threshold = 4.0f; 

		if (distanceSq < arrival_threshold * arrival_threshold && !bIsArrived)
		{
			bIsArrived = true;
			if (Manager::gm.currStateREF)
			{
				Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
				if (GS)
				{
					if (GS->PC)
					{
						GS->PC->PS->Potion += 1;
						Manager::SFXManager.Play("orb");
					}
				}
			}
		}
		else if(!(distanceSq < arrival_threshold * arrival_threshold))
		{
			Speed = std::clamp(Speed * 1.05f, 0.f, maxSpeed);

			float moveDistance = Speed;

			AEVec2 displacement;
			AEVec2Scale(&displacement, &direction, moveDistance);

			AEVec2Add(&position, &position, &displacement);
		}

		if (bIsArrived)
		{
			alpha -= 1.f / 0.5f * global::DeltaTime;
		}

		if (alpha <= 0)
		{
			bIsPandingKill = true;
		}
	}
	void SoulOrb::Draw()
	{
		Utils::DrawObject(*this, SoulOrbTexture, SoulOrbMesh, alpha);
	}
	void SoulOrb::Destroy()
	{
		
	}
	void SoulOrb::StaticInit()
	{
		SoulOrbTexture = AEGfxTextureLoad("Assets/SoulOrb.png");
		SoulOrbMesh = Utils::CreateMesh();
	}
	void SoulOrb::StaticDestroy()
	{
		if (SoulOrbTexture)
		{
			AEGfxTextureUnload(SoulOrbTexture);
			SoulOrbTexture = nullptr;
		}

		Utils::DestroyMesh(SoulOrbMesh);
	}
}