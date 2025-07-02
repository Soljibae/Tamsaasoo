#pragma once
#include "AEEngine.h"
#include "ArealAttack.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
namespace InGame
{
	void InGame::ArealAttack::Init()
	{
		Actor::Init();
		Mesh = Utils::CreateMesh();
		AEVec2Set(&size, 100.f, 100.f);
	}
	void ArealAttack::Update(std::vector<Character*>InCharacters)
	{
		Actor::Update();
		timer += global::DeltaTime;
		if (timer > lifeTime)
		{
			bIsPandingKill = true;
		}
	}
	void ArealAttack::Draw()
	{
		Actor::Draw();
	}
	void ArealAttack::Destroy()
	{
		Actor::Destroy();
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
		}
		if (Texture)
		{
			AEGfxTextureUnload(Texture);
		}
	}

	void BlackholeAttack::Init(AEVec2 SpawnLocation, AEVec2 DrawSize, f32 InRadius, f32 InlifeTime)
	{
		ArealAttack::Init();
		AEVec2Set(&position, SpawnLocation.x, SpawnLocation.y);
		AEVec2Set(&size, DrawSize.x, DrawSize.y);
		Texture = AEGfxTextureLoad("Assets/Blackhole.png");
		radius = InRadius;
		lifeTime = InlifeTime;
		timer = 0.f;
	}

	void BlackholeAttack::Update(std::vector<Character*>InCharacters)
	{
		ArealAttack::Update(InCharacters);
		timer += global::DeltaTime;
		for (Character* InCharacter : InCharacters)
		{
			if (timer < lifeTime)
			{
				AEVec2 toCenter;
				AEVec2Sub(&toCenter, &position, &global::PlayerLocation);
				float dist = AEVec2Length(&toCenter);
				if (dist < radius)
				{
					AEVec2 dir;
					AEVec2Normalize(&dir, &toCenter);

					AEVec2 pull;
					AEVec2Scale(&pull, &dir, pullStrength * global::DeltaTime);

					InCharacter->position.x += pull.x;
					InCharacter->position.y += pull.y;
				}
			}
		}
		if (timer > lifeTime)
		{
			bIsPandingKill = true;
		}
	}

	void BlackholeAttack::Draw()
	{
		float alpha = 0.8f;//0.4f + 0.2f * sinf(timer * 5.f);
		Utils::DrawObject(*this,true,alpha);
	}

	void BlackholeAttack::Destroy()
	{
		ArealAttack::Destroy();
	}

	void BombAttack::Init(AEVec2 SpawnLocation, f32 InRadius, f32 InLifeTime)
	{
		ArealAttack::Init();
		AEVec2Set(&position, SpawnLocation.x, SpawnLocation.y);
		radius = InRadius;
		lifeTime = InLifeTime;
		timer = 0.f;
		Texture = AEGfxTextureLoad("Assets/Bomb.png");
	}

	void BombAttack::Update(std::vector<Character*>InCharacters)
	{
		ArealAttack::Update(InCharacters);

		if (!bDidExplode && timer >= lifeTime)
		{
			// ∆¯πﬂ »ø∞˙
			//Utils::SpawnEffect(position, "Explosion");

			// «««ÿ √≥∏Æ
			for (Character* InCharacter : InCharacters)
			{
				AEVec2 toPlayer;
				AEVec2Sub(&toPlayer, &InCharacter->position, &position);
				if (AEVec2Length(&toPlayer) <= radius)
				{
					InCharacter->adjustHealth(-damage);
				}
			}

			bDidExplode = true;
		}
	}

	void BombAttack::Draw()
	{
		ArealAttack::Draw();
		Utils::DrawObject(*this);
	}

	void BombAttack::Destroy()
	{
		ArealAttack::Destroy();
	}

	void BurningAreaAttack::Init(AEVec2 SpawnLocation, f32 InRadius, f32 InLifeTime, float damageInterval)
	{
		ArealAttack::Init();
		position = SpawnLocation;
		radius = InRadius;
		lifeTime = InLifeTime;
		tickInterval = damageInterval;
		timer = 0.f;
		Texture = AEGfxTextureLoad("Assets/BurningArea.png");
	}

	void BurningAreaAttack::Update(std::vector<Character*>InCharacters)
	{
		ArealAttack::Update(InCharacters);

		tickTimer += global::DeltaTime;

		if (tickTimer >= tickInterval)
		{
			for (Character* InCharacter : InCharacters)
			{
				AEVec2 toPlayer;
				AEVec2Sub(&toPlayer, &global::PlayerLocation, &position);
				if (AEVec2Length(&toPlayer) <= radius)
				{
					InCharacter->adjustHealth(-damagePerTick);
				}
			}
			tickTimer = 0.f;
		}
	}

	void BurningAreaAttack::Draw()
	{
		float alpha = 0.3f + 0.2f * sinf(timer * 6.f); // ∫“≤… ±Ù∫˝¿”
		AEGfxSetRenderMode(AE_GFX_RM_COLOR);
		AEGfxSetBlendMode(AE_GFX_BM_BLEND);
		AEGfxSetTransparency(alpha);
		AEGfxTextureSet(Texture, 0, 0);
		AEGfxMeshDraw(Mesh, AE_GFX_MDM_TRIANGLES);
		AEGfxSetTransparency(1.0f);
	}

	void BurningAreaAttack::Destroy()
	{
		ArealAttack::Destroy();
	}

	void LaserAttack::Init(const AEVec2& InPivot, const AEVec2& dir, Actor* InOwner, float InLength, float InDuration, float InSpeed, bool bInRotRight)
	{
		row = 1;
		column = 1;
		Mesh = Utils::CreateMesh();
		Utils::InitOffset(*this);
		MaxAnimationCount[NONE] = 1;
		Pivot = InPivot;
		direction = dir;
		length = InLength;
		AEVec2Set(&position, Pivot.x + direction.x* InLength / 2, Pivot.y + direction.y * InLength / 2);
		speed = InSpeed;
		size = { length, 30.f }; // ∑π¿Ã¿˙ ±Ω±‚
		CollisionRadius = InLength * 0.5f;
		lifeTime = InDuration;
		bRotRight = bInRotRight;
		Texture = AEGfxTextureLoad("Assets/LaserTexture.png");

		Owner = InOwner;
	}

	void LaserAttack::Update(std::vector<Character*>InCharacters)
	{
		timer += global::DeltaTime;

		if (timer >= lifeTime)
		{
			bIsPandingKill = true;
			return;
		}
		if (Owner)
		{
			Pivot = Owner->position;
		}
		else
		{
			bIsPandingKill = true;
			return;
		}

		// πÊ«‚ »∏¿¸
		float angleRad = speed * global::DeltaTime * (bRotRight ? -1.f : 1.f);
		float cosA = cosf(angleRad);
		float sinA = sinf(angleRad);

		AEVec2 newDir;
		newDir.x = direction.x * cosA - direction.y * sinA;
		newDir.y = direction.x * sinA + direction.y * cosA;
		direction = newDir;

		position.x = Pivot.x + direction.x * length / 2;
		position.y = Pivot.y + direction.y * length / 2;

		Utils::UpdateOffset(*this);

		for (Character* InCharacter : InCharacters)
		{
			if (CheckCollision(InCharacter->position))
			{
				InCharacter->adjustHealth(-Damage);
			}
		}
	}

	void LaserAttack::Draw()
	{
		Utils::DrawObjectWithDirection(*this, 1.f);
	}

	void LaserAttack::Destroy()
	{
		ArealAttack::Destroy();
	}

	bool LaserAttack::CheckCollision(AEVec2& characterPos)
	{
		AEVec2 laserEnd;
		AEVec2Scale(&laserEnd, &direction, length);
		AEVec2Add(&laserEnd, &Pivot, &laserEnd);

		AEVec2 laserVec, toChar;
		AEVec2Sub(&laserVec, &laserEnd, &Pivot);
		AEVec2Sub(&toChar, &characterPos, &Pivot);

		float laserLen = AEVec2Length(&laserVec);
		AEVec2 laserDir;
		AEVec2Normalize(&laserDir, &laserVec);

		float projLen = AEVec2DotProduct(&toChar, &laserDir);

		if (projLen < 0 || projLen > laserLen)
			return false;

		AEVec2 projVec;
		AEVec2Scale(&projVec, &laserDir, projLen);
		AEVec2 closestPoint;
		AEVec2Add(&closestPoint, &Pivot, &projVec);

		AEVec2 diffVec;
		AEVec2Sub(&diffVec, &characterPos, &closestPoint);

		float distance = AEVec2Length(&diffVec);

		return distance <= (size.y * 0.5f);
	}
}
