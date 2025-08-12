#pragma once
#include "AEEngine.h"
#include <iostream>
#include "../Global/GlobalVariables.h"
#include "GameState.h"
#include "../InGame/Actor.h"
#include "../InGame/SFX.h"
#include "../Utils/Utils.h"
namespace Manager
{
	class GameManager
	{
	public:
		void Init();
		void Update();
		void Pause();
		void Resume();
		void Draw();
		void Destroy();
		void SetNextGameState(EGameState state);
		
		AGameState* currStateREF;
		EGameState currState;
		EGameState nextState;
		bool GamePaused{ false };
		bool forceRestart{ false };
		bool shouldExit{ false };

		struct Cursor : public InGame::Actor
		{
			void Init() override
			{
				position = { 0,0 };
				size = { 30.f, 30.f };
				Mesh = Utils::CreateMesh();
				Texture = AEGfxTextureLoad("Assets/UI/cursor.png");
			}
			void Update() override
			{
				AEInputGetCursorPosition(&mouseX, &mouseY);
				position.x = MP.x = static_cast<f32>(mouseX) - AEGfxGetWindowWidth() / 2.0f;
				position.y = MP.y = AEGfxGetWindowHeight() / 2.0f - static_cast<float>(mouseY);
			}
			void Draw() override
			{
				Utils::DrawCursor(*this);
			}
			void Destroy() override
			{
				if (Mesh)
				{
					AEGfxMeshFree(Mesh);
					Mesh = nullptr;
				}
				if (Texture)
				{
					AEGfxTextureUnload(Texture);
					Texture = nullptr;
				}
			}
			s32 mouseX, mouseY;
			AEVec2 MP;
		};
	};
	extern GameManager gm;
	extern InGame::SFXManager SFXManager;
}