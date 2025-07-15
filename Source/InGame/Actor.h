#pragma once
#include "AEEngine.h"
#include <map>
namespace InGame
{
	enum EAnimationState
	{
		NONE,
		IDLE,
		MOVE,
		DEAD,
		JUMP,
		DASH,
		ATTACK,
	};
	class Actor
	{
	public:
		virtual ~Actor() {};
		virtual void Init();
		virtual void Update();
		virtual void Draw();
		virtual void Destroy();
		AEGfxVertexList* Mesh = nullptr;
		AEGfxTexture* Texture = nullptr;
		AEVec2 size;
		AEVec2 position;
		f32 CollisionRadius = 0.f;
		AEVec2 direction;
		f32 MovementSpeed = 0.f; 
		s32 row = 0;
		s32 column = 0;
		f32 FrameTime = 0.f;
		f32 TimeAcc = 0.f;
		f32 Alpha = 1.f;
		AEVec2 offset;
		bool bIsPandingKill = false;
		EAnimationState AnimationState = EAnimationState::IDLE;
		s32 AnimationCount = 0;
		std::map<EAnimationState, s32> MaxAnimationCount;
	};
}
