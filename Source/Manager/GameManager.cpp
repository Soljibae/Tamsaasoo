#include "GameManager.h"
namespace manager
{
	GameManager gm;
	void GameManager::init()
	{
		gm.gs = temp;
	}
	void GameManager::update()
	{
		// Informing the system about the loop's start
		AESysFrameStart();
		std::cout << gm.gs;
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

		// Informing the system about the loop's end
		AESysFrameEnd();
	}
	void GameManager::draw()
	{

	}
}