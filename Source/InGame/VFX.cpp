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
	}
	void VFXManager::Init()
	{
		Actor::Init();
		Texture = AEGfxTextureLoad("Assets/VFX.png");
		Mesh = Utils::CreateMesh(2,9);
		MaxAnimationCount[Explosion] = 9;
		MaxAnimationCount[Burn] = 9;

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
			AEVec2Set(&position, DataItem->position.x, DataItem->position.y);
			AEVec2Set(&size, DataItem->size.x, DataItem->size.y);
			offset.x = DataItem->Animationoffset;
			offset.y = DataItem->Type;
			Utils::DrawObject(*this);
		}
	}
	void VFXManager::Destroy()
	{
		Actor::Destroy();
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
		}
		DrawItemList.push_back(NewData);
	}
	
}