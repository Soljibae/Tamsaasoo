#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "GlobalVariables.h"
#include "../Manager/GameManager.h"
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int gGameRunning = 1;

    // Using custom window procedure
    AESysInit(hInstance, nCmdShow, global::ScreenWidth, global::ScreenHeight, 1, 60, true, NULL);
	// Changing the window title
	AESysSetWindowTitle("HeavenRiser");
	global::font = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);
	AEInputShowCursor(1);

	Manager::gm.Init();
	while (gGameRunning)
	{
		Manager::gm.Update();
		Manager::gm.Draw();
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;
	}
	Manager::gm.Destroy();

	AESysExit();
}