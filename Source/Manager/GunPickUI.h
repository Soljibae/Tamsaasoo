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
		void Destroy();
		bool IsActive() const;
		std::array<InGame::GunType, 3> GenerateRandomGun();
		bool shouldShowStage = false;
	private:
		InGame::PlayerCharacter* PC;
		std::array<Button, 3> weaponOptionButtons;
		std::array<InGame::GunType, 3> weaponOptions;
		std::array<AEGfxVertexList*, 9> ButtonMesh;
		AEGfxTexture* ButtonTexture;
		std::array<InGame::Actor, 3> gunIcons;
		std::array<std::string, 3> GunNames;
		AEGfxVertexList* iconMesh;
		bool isActive{ false };
		s32 stageIdx;
		s8 pFont;
		struct Colors
		{
			f32 r, g, b;
		};
		std::array<Colors, 4> colors;
		std::map<InGame::GunType, std::string> gunDescriptions;
	};
	extern GunPickUI gunPickPanel;
}