#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"
#include <iostream>
#include "GlobalVariables.h"
#include "../Manager/GameManager.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow)
{
    //_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetBreakAlloc(62193);
	//53030
	//53020
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    int gGameRunning = 1;

    // Using custom window procedure
    AESysInit(hInstance, nCmdShow, global::ScreenWidth, global::ScreenHeight, 0, 100, true, NULL);
	// Changing the window title
	AESysSetWindowTitle("HeavenRiser");
	global::font = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);
	AEInputShowCursor(0);

	//_CrtSetBreakAlloc(110390);
	
	Manager::gm.Init();
	AESysSetWindowIcon("Assets/UI/horn.ico", 16.f, 16.f);

	while (gGameRunning)
	{
		Manager::gm.Update();
		Manager::gm.Draw();
		if (0 == AESysDoesWindowExist()|| Manager::gm.shouldExit)
			gGameRunning = 0;
	}
	Manager::gm.Destroy();
	AESysExit();
}