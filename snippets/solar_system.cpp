//
// Solar System Demo
// 
// Written by: Gerald Wong
//  
// The code below to supplement the student's understanding on
// how render something on the screen in Alpha Engine by using
// meshs, textures, and transformations.
// 
// Students should not take this code as a reference to
// "good architecture". That's not the point of this demo.
// 

#include <crtdbg.h> // To check for memory leaks
#include "AEEngine.h"

// Planets can rotate around each other
typedef struct Planet {
	struct Planet* parent;	// Who this planet is parented to

	f32 scale;			// absolute scale from the parent
	f32 pos_x, pos_y;	// absolute position relative to parent

	f32 orbit;			// current orbit angle relative to parent
	f32 orbit_speed;	// orbit speed

	f32 spin;			// current spin angle
	f32 spin_speed;		// spin speed

	AEMtx33 transform;  // Final transformation matrix for rendering
} Planet;


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
	AESysSetWindowTitle("Spinning planet demo!");

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

	// Initialize planets
	Planet planets[3] = { 0 };
	planets[0].scale = 200.f;
	planets[0].spin_speed = 1.f;
	planets[0].orbit_speed = 0.f;
	planets[0].pos_x = planets[0].pos_y = 0.f;

	planets[1].parent = &planets[0];
	planets[1].scale = 100.f; // 
	planets[1].spin_speed = -1.f;
	planets[1].orbit_speed = -1.f;
	planets[1].pos_x = planets[1].pos_y = 200.f;

	planets[2].parent = &planets[1];
	planets[2].scale = 50.f;
	planets[2].spin_speed = 2.f;
	planets[2].orbit_speed = -2.f;
	planets[2].pos_x = planets[2].pos_y = 100.f;

	AEInputShowCursor(1);
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

		f32 dt = (f32)AEFrameRateControllerGetFrameTime();

		//
		// Update the variables of the planets
		//
		for (int i = 0; i < (sizeof(planets) / sizeof(*planets)); ++i)
		{
			planets[i].spin += planets[i].spin_speed * dt;
			planets[i].orbit += planets[i].orbit_speed * dt;
		}

		// Draw the planets
		for (int i = 0; i < (sizeof(planets) / sizeof(*planets)); ++i)
		{
			// Calculate the final transform
			AEMtx33 transform;
			AEMtx33Identity(&transform);

			AEMtx33 orbit;
			AEMtx33Rot(&orbit, planets[i].orbit);

			AEMtx33 spin;
			AEMtx33Rot(&spin, planets[i].spin);

			AEMtx33 scale;
			AEMtx33Scale(&scale, planets[i].scale, planets[i].scale);

			AEMtx33 translate;
			AEMtx33Trans(&translate, planets[i].pos_x, planets[i].pos_y);

			// Concatenate everything
			AEMtx33Concat(&transform, &spin, &scale);
			AEMtx33Concat(&transform, &translate, &transform);
			AEMtx33Concat(&transform, &orbit, &transform);

			//
			// Calculate the transform of the parent that contains
			// things the child (this planet) cares about so that
			// we can move the child to where the parent is.
			// 
			// We care about two things from the parent:
			// - The parent's position
			// - The parent's orbit angle
			// Of course, if that parent has a parent (grandparent),
			// we have to use their position and orbit angle as well.
			//
			// NOTE: This is clearly not the best way to do this. 
			// It is slow, but at least it's robust. Making it more 
			// efficient is left as an exercise to the reader :)
			// 
			AEMtx33 parent_transform;
			AEMtx33Identity(&parent_transform);

			for (Planet* p = planets[i].parent; p != 0; p = p->parent)
			{
				AEMtx33 current_parent_orbit;
				AEMtx33Identity(&current_parent_orbit);
				AEMtx33Rot(&current_parent_orbit, p->orbit);

				AEMtx33 current_parent_pos;
				AEMtx33Trans(&current_parent_pos, p->pos_x, p->pos_y);

				// Translate THEN rotate
				AEMtx33 current_parent_transform;
				AEMtx33Concat(&current_parent_transform, &current_parent_orbit, &current_parent_pos);
				AEMtx33Concat(&parent_transform, &current_parent_transform, &parent_transform);
			}

			// Putting everything together
			AEMtx33Concat(&transform, &parent_transform, &transform);


			// Choose the transform to use
			AEGfxSetTransform(transform.m);

			// Actually drawing the mesh 
			AEGfxMeshDraw(pMesh, AE_GFX_MDM_TRIANGLES);

		}



		// Informing the system about the loop's end
		AESysFrameEnd();


		// check if forcing the application to quit
		if (AEInputCheckTriggered(AEVK_ESCAPE) || 0 == AESysDoesWindowExist())
			gGameRunning = 0;

		if (AEInputCheckTriggered(AEVK_1))
			AESysSetFullScreen(1);
		if (AEInputCheckTriggered(AEVK_2))
			AESysSetFullScreen(0);
	}

	AEGfxMeshFree(pMesh);
	AEGfxTextureUnload(pTex);
	// free the system
	AESysExit();

}


