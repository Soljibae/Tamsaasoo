#include "PlayerCharacter.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include "../Manager/Playing.h"
#include <iostream>
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
			if(AEInputCheckCurr(AEVK_W))
			{
				position.y += MovementSpeed * global::DeltaTime;
				if (position.y > global::worldMax.y - size.y/2)
				{
					position.y = global::worldMax.y - size.y/2;
				}
			}
			if(AEInputCheckCurr(AEVK_S))
			{
				position.y -= MovementSpeed * global::DeltaTime;
				if (position.y < global::worldMin.y + size.y / 2)
				{
					position.y = global::worldMin.y + size.y / 2;
				}
			}
			if(AEInputCheckCurr(AEVK_A))
			{
				position.x -= MovementSpeed * global::DeltaTime;
				if (position.x < global::worldMin.x + size.x / 2)
				{
					position.x = global::worldMin.x + size.x / 2;
				}
			}
			if(AEInputCheckCurr(AEVK_D))
			{
				position.x += MovementSpeed * global::DeltaTime;
				if (position.x > global::worldMax.x - size.x / 2)
				{
					position.x = global::worldMax.x - size.x / 2;
				}
			}

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