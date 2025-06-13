#include "Character.h"
namespace InGame
{
	void Character::adjustHealth(s32 Amount)
	{
		Health += Amount;
		if (Health <= 0)
		{
			bIsPandingKill = true;
		}
	}
}