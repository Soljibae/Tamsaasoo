#include "PlayingState.h"

namespace Manager
{
	Utils::Camera* CAM = nullptr;

	void Playing::Init()
	{
		if (PC == nullptr)
		{
			PC = new InGame::PlayerCharacter();
			PC->Init();
		}

		if (CAM == nullptr)
		{
			CAM = new Utils::Camera();
			CAM->Init(*PC);
		}
	}
	void Playing::Update()
	{
		PC->Update();
		for (InGame::Projectile* PP : PPs)
		{
			PP->Update();
		}
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->Update();
		}
		for (InGame::Projectile* EP : EPs)
		{
			EP->Update();
		}
		CAM->Update(*PC);
	}
	void Playing::Draw()
	{
		PC->Draw();
		for (InGame::Projectile* PP : PPs)
		{
			PP->Draw();
		}
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->Draw();
		}
		for (InGame::Projectile* EP : EPs)
		{
			EP->Draw();
		}
	}
	void Playing::Destroy()
	{
		PC->Destroy();
		delete PC;
		PC = nullptr;
		for (InGame::Projectile* PP : PPs)
		{
			PP->Destroy();
			delete PP;
		}
		bool bSuccess = PPs.empty();
		for (InGame::EnemyCharacter* EC : ECs)
		{
			EC->Destroy();
			delete EC;
		}
		bSuccess = ECs.empty();
		for (InGame::Projectile* EP : EPs)
		{
			EP->Destroy();
			delete EP;
		}
		bSuccess = EPs.empty();
		delete CAM;
	}
}