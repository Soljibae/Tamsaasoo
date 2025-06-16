#pragma once
#include "AEEngine.h"
#include "Stat.h"

namespace InGame
{
	class PlayerStat : public Stat
	{
	public:
		virtual void Init();
		virtual void Update();
		virtual void Draw();
		virtual void Destroy();

		s32 Damage;
		u32 Level;
		f32 ExpGained;
		s32 PiercingCnt;
	};
}
