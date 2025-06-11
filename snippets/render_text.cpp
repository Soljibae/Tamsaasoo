//
// Text rendering demo. 
// 
// AUTHORS: 
//   Gerald Wong
//  
// DESCRIPTION:
//   The code below shows:
//   - How to render text
//   - How to perform different anchoring techniques to text rendering
//     to emulate text justification and vertical alignment.
//
//   Students should not take this code as a reference to
//   "good architecture". That's not the point of this demo.
//

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
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
    AESysSetWindowTitle("Text Rendering Demo!");

    // Load the font here 
    s8 pFont = AEGfxCreateFont("Assets/liberation-mono.ttf", 72.f);

    // Text to print
    const char* pText = "Hello World";

    f32 w, h;
    AEGfxGetPrintSize(pFont, pText, 1.f, &w, &h);

    // Game Loop
    while (gGameRunning)
    {
        AESysFrameStart();
        AEGfxSetBackgroundColor(0.1f, 0.1f, 0.1f);

        //
        // Text that is anchored to the top left hand of the window, as if it has:
        // - Left justification
        // - Top vertical alignment
        //
        AEGfxPrint(pFont, pText, -1.f, 1.f - h, 1, 1, 1, 1, 1); 

        //
        // Text that is drawn in the middle, as if it has:
        // - Center justification
        // - Center vertical alignment
        //
        AEGfxPrint(pFont, pText, -w/2, -h/2, 1, 1, 1, 1, 1);


        //
        // Text that is anchored to the top right of the window, as if it has:
        // - Right justification
        // - Top vertical alignment
        //
        AEGfxPrint(pFont, pText, 1.f-w, 1.f - h, 1, 1, 1, 1, 1); 

        //
        // Text that is anchored to the bottom left of the window, as if it has:
        // - Left justification
        // - Bottom vertical alignment
        //
        AEGfxPrint(pFont, pText, -1.f, -1.f, 1, 1, 1, 1, 1); 

        //
        // Text that is anchored to the bottom right of the window, as if it has:
        // - Right justification
        // - Bottom vertical alignment
        //
        AEGfxPrint(pFont, pText, 1.f-w, -1.f, 1, 1, 1, 1, 1); 


        // Informing the system about the loop's end
        AESysFrameEnd();


        // check if forcing the application to quit
        if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
            gGameRunning = 0;
    }

    AEGfxDestroyFont(pFont);

    AESysExit();
}



