//
// Multiple sprite rendering demo
// 
// AUTHORS: 
//   Gerald Wong
//  
// DESCRIPTION:
//   The code below show cases how you can draw  
//   multiple sprites by reusing a single mesh. 
//
//   Students should not take this code as a reference to
//   "good architecture". That's not the point of this demo.
// 
#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

// Useful macro to count number of items in an array
#define array_count(a) (sizeof(a)/sizeof(*a))

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
    AESysSetWindowTitle("Draw Multiple Sprite Demo!");

    // Pointer to Mesh. All our sprites use the same mesh, so we can just have one.
    AEGfxVertexList* pMesh = 0;

    // Informing the library that we're about to start adding triangles
    AEGfxMeshStart();

    // This shape has 2 triangles that makes up a square
    // Color parameters represent colours as ARGB
    // UV coordinates to read from loaded textures
    AEGfxTriAdd(
            -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, 1.0f,
            0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
            -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    AEGfxTriAdd(
            0.5f, -0.5f, 0xFFFFFFFF, 1.0f, 1.0f,
            0.5f, 0.5f, 0xFFFFFFFF, 1.0f, 0.0f,
            -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);

    // Saving the mesh (list of triangles) in pMesh
    pMesh = AEGfxMeshEnd();
    AEGfxTexture* pTex = AEGfxTextureLoad("Assets/PlanetTexture.png");


    // Create a variable to store the transforms of our sprites
    AEMtx33 transforms[3] = { 0 };

    //
    // Create transform for our first sprite,
    // which will simply be scaled 500 to the x-axis
    // and 100 to the y-axis. 
    //
    // This should result in an oval planet :)
    //
    {
        AEMtx33Scale(&transforms[0], 500.f, 100.f);
    }

    // Create transform for our second sprite, 
    // which will be rotated 45 degrees, 
    // 100 pixel width and height,
    // translated 100 in the x axis and 200 in the y axis. 
    {
        AEMtx33 scale;
        AEMtx33Scale(&scale, 100.f, 100.f);

        AEMtx33 tran;
        AEMtx33Trans(&tran, 100.f, 200.f);

        AEMtx33 rotate;
        AEMtx33Rot(&rotate, PI/4);

        AEMtx33Concat(&transforms[1], &rotate, &scale);
        AEMtx33Concat(&transforms[1], &tran, &transforms[1]);
    }


    // Create transform for our third sprite
    // which will be scaled 200 by 200,
    // and translated 400 to the left
    { 
        AEMtx33 scale;
        AEMtx33Scale(&scale, 200.f, 200.f);

        AEMtx33 tran;
        AEMtx33Trans(&tran, -400.f, 0.f);

        AEMtx33Concat(&transforms[2], &tran, &scale);

    }

    // Game Loop
    while (gGameRunning)
    {
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


        // For each transform in the array...
        for( int i = 0; i < array_count(transforms); ++i) {
            // Tell Alpha Engine to use the matrix in 'transform' to apply onto all
            // the vertices of the mesh that we are about to choose to draw in the next line.
            AEGfxSetTransform(transforms[i].m);

            // Tell Alpha Engine to draw the mesh with the above settings.
            AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

        }


        
        // Informing the system about the loop's end
        AESysFrameEnd();


        // check if forcing the application to quit
        if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
            gGameRunning = 0;
    }

    AEGfxMeshFree(pMesh);
    AEGfxTextureUnload(pTex);
    // free the system
    AESysExit();

}
