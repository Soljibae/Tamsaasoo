#include "PlayerCharacter.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
namespace InGame
{
	void PlayerCharacter::Init()
	{
		position.x = 0;
		position.y = 0;
		Mesh = Utils::CreateMesh();
	}
	void PlayerCharacter::Update()
	{
		if(Health > 0)
		{
			if(AEInputCheckCurr(AEVK_W))
			{
				position.y -= MovementSpeed * global::DeltaTime;
			}
			if(AEInputCheckCurr(AEVK_S))
			{
				position.y += MovementSpeed * global::DeltaTime;
			}
			if(AEInputCheckCurr(AEVK_A))
			{
				position.x -= MovementSpeed * global::DeltaTime;
			}
			if(AEInputCheckCurr(AEVK_D))
			{
				position.x += MovementSpeed * global::DeltaTime;
			}

			global::PlayerLocation = position;
		}
	}
	void PlayerCharacter::Draw()
	{
		
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

		f32 NDCX = (static_cast<f32>(MX) / static_cast<f32>(global::ScreenWidth)) * 2.0f - 1.0f;
		f32 NDCY = 1.0f - (static_cast<f32>(MY) / static_cast<f32>(global::ScreenHeight)) * 2.0f; // y inverse
		
	}
}