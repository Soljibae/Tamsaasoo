#include "GameManager.h"
namespace manager
{
	GameManager gm;

	void GameManager::init()
	{
		gm.gs = GameState::Intro;

	}

	// Pointer to Mesh. All our sprites use the same mesh, so we can just have one.
	AEGfxTexture* pTex = AEGfxTextureLoad("Assets/DigiPen_WHITE.png");
	void GameManager::update()
	{
		switch (gs)
		{
		case GameState::Intro:
			break;
		case GameState::MainMenu:
			break;
		}

		// Informing the system about the loop's start
		AESysFrameStart();

		// Your own rendering logic goes here
		// Set the background to black.
		AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

		// Tell the engine to get ready to draw something with texture.
		AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

		// Set the the color to multiply to white, so that the sprite can 
		// display the full range of colors (default is black).
		AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);

		// Set the color to add to nothing, so that we don't alter the sprite's color
		AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

		// Set blend mode to AE_GFX_BM_BLEND
		// This will allow transparency.
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(1.0f);

		// Set the texture to pTex
		AEGfxTextureSet(pTex, 0, 0);
		global::DeltaTime = (f32)AEFrameRateControllerGetFrameTime();
		// Informing the system about the loop's end
		AESysFrameEnd();
	}

	void GameManager::draw()
	{

	}
	
	void GameManager::exit()
	{
		AEGfxTextureUnload(pTex);
	}
}