#pragma once
#include "Character.h"
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
		f32 MouseDirX = 0.f;
		f32 MouseDirY = 0.f;
	private:
	};
}