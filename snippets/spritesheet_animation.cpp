//
// Spritesheet animation 
// 
// AUTHORS: 
//   Gerald Wong
//  
// DESCRIPTION:
//   The code below shows how to swap between different sprites
//   within a spritesheet over time, emulating sprite animation. 
//
//   Press [0] to decrease animation speed.
//   Press [1] to increase animation speed.
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
    AESysSetWindowTitle("Spritesheet Demo!");

    // (Not so good) hardcoded values about our spritesheet
    const u32 spritesheet_rows = 5;
    const u32 spritesheet_cols = 4;
    const u32 spritesheet_max_sprites = 19;
    const f32 sprite_uv_width = 1.f/spritesheet_cols;
    const f32 sprite_uv_height = 1.f/spritesheet_rows;

    // (Not so good) hardcoded values about our animation
    f32 animation_timer = 0.f;
    f32 animation_duration_per_frame = 0.1f;
    u32 current_sprite_index = 0; // start from first sprite
    f32 current_sprite_uv_offset_x = 0.f;
    f32 current_sprite_uv_offset_y = 0.f;


    // Pointer to Mesh
    AEGfxVertexList* pMesh = 0;

    // Informing the library that we're about to start adding triangles
    AEGfxMeshStart();

    // This shape has 2 triangles that makes up a square
    // Color parameters represent colours as ARGB
    // UV coordinates to read from loaded textures
    AEGfxTriAdd(
            -0.5f, -0.5f, 0xFFFFFFFF, 0.0f, sprite_uv_height, // bottom left
            0.5f, -0.5f, 0xFFFFFFFF, sprite_uv_width, sprite_uv_height, // bottom right 
            -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // top left

    AEGfxTriAdd(
            0.5f, -0.5f, 0xFFFFFFFF, sprite_uv_width, sprite_uv_height, // bottom right 
            0.5f, 0.5f, 0xFFFFFFFF, sprite_uv_width, 0.0f,   // top right
            -0.5f, 0.5f, 0xFFFFFFFF, 0.0f, 0.0f);  // bottom left


    // Saving the mesh (list of triangles) in pMesh
    pMesh = AEGfxMeshEnd();

    // Load texture
    AEGfxTexture* pTex = AEGfxTextureLoad("Assets/ame.png");


    // Game Loop
    while (gGameRunning)
    {

        // Informing the system about the loop's start
        AESysFrameStart();

        // Input that controls animation speed


        //
        // Update animation.
        // This is where all the animation magic happens.
        //

        // Update the animation timer.
        // animation_timer should go up to animation_duration_per_frame.
        animation_timer += (f32)AEFrameRateControllerGetFrameTime();

        if (animation_timer >= animation_duration_per_frame) { 
            // When the time is up go to the next sprite.

            // Reset the timer.
            animation_timer = 0;

            // Calculate the next sprite UV
            current_sprite_index = ++current_sprite_index % spritesheet_max_sprites; 

            u32 current_sprite_row = current_sprite_index / spritesheet_cols;
            u32 current_sprite_col = current_sprite_index % spritesheet_cols;

            current_sprite_uv_offset_x = sprite_uv_width * current_sprite_col ;
            current_sprite_uv_offset_y = sprite_uv_height * current_sprite_row;

        }



        // Your own rendering logic goes here
        // Set the background to black.
        AEGfxSetBackgroundColor(0.0f, 0.0f, 0.0f);

        // Tell the engine to get ready to draw something with texture.
        AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);

        // Set the the color to multiply to white, so that the sprite can 
        // display the full range of colors (default is black).
        AEGfxSetColorToMultiply(1.0f, 1.0f, 1.0f, 1.0f);
        AEGfxSetColorToAdd(0.0f, 0.0f, 0.0f, 0.0f);

        // Set blend mode to AE_GFX_BM_BLEND
        // This will allow transparency.
        AEGfxSetBlendMode(AE_GFX_BM_BLEND);
        AEGfxSetTransparency(1.0f);

        // Set the texture to pTex
        AEGfxTextureSet(pTex, current_sprite_uv_offset_x, current_sprite_uv_offset_y);

        AEMtx33 scale = { 0 };
        AEMtx33Scale(&scale, 800.f, 800.f);

        // Choose the transform to use
        AEGfxSetTransform(scale.m);

        // Actually drawing the mesh 
        AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

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
