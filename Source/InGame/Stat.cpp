#include "Stat.h"
namespace InGame
{
	void Stat::Init()
	{
		StatusEffectTimer[BURN] = true;
		StatusEffectTimer[STUN] = true;
		StatusEffectTimer[SLOW] = true;
		StatusEffectTimer[FEAR] = true;
		StatusEffectTimer[VULNERABLE] = true;
	}
	void Stat::Update()
	{

	}
	void Stat::Draw()
	{

	}
	void Stat::Destroy()
	{

	}
}
