#include "PlayerCharacter.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include "../Manager/Playing.h"
#include <iostream>
#include <algorithm>
namespace InGame
{
	void PlayerCharacter::Init()
	{
		position.x = 0;
		position.y = 0;
		CollisionRadius = 5;
		size.x = 100;
		size.y = 100;
		MovementSpeed = 300;
		Mesh = Utils::CreateMesh();
		Texture = AEGfxTextureLoad("Assets/idle_right_down.png");
		HoldingGun = new Gun();
		HoldingGun->Init();
	}
	void PlayerCharacter::Update()
	{
		if(Health > 0)
		{
			AEVec2 MovingVec;
			AEVec2Set(&MovingVec, 0.f, 0.f);

			if (AEInputCheckCurr(AEVK_W))
			{
				MovingVec.y += 1.f;
			}
			if (AEInputCheckCurr(AEVK_S))
			{
				MovingVec.y -= 1.f;
			}
			if (AEInputCheckCurr(AEVK_A))
			{
				MovingVec.x -= 1.f;
			}
			if (AEInputCheckCurr(AEVK_D))
			{
				MovingVec.x += 1.f;
			}

			if(MovingVec.x != 0 && MovingVec.y != 0)
				AEVec2Normalize(&MovingVec, &MovingVec);
			AEVec2Scale(&MovingVec, &MovingVec, MovementSpeed * global::DeltaTime);

			position.x = std::clamp(position.x + MovingVec.x, global::worldMin.x + size.x / 2, global::worldMax.x - size.x / 2);
			position.y = std::clamp(position.y + MovingVec.y, global::worldMin.y + size.y / 2, global::worldMax.y - size.y / 2);

			global::PlayerLocation = position;
			GetMouseDir();
			if (HoldingGun)
			{
				HoldingGun->Update(MouseDirection, position, Level);
			}
		}
	}
	void PlayerCharacter::Draw()
	{
		Utils::DrawObject(*this);
		HoldingGun->Draw();
	}
	void PlayerCharacter::Destroy()
	{
		Utils::DestroyMesh(Mesh);
	}
	void PlayerCharacter::GetMouseDir()
	{
		s32 MX;
		s32 MY;
		AEInputGetCursorPosition(&MX, &MY);

		global::ScreenWidth;
		global::ScreenHeight;

		AEVec2 MP;
		MP.x = static_cast<float>(MX) - AEGfxGetWindowWidth() / 2.0f;
		MP.y = AEGfxGetWindowHeight() / 2.0f - static_cast<float>(MY);
		
		AEMtx33 translate_matrix;
		AEMtx33Inverse(&translate_matrix, &(Manager::CAM->translate_matrix));

		AEVec2 Result;
		AEMtx33MultVec(&Result, &translate_matrix, &MP);
		f32 length = AEVec2Distance(&position, &Result);
		f32 dx = Result.x - position.x;
		f32 dy = Result.y - position.y;
		MouseDirection.x = dx / length;
		MouseDirection.y = dy / length;
	}
	void PlayerCharacter::UpdateKill(u32 Exp)
	{
		KillCount+= Exp;
		if (KillCount >= TargetExp)
		{
			TargetExp *= 2;
			KillCount = 0;
			Level++;
			std::cout << Level << std::endl;
		}
	}
}