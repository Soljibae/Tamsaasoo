#pragma once
#include "Actor.h"
#include <list>
#include <map>
#define MaxFrameCount 9
namespace InGame
{
	enum VFXType
	{
		WarningCircle = 0,
		WarningSquare = 1,
		Explosion = 2,
		Burn = 3,
		Lazer = 4,
		BlackHole = 5,
	};
	struct VFXData
	{
	public:
		void Update(f32 DeltaTime);
		VFXType Type;
		AEVec2 size;
		AEVec2 position;
		AEVec2 Direction;
		AEVec2 Pivot;
		f32 Animationoffset;
		f32 Timer = 0.f;
		f32 MaxTimer = 0.f;
		Actor* Owner = nullptr;
	};

	class VFXManager : public Actor
	{
	public:
		virtual ~VFXManager() {};
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		void AddNewVFX(VFXType NewVFXType, AEVec2 InPosition, AEVec2 InSize, f32 Duration);
		void AddWarningVFX(VFXType NewVFXType, AEVec2 InPosition, AEVec2 InSize, AEVec2 InDirection, Actor* InOwner);
		std::map<VFXType, s32> MaxAnimationCount;
		std::list<VFXData*> DrawItemList;
	};
}
//a