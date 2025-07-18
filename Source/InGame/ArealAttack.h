#pragma once
#include "AEEngine.h"
#include "Actor.h"
#include "character.h"
#include <vector>
namespace InGame
{
	class ArealAttack : public Actor
	{
	public:
		f32 radius = 0.f;
		f32 lifeTime = 0.f;
		f32 timer = 0.f;
		virtual void Init() override;
		virtual void Update(std::vector<Character*>InCharacters);
		virtual void Draw() override;
		virtual void Destroy() override;
	protected:
	private:

	};

	class BlackholeAttack : public ArealAttack
	{
	public:
		virtual void Init(AEVec2 SpawnLocation, AEVec2 DrawSize, f32 InRadius, f32 InlifeTime, Actor* InOwner, bool bInOwnerEffectable);
		virtual void Update(std::vector<Character*>InCharacters) override;
		virtual void Draw() override;
		virtual void Destroy() override;

		f32 pullStrength = 100.f;
		f32 damage = 3.f;
		Actor* Owner = nullptr;
		bool bOwnerEffectable = false;
	private:
	};

	class BombAttack : public ArealAttack
	{
	public:
		virtual void Init(AEVec2 SpawnLocation, f32 InRadius, f32 InlifeTime);
		virtual void Update(std::vector<Character*>InCharacters) override;
		virtual void Draw() override;
		virtual void Destroy() override;

		f32 damage = 4.f;

	private:
		bool bDidExplode = false;
	};

	class BurningAreaAttack : public ArealAttack
	{
	public:
		virtual void Init(AEVec2 SpawnLocation, f32 InRadius, f32 lifeTime, f32 damageInterval,Actor* InOwner, bool bInOwnerDamageAble);
		virtual void Update(std::vector<Character*>InCharacters) override;
		virtual void Draw() override;
		virtual void Destroy() override;

		f32 damagePerTick = 1.f;

	private:
		f32 tickTimer = 0.f;
		f32 tickInterval = 1.f;
		bool bOwnerDamageAble = true;
		Actor* Owner = nullptr;
	};

	class LaserAttack : public ArealAttack
	{
	public:
		void Init(const AEVec2& InPivot, const AEVec2& dir, Actor* InOwner, float InLength, float InDuration, float InSpeed, bool bInRotRight);
		virtual void Update(std::vector<Character*>InCharacters) override;
		virtual void Draw() override;
		virtual void Destroy() override;
		bool CheckCollision(AEVec2& characterPos);
	private:
		f32 Damage = 1.f;
		AEVec2 Pivot;
		f32 length;
		f32 timer = 0.f;
		f32 speed;
		Actor* Owner;
		bool bRotRight = true;
	};
}
