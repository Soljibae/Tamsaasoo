#include "PlayerCharacter.h"

namespace InGame
{
	void PlayerCharacter::Init()
	{
		
	}
	void PlayerCharacter::Update()
	{
		if(Health > 0)
		{
			
			if(AEInputCheckCurr(AEVK_W))
			{
				posY -= MovementSpeed;
			}
			if(AEInputCheckCurr(AEVK_S))
			{
				posY += MovementSpeed;
			}
			if(AEInputCheckCurr(AEVK_A))
			{
				posX -= MovementSpeed;
			}
			if(AEInputCheckCurr(AEVK_D))
			{
				posX += MovementSpeed;
			}
		}
	}
	void PlayerCharacter::Draw()
	{
		
	}
	void PlayerCharacter::Destroy()
	{
		
	}
	void PlayerCharacter::GetMouseDir()
	{
		s32 MX;
		s32 MY;
		AEInputGetCursorPosition(&MX, &MY);
	}
}