//
// Playing Audio Demo
// 
// AUTHORS: 
//   Gerald Wong
// 
// DESCRIPTION: 
//   The code below demonstrates the how to play audio using
//   Alpha Engine's Audio component. 
// 
//   Students should not take this code as a reference to
//   "good architecture". That's not the point of this demo.
// 
// HOW TO USE:
//   Press '1' to decrease BGM volume.
//   Press '2' to increase BGM volume.
//   Press '3' to decrease BGM pitch.
//   Press '4' to increase BGM pitch.
//   Press 'Space' to play a sound effect 
//
// 

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

//
// Quick and hacky way to check for collision vs mouse
//
s32 is_mouse_on_area(s32 minx, s32 miny, s32 maxx, s32 maxy) {
    s32 x, y;
    AEInputGetCursorPosition(&x, &y);

    return x >= minx && x <= maxx && y >= miny && y <= maxy;
}

int APIENTRY wWinMain(
        _In_ HINSTANCE hInstance,
        _In_opt_ HINSTANCE hPrevInstance,
        _In_ LPWSTR    lpCmdLine,
        _In_ int       nCmdShow)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


    int gGameRunning = 1;

    // Initialization of your own variables go here

    // Using custom window procedure
    AESysInit(hInstance, nCmdShow, 1600, 900, 1, 60, true, NULL);

    // Changing the window title
    AESysSetWindowTitle("My New Demo!");

    //
    // Audio variables
    //
    f32 bgm_volume = 1.f;
    f32 bgm_pitch = 1.f;

    // Configure audio group
    AEAudio bouken = AEAudioLoadMusic("Assets/bouken.mp3");
    AEAudioGroup bgm = AEAudioCreateGroup(); // short for 'background music'
                                             
    AEAudioPlay(bouken, bgm, 1.f, 1.f, -1); // we can start playing here.

    // Configure sound effects
    AEAudio ore = AEAudioLoadSound("Assets/ore.mp3");
    AEAudioGroup se = AEAudioCreateGroup();   // short for 'sound effect'

    //
    // Font for demo purposes
    //
    s8 font = AEGfxCreateFont("Assets/liberation-mono.ttf", 72);

    // Game Loop
    while (gGameRunning)
    {
        AESysFrameStart();
        AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

        //
        // process input
        //

        if (AEInputCheckTriggered(AEVK_1)) {
            bgm_volume -= 0.1f;
            if (bgm_volume <= 0.f) 
                bgm_volume = 0.f;
            AEAudioSetGroupVolume(bgm, bgm_volume);
        }

        if (AEInputCheckTriggered(AEVK_2)) {
            bgm_volume += 0.1f;
            if (bgm_volume >= 2.f) 
                bgm_volume = 2.f;
            AEAudioSetGroupVolume(bgm, bgm_volume);
        }


        if (AEInputCheckTriggered(AEVK_3)) {
            bgm_pitch -= 0.1f;
            if (bgm_pitch <= 0.f) 
                bgm_pitch = 0.f;
            AEAudioSetGroupPitch(bgm, bgm_pitch);
        }

        if (AEInputCheckTriggered(AEVK_4)) {
            bgm_pitch += 0.1f;
            if (bgm_pitch >= 2.f) 
                bgm_pitch = 2.f;
            AEAudioSetGroupPitch(bgm, bgm_pitch);
        }

        if (AEInputCheckTriggered(AEVK_SPACE)) {
            AEAudioPlay(ore, se, 1.f, 1.f, 0); 
        }
        char buffer[255];


        AEGfxPrint(font, "BGM Volume:", -0.5f, 0.4f, 1.f, 1, 1, 1, 1);
        AEGfxPrint(font, "[1]", 0.1f, 0.4f, 1.f, 1, 1, 1, 1);
        sprintf_s(buffer, 255, "%.1f", bgm_volume);
        AEGfxPrint(font, buffer, 0.3f, 0.4f, 1.f, 1,1,1,1);
        AEGfxPrint(font, "[2]", 0.5f, 0.4f, 1.f, 1, 1, 1, 1);

        AEGfxPrint(font, "BGM Pitch:", -0.5f, 0.2f, 1.f, 1, 1, 1, 1);
        AEGfxPrint(font, "[3]", 0.1f, 0.2f, 1.f, 1, 1, 1, 1);
        sprintf_s(buffer, 255, "%.1f", bgm_pitch);
        AEGfxPrint(font, buffer, 0.3f, 0.2f, 1.f, 1,1,1,1);
        AEGfxPrint(font, "[4]", 0.5f, 0.2f, 1.f, 1, 1, 1, 1);

        AEGfxPrint(font, "Play Sound: ", -0.5f, -0.1f, 1.f, 1, 1, 1, 1);
        AEGfxPrint(font, "[Space]", 0.1f, -0.1f, 1.f, 1, 1, 1, 1);

        // Informing the system about the loop's end
        AESysFrameEnd();


        // check if forcing the application to quit
        if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
            gGameRunning = 0;
    }

    // Unload audio resources
    AEAudioUnloadAudio(bouken);
    AEAudioUnloadAudio(ore);
    AEAudioUnloadAudioGroup(bgm);
    AEAudioUnloadAudioGroup(se);

    // free the system
    AESysExit();

}
