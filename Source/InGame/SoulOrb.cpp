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
		size = { 12.f , 12.f };
		lifeTime = Utils::GetRandomFloat(0.f, 3.f);
		alpha = 0.1f;

		TargetPostion = { -(w / 2) + 100.f, h / 2 - 100.f };
		TargetPostion.x += Utils::GetRandomFloat(-10.f, 10.f);
		TargetPostion.y += Utils::GetRandomFloat(-10.f, 10.f);
		TargetSize = { 70.f, 70.f };

		startPos = position;
		endPos = TargetPostion;
		currentTime = 0.0f;
		totalTime = lifeTime;
		peakHeight = 200.0f;

		AEVec2Sub(&flatDir, &endPos, &startPos);
		AEVec2Normalize(&flatDir, &flatDir);

		float sign = Utils::GetRandomFloat(0.f, 1.f) < 0.5f ? -1.f : 1.f;
		curveDirection = sign * Utils::GetRandomFloat(0.5f, 1.0f);
	}
	void SoulOrb::Update()
	{
		currentTime += global::DeltaTime;
		if (currentTime > totalTime)
			currentTime = totalTime;

		float t = currentTime / totalTime; 

		AEVec2 linearPos;
		AEVec2Sub(&linearPos, &endPos, &startPos);
		AEVec2Scale(&linearPos, &linearPos, t);
		AEVec2Add(&linearPos, &startPos, &linearPos);

		float curveOffset = -4 * peakHeight * (t - 0.5f) * (t - 0.5f) + peakHeight;

		AEVec2 perp;
		perp.x = -flatDir.y;
		perp.y = flatDir.x;

		AEVec2 offset;
		AEVec2Scale(&offset, &perp, curveOffset * curveDirection);
		AEVec2Add(&position, &linearPos, &offset);

		if (t >= 1.0f && !bIsArrived)
		{
			bIsArrived = true;

			if (Manager::gm.currStateREF)
			{
				if (auto* GS = dynamic_cast<Manager::Playing*>(Manager::gm.currStateREF))
				{
					if (GS->PC)
					{
						GS->PC->PS->Potion += 1;
					}
				}
			}
		}

		if (!bIsArrived)
		{
			alpha = 0.2f + 0.6f * t;
		}
		else if (bIsArrived)
		{
			alpha -= 1.f / 0.5f * global::DeltaTime;
		}
		if (alpha <= 0)
		{
			bIsPandingKill = true;
		}

		//f32 distanceSq = AEVec2Distance(&position, &TargetPostion);
		//const f32 arrival_threshold = 4.0f; 

		//if (distanceSq < arrival_threshold * arrival_threshold && !bIsArrived)
		//{
		//	bIsArrived = true;
		//	if (Manager::gm.currStateREF)
		//	{
		//		Manager::Playing* GS = static_cast<Manager::Playing*>(Manager::gm.currStateREF);
		//		if (GS)
		//		{
		//			if (GS->PC)
		//			{
		//				GS->PC->PS->Potion += 1;
		//			}
		//		}
		//	}
		//}
		//else if(!(distanceSq < arrival_threshold * arrival_threshold))
		//{
		//	Speed = std::clamp(Speed * 1.05f, 0.f, maxSpeed);

		//	float moveDistance = Speed;

		//	AEVec2 displacement;
		//	AEVec2Scale(&displacement, &direction, moveDistance);

		//	AEVec2Add(&position, &position, &displacement);
		//}

		//if (bIsArrived)
		//{
		//	alpha -= 1.f / 0.5f * global::DeltaTime;
		//}

		//if (alpha <= 0)
		//{
		//	bIsPandingKill = true;
		//}
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