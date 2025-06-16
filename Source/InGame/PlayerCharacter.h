#pragma once
#include "Character.h"
#include "Gun/Gun.h"
namespace InGame
{
	class PlayerCharacter : public Character
	{
	public:
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		void GetMouseDir();
		void UpdateKill(u32 Exp);
		u32 KillCount = 0;
		u32 TargetExp = 1;
		u32 Level = 1;
	protected:
		AEVec2 MouseDirection;
		Gun* HoldingGun = nullptr;
	private:
	};
}