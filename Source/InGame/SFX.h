#pragma once
#include "Actor.h"
namespace InGame
{
	enum SFXType
	{
		System = 0,
		Enemy = 1,
		Player = 2,
		Sfx = 3,
		Bgm = 4,
		Ui = 5,
		Last = Ui
	};
	struct SFXData
	{
	public:
		void Update(f32 DeltaTime);
		SFXType Type;
	};

	class SFXManager : public Actor
	{
	public:
		~SFXManager() {};
		void Init() override;
		void Update() override;
		void Draw() override;
		void Destroy() override;
		void AddNewSFX(SFXType NewSFXType);
		//std::map<VFXType, s32> MaxAnimationCount;
		//std::list<VFXData*> DrawItemList;
	};
}