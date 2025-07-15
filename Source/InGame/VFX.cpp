#include "VFX.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"
namespace InGame
{
	void VFXData::Update(f32 DeltaTime)
	{
		Timer += DeltaTime;
		s32 CurrentFrame = static_cast<int>((Timer / MaxTimer) * MaxFrameCount);
		Animationoffset = static_cast<float>(CurrentFrame) / MaxFrameCount;
		if (Type == VFXType::Laser || Type == VFXType::WarningSquare)
		{
			if (Owner)
			{
				if (Owner->bIsPandingKill)
				{
					Timer = MaxTimer + 1;
				}
				Pivot = Owner->position;
			}
			if (bIsTracking)
			{
				AEVec2 dirToPlayer;
				AEVec2Sub(&dirToPlayer, &global::PlayerLocation, &Pivot);
				AEVec2Normalize(&dirToPlayer, &dirToPlayer);
				Direction = dirToPlayer;
			}
			

			position.x = Pivot.x + Direction.x * size.x / 2;
			position.y = Pivot.y + Direction.y * size.x / 2;
		}
		else if (Type == VFXType::WarningCircle)
		{
			if (bIsTracking)
			{
				if (Owner)
				{
					position = Owner->position;
				}
			}
		}
	}
	void VFXManager::Init()
	{
		Actor::Init();
		Texture = AEGfxTextureLoad("Assets/VFX.png");
		Mesh = Utils::CreateMesh(6,9);
		MaxAnimationCount[WarningCircle] = 9;
		MaxAnimationCount[WarningSquare] = 9;
		MaxAnimationCount[Explosion] = 9;
		MaxAnimationCount[Burn] = 9;
		MaxAnimationCount[Laser] = 9;
		MaxAnimationCount[BlackHole] = 9;
	}
	void VFXManager::Update()
	{
		Actor::Update();
		for (VFXData* DataItem : DrawItemList)
		{
			DataItem->Update(global::DeltaTime);
		}
		for (auto it = DrawItemList.begin(); it != DrawItemList.end(); )
		{
			if (it == DrawItemList.end() || *it == nullptr)
				break;

			VFXData* Data = *it;
			if (Data->Timer>Data->MaxTimer)
			{
				delete Data;
				it = DrawItemList.erase(it);
			}
			else
			{
				++it;
			}
		}
	}
	void VFXManager::Draw()
	{
		Actor::Draw();
		for (VFXData* DataItem : DrawItemList)
		{
			if (DataItem->Type == VFXType::Laser || DataItem->Type == VFXType::WarningSquare)
			{
				AEVec2Set(&position, DataItem->position.x, DataItem->position.y);
				AEVec2Set(&size, DataItem->size.x, DataItem->size.y);
				offset.x = DataItem->Animationoffset;
				offset.y = DataItem->Type/6.f;
				direction = DataItem->Direction;
				Utils::DrawObjectWithDirection(*this, 1.f);
			}
			else
			{
				AEVec2Set(&position, DataItem->position.x, DataItem->position.y);
				AEVec2Set(&size, DataItem->size.x, DataItem->size.y);
				offset.x = DataItem->Animationoffset;
				offset.y = DataItem->Type / 6.f;
				Utils::DrawObject(*this);
			}
			
		}
	}
	void VFXManager::Destroy()
	{
		Actor::Destroy();
		for (VFXData* DrawItem : DrawItemList)
		{
			delete DrawItem;
		}
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
		}
		if (Texture)
		{
			AEGfxTextureUnload(Texture);
		}
	}
	void VFXManager::AddNewVFX(VFXType NewVFXType, AEVec2 InPosition, AEVec2 InSize, f32 Duration)
	{
		VFXData* NewData = new VFXData;
		NewData->Animationoffset = 0;
		NewData->Timer = 0;
		NewData->Type = NewVFXType;
		switch (NewVFXType)
		{
			case VFXType::Explosion:
			{
				NewData->MaxTimer = Duration;
				AEVec2Set(&NewData->size, InSize.x, InSize.y);
				AEVec2Set(&NewData->position, InPosition.x, InPosition.y);
				break;
			}
			case VFXType::Burn:
			{
				NewData->MaxTimer = Duration;
				AEVec2Set(&NewData->size, InSize.x, InSize.y);
				AEVec2Set(&NewData->position, InPosition.x, InPosition.y);
				break;
			}
			case VFXType::SoulOrbVfx:
			{
				NewData->MaxTimer = Duration;
				AEVec2Set(&NewData->size, InSize.x, InSize.y);
				AEVec2Set(&NewData->position, InPosition.x, InPosition.y);
				break;
			}

		}
		DrawItemList.push_back(NewData);
	}

	void VFXManager::AddWarningVFX(VFXType NewVFXType, AEVec2 InSize, f32 InDuration, AEVec2 InTargetPosition, bool InbIsTracking, Actor* InOwner)
	{
		VFXData* NewData = new VFXData;
		NewData->size = InSize;
		NewData->Animationoffset = 0;
		NewData->Timer = 0;
		NewData->MaxTimer = InDuration;
		NewData->Type = NewVFXType;
		NewData->Owner = InOwner;
		NewData->position = InTargetPosition;
		NewData->bIsTracking = InbIsTracking;
		DrawItemList.push_back(NewData);
	}

	void VFXManager::AddWarningVFX(VFXType NewVFXType, AEVec2 InPosition, AEVec2 InSize, AEVec2 InDirection, bool InbIsTracking, Actor* InOwner, float InTime)
	{
		VFXData* NewData = new VFXData;
		NewData->Animationoffset = 0;
		NewData->Timer = 0;
		NewData->MaxTimer = InTime;
		NewData->Type = NewVFXType;
		NewData->Direction = InDirection;
		NewData->Owner = InOwner;
		NewData->bIsTracking = InbIsTracking;
		switch (NewVFXType)
		{
			case VFXType::WarningCircle:
			{
				AEVec2Set(&NewData->size, InSize.x, InSize.y);
				AEVec2Set(&NewData->position, InPosition.x, InPosition.y);
				break;
			}
			case VFXType::WarningSquare:
			{
				AEVec2Set(&NewData->size, InSize.x, InSize.y);
				AEVec2Set(&NewData->position, InPosition.x, InPosition.y);
				break;
			}
		}
		DrawItemList.push_back(NewData);
	}
	
}