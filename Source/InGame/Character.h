#pragma once
#include "Actor.h"
#include "Stat.h"
#include <vector>
namespace InGame
{
	struct AfterImage
	{
		AEVec2 position;
		AEVec2 size;
		f32 alpha;
		f32 timer;
	};
	class Character : public Actor
	{
	public:
		Stat* Stats;
		virtual ~Character() {};
		virtual void adjustHealth(f32 Amount);
		std::vector<AfterImage> afterImages;
		f32 afterImageCooldown = 0.05f;
		f32 afterImageTimer = 0.f;
	};
}
