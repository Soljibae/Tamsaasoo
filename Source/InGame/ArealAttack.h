#pragma once
#include "AEEngine.h"
#include "Actor.h"
namespace InGame
{
	class ArealAttack : public Actor
	{
	public:
		AEVec2 position;
		float radius = 0.f;
		float lifeTime = 0.f;
		float timer = 0.f;
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
	protected:
	private:

	};
}
