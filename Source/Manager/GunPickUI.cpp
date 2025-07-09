#include "GunPickUI.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include "GameManager.h"
#include "GameOver.h"
#include <random>
#include <unordered_set>
namespace Manager
{
	GunPickUI gunPickPanel;
	static const f32 w = static_cast<f32>(global::ScreenWidth);
	static const f32 h = static_cast<f32>(global::ScreenHeight);
	static f32 buttonWidth = w / 2.f;
	static f32 buttonHeight = h / 5.f;
	static f32 spacingY = 50.f;
	static f32 totalHeight = 3 * buttonHeight + 2 * spacingY;
	static f32 margin = 60.f;
	static f32 padding = 15.f;
	static f32 startY = -(h / 2.f) + margin + (buttonHeight / 2.f);

	void GunPickUI::Init(InGame::PlayerCharacter* InPC)
	{
		PC = InPC;
		for (int i = 0; i < weaponOptionButtons.size(); i++)
		{
			weaponOptionButtons[i].size = { buttonWidth, buttonHeight };
			weaponOptionButtons[i].Init();
			weaponOptionButtons[i].position = {
				0,
				startY + i * (buttonHeight + spacingY)
			};
			weaponOptionButtons[i].SetCallback([this, i](){
				delete PC->GunData;
				PC->HoldingGun->Destroy();
				delete PC->HoldingGun;
				PC->HoldingGun = nullptr;
				switch (weaponOptions[i])
				{
				case InGame::GunType::SHOTGUN:
					PC->GunData = new InGame::ShotGunStruct();
					PC->HoldingGun = new InGame::Gun();
					PC->HoldingGun->Init(PC);
					break;
				case InGame::GunType::RIFLE:
					PC->GunData = new InGame::RifleStruct();
					PC->HoldingGun = new InGame::Gun();
					PC->HoldingGun->Init(PC);
					break;
				case InGame::GunType::PISTOL:
					PC->GunData = new InGame::PistolStruct();
					PC->HoldingGun = new InGame::Gun();
					PC->HoldingGun->Init(PC);
					break;
				}
				isActive = false;
				gm.Resume();
				});
			gunIcons[i].size = {buttonWidth/5.f, buttonHeight};
		}
		iconMesh = Utils::CreateMesh();
		ButtonMesh = Utils::CreateNinePatchMesh();
		ButtonTexture = AEGfxTextureLoad("Assets/SelectItem_LevelUp.png");
	}
	void GunPickUI::Update()
	{
		if (!isActive || gameOverScreen.isGameOver)
			return;
		for (int i = 0; i < weaponOptions.size(); i++)
		{
			weaponOptionButtons[i].Update();
			gunIcons[i].size = { weaponOptionButtons[i].size.x/5.f, weaponOptionButtons[i].size.y/2.f };
		}
	}
	void GunPickUI::Draw()
	{
		if (!isActive || gameOverScreen.isGameOver)
			return;
		for (int i = 0; i < weaponOptions.size(); i++)
		{
			Utils::DrawNinePatchMesh(weaponOptionButtons[i], ButtonTexture, ButtonMesh, padding);
			Utils::DrawObject(gunIcons[i], gunIcons[i].Texture, iconMesh);
		}
	}
	void GunPickUI::Show()
	{
		isActive = true;
		weaponOptions = GenerateRandomGun();
		for (int i = 0; i < weaponOptions.size(); i++)
		{
			if (gunIcons[i].Texture)
			{
				AEGfxTextureUnload(gunIcons[i].Texture);
				gunIcons[i].Texture = nullptr;
			}

			switch (weaponOptions[i])
			{
			case InGame::SHOTGUN:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/ShotGun.png");
				break;
			case InGame::RIFLE:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Rifle.png");
				break;
			case InGame::PISTOL:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Pistol.png");
				break;
			}
			gunIcons[i].position = weaponOptionButtons[i].position;
		}
		gm.Pause();
	}
	void GunPickUI::Destory()
	{
		for (auto mesh : ButtonMesh)
		{
			if(mesh)
				AEGfxMeshFree(mesh);
		}
		AEGfxMeshFree(iconMesh);
		for (int i = 0; i < gunIcons.size(); i++)
		{
			if(gunIcons[i].Texture)
				AEGfxTextureUnload(gunIcons[i].Texture);
		}
		AEGfxTextureUnload(ButtonTexture);
	}
	bool GunPickUI::IsActive() const
	{
		return isActive;
	}
	std::array<InGame::GunType, 3> GunPickUI::GenerateRandomGun()
	{
		std::array<InGame::GunType, 3> rewards;
		s16 min = 0, max = static_cast<s16>(InGame::GunType::LAST);
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution dis(min, max);
		std::unordered_set<int> used_indicies;
		s16 idx{ 0 };
		for (s32 i = 0; i < rewards.size(); i++)
		{
			while (true)
			{
				idx = dis(gen);
				auto result = used_indicies.insert(idx);
				if (result.second)
					break;
			}
			rewards[i] = static_cast<InGame::GunType>(idx);
		}
		return rewards;
	}
}