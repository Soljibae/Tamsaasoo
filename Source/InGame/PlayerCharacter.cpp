#include "PlayerCharacter.h"

namespace InGame
{
	void PlayerCharacter::Init()
	{
		position.x = 0;
		position.y = 0;
		CollisionRadius = 50;
		size.x = 100;
		size.y = 100;
		MovementSpeed = 300;
		row = 4;
		column = 2;
		Mesh = Utils::CreateMesh(row, column);
		FrameTime = 1.f;
		TimeAcc = 0.f;
		AnimationState = IDLE;
		AnimationCount = 0;
		MaxAnimationCount[IDLE] = 2;
		MaxAnimationCount[MOVE] = 2;

		Stats.HP = 100;
		Stats.MovementSpeed = MovementSpeed;
		Stats.FireRate = 10.0f;
		Stats.BulletSpeed = 30.f;
		Stats.Damage = 1;
		Stats.Level = 1;
		Stats.ExpGained = 1.f;
		Stats.HitCount = 1;
		Stats.ExpCount = 0.f;
		Stats.TargetExp = 1.f;
		
		Utils::InitOffset(*this);

		Texture = AEGfxTextureLoad("Assets/Character.png");
		HoldingGun = new Gun();
		HoldingGun->Init(this);

		inventory.clear();
	}
	void PlayerCharacter::Update()
	{
		if(Stats.HP > 0)
		{
			AEVec2 MovingVec;
			AEVec2Set(&MovingVec, 0.f, 0.f);

			if (AEInputCheckCurr(AEVK_W))
			{
				MovingVec.y += 1.f;
			}
			if (AEInputCheckCurr(AEVK_S))
			{
				MovingVec.y -= 1.f;
			}
			if (AEInputCheckCurr(AEVK_A))
			{
				MovingVec.x -= 1.f;
				if (size.x > 0)
				{
					size.x *= -1;
				}
			}
			if (AEInputCheckCurr(AEVK_D))
			{
				MovingVec.x += 1.f;
				if (size.x < 0)
				{
					size.x *= -1;
				}
			}

			if (MovingVec.x == 0 && MovingVec.y == 0)
			{
				if (AnimationState == MOVE)
				{
					AnimationCount = 0;
				}
				AnimationState = IDLE;
			}
			else
			{
				if (AnimationState == IDLE)
				{
					AnimationCount = 0;
				}
				AnimationState = MOVE;
			}

			if(MovingVec.x != 0 && MovingVec.y != 0)
				AEVec2Normalize(&MovingVec, &MovingVec);
			AEVec2Scale(&MovingVec, &MovingVec, Stats.MovementSpeed * global::DeltaTime);

			position.x = std::clamp(position.x + MovingVec.x, global::worldMin.x + abs(size.x) / 2, global::worldMax.x - abs(size.x) / 2);
			position.y = std::clamp(position.y + MovingVec.y, global::worldMin.y + size.y / 2, global::worldMax.y - size.y / 2);

			global::PlayerLocation = position;

			Utils::UpdateOffset(*this);

			GetMouseDir();

			global::PlayerMouseDirection = MouseDirection;

			if (HoldingGun)
			{
				HoldingGun->Update(MouseDirection, position);
			}
			if (bIsInvincible)
			{
				InvincibleTimer += global::DeltaTime;
				if (InvincibleTimer > 0.5f)
				{
					bIsInvincible = false;
					InvincibleTimer = 0.f;
				}
			}

			for (const auto& item_ptr : inventory)
			{
				item_ptr.first->Use(this);
			}
		}
	}
	void PlayerCharacter::Draw()
	{
		if (bIsInvincible)
		{
			Utils::DrawObject(*this, true, 0.5f);
		}
		else
		{
			Utils::DrawObject(*this);
		}
		HoldingGun->Draw();
	}
	void PlayerCharacter::Destroy()
	{
		if (Mesh)
		{
			Utils::DestroyMesh(Mesh);
		}
		if (Texture)
		{
			AEGfxTextureUnload(Texture);
		}
		if (HoldingGun)
		{
			HoldingGun->Destroy();
			delete HoldingGun;
			HoldingGun = nullptr;
		}
	}
	void PlayerCharacter::adjustHealth(s32 Amount)
	{
		if (!bIsInvincible)
		{
			Stats.HP += Amount;
			if (Stats.HP <= 0)
			{
				bIsPandingKill = true;
			}
			else
			{
				bIsInvincible = true;
			}
		}
	}
	void PlayerCharacter::GetMouseDir()
	{
		s32 MX;
		s32 MY;
		AEInputGetCursorPosition(&MX, &MY);

		global::ScreenWidth;
		global::ScreenHeight;

		AEVec2 MP;
		MP.x = static_cast<float>(MX) - AEGfxGetWindowWidth() / 2.0f;
		MP.y = AEGfxGetWindowHeight() / 2.0f - static_cast<float>(MY);
		
		AEMtx33 translate_matrix;
		AEMtx33Inverse(&translate_matrix, &(Manager::CAM->translate_matrix));

		AEVec2 Result;
		AEMtx33MultVec(&Result, &translate_matrix, &MP);
		f32 length = AEVec2Distance(&position, &Result);
		f32 dx = Result.x - position.x;
		f32 dy = Result.y - position.y;
		MouseDirection.x = dx / length;
		MouseDirection.y = dy / length;
	}
	void PlayerCharacter::UpdateKill(u32 Exp)
	{
		Stats.ExpCount += Exp;
		if (Stats.ExpCount >= Stats.TargetExp)
		{
			Stats.ExpCount -= Stats.TargetExp;
			Stats.TargetExp *= 2;
			Stats.Level++;
			std::cout << "Level Up : " << Stats.Level  << " Next : Target Exp : " << Stats.TargetExp << std::endl;
			Manager::pickPanel.Show(Manager::pickPanel.GenerateRandomRewards());
		}
	}
	void PlayerCharacter::AddItemToInventory(std::shared_ptr<Item> item)
	{
		auto it = std::find_if(inventory.begin(), inventory.end(),
			[&](const auto& pair) {return pair.first->id == item->id; });
		if (it == inventory.end())
		{
			inventory.push_back({ std::move(item), 1 });
		}
		else
		{
			it->second++;
		}
	}
}