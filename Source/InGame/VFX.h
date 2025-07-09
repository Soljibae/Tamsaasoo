#pragma once
#include "Actor.h"
#include <list>
#include <map>
#define MaxFrameCount 9
namespace InGame
{
	enum VFXType
	{
		Explosion = 0,
		WarningExplosion = 1,
		Burn = 2,
		WarningBurn = 3,
		Lazer = 4,
		WarningLazer = 5,
	};
	struct VFXData
	{
	public:
		void Update(f32 DeltaTime);
		VFXType Type;
		AEVec2 size;
		AEVec2 position;
		f32 Animationoffset;
		f32 Timer = 0.f;
		f32 MaxTimer = 0.f;
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
		std::map<VFXType, s32> MaxAnimationCount;
		std::list<VFXData*> DrawItemList;
	};
}
//a