#pragma once
#include "Actor.h"
#include "EnemyCharacter.h"

namespace InGame
{
	class SoulOrb : public Actor
	{
	public:
		void Init(InGame::EnemyCharacter* target);
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
		static void StaticInit();
		static void StaticDestroy();

		AEVec2 TargetPostion;
		AEVec2 TargetSize;
		f32 lifeTime;
		f32 distance;
		AEVec2 direction;
		bool bIsPandingKill;
		bool bIsArrived;
		f32 maxSpeed;
		f32 Speed;
		f32 alpha;

		static AEGfxTexture* SoulOrbTexture;
		static AEGfxVertexList* SoulOrbMesh;

		AEVec2 startPos, endPos;
		AEVec2 flatDir;
		float currentTime;
		float totalTime;
		float peakHeight;
		float curveDirection;
	};
}
