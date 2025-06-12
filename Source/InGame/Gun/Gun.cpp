#include "Gun.h"
#include "../../Global/GlobalVariables.h"
#include "../../Utils/Utils.h"
#include <iostream>
#include "../../Manager/GameManager.h"
#include "../Projectile.h"
#include "../../Manager/PlayingState.h"
namespace InGame
{
	void Gun::Init()
	{
		Mesh = Utils::CreateMesh();
		Texture = AEGfxTextureLoad("Assets/TestBlankWhite.png");
	}
	void Gun::Update(AEVec2 Dir, AEVec2 Pos)
	{
		FireTimer += global::DeltaTime;
		if (FireTimer > ChamberTime)
		{
			FireProjectile(Dir, Pos);
			FireTimer = 0.f;
		}
	}

	void Gun::Draw()
	{
		Utils::DrawObject(*this);
	}

	void Gun::FireProjectile(AEVec2 Dir, AEVec2 Pos)
	{
		Projectile* PP = new Projectile();
		PP->Init(Dir,Pos);
		PP->direction = Dir;
		if (Manager::gm.currStateREF)
		{
			static_cast<Manager::Playing*>(Manager::gm.currStateREF)->PPs.push_back(PP);
		}
		
		std::cout << "ProjectileFired" << std::endl;
	}
}

