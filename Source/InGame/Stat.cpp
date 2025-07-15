#include "Stat.h"
namespace InGame
{
	void Stat::Init()
	{
		StatusEffectTimer[BURN] = 0.f;
		StatusEffectTimer[STUN] = 0.f;
		StatusEffectTimer[SLOW] = 0.f;
		StatusEffectTimer[FEAR] = 0.f;
		StatusEffectTimer[VULNERABLE] = 0.f;
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
