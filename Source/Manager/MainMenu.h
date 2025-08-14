#pragma once
#include "GameState.h"
#include "../Global/GlobalVariables.h"
#include "../InGame/Actor.h"
#include "../Utils/Utils.h"
#include "Button.h"
#include <array>
namespace Manager
{
	class MainMenu : public AGameState
	{
	public:
		void Init() override;
		void Update() override;
		void Draw() override;
		void Destroy() override;
	private:
		InGame::Actor Illust;
		InGame::Actor Black;
		InGame::Actor Title;
		f32 fadeInTime{ 0 };
		const f32 fadeInDuration{ 1.f };
		static f32 alpha;
		f32 buttonAlpha{ 0.3f };
		InGame::Actor Wbutton;
		AEGfxVertexList* buttonMesh;
		AEGfxTexture* BbuttonTexture;
		std::array<Button, 3> Buttons;
		s8 pFont;
	};
	AEGfxVertexList* ButtonGlossing(f32 start, f32 end);
}