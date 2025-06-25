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
	}
	void ArealAttack::Update()
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
}
