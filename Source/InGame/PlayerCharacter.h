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
	protected:
		AEVec2 MouseDirection;
		Gun* HoldingGun = nullptr;
	private:
	};
}