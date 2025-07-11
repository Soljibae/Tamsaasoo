#pragma once
#include "Button.h"
#include "../InGame/Actor.h"
#include "../InGame/PlayerCharacter.h"
#include <array>
namespace Manager
{
	class GunPickUI
	{
	public:
		void Init(InGame::PlayerCharacter* InPC);
		void Update();
		void Draw();
		void Show();
		void Destory();
		bool IsActive() const;
		std::array<InGame::GunType, 3> GenerateRandomGun();

	private:
		InGame::PlayerCharacter* PC;
		std::array<Button, 3> weaponOptionButtons;
		std::array<InGame::GunType, 3> weaponOptions;
		std::array<AEGfxVertexList*, 9> ButtonMesh;
		AEGfxTexture* ButtonTexture;
		std::array<InGame::Actor, 3> gunIcons;
		AEGfxVertexList* iconMesh;
		bool isActive{ false };
	};
	extern GunPickUI gunPickPanel;
}