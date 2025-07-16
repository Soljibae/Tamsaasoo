#pragma once
#include "GameState.h"
#include "../InGame/Actor.h"
#include "Button.h"
#include <array>
namespace Manager
{
	class GameOver
	{
	public:
		void Init();
		void Update();
		void Draw();
		void Destroy();
		bool isGameOver = false;
	private:
		InGame::Actor BG;
		f32 BGalpha;

		f32 buttonAlpha{ 0.3f };
		InGame::Actor Wbutton;
		AEGfxVertexList* buttonMesh;
		AEGfxTexture* BbuttonTexture;
		std::array<Button, 2> Buttons;

		f32 fadeoutTime;
		f32 fadeoutDuration;
		f32 buttonShowDelay;
		s8 pFont;
	};
	extern GameOver gameOverScreen;
}