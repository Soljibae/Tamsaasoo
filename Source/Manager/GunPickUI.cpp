#include "GunPickUI.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include "GameManager.h"
#include "GameOver.h"
#include "../InGame/Gun.h"
#include <random>
#include <unordered_set>
namespace Manager
{
	GunPickUI gunPickPanel;
	static f32 w = static_cast<f32>(global::ScreenWidth);
	static f32 h = static_cast<f32>(global::ScreenHeight);
	static f32 buttonWidth = w / 2.f;
	static f32 buttonHeight = h / 5.f;
	static f32 spacingY = 50.f;
	static f32 totalHeight = 3 * buttonHeight + 2 * spacingY;
	static f32 margin = 100.f;
	static f32 padding = 15.f;
	static f32 startY = -(h / 2.f) + margin + (buttonHeight / 2.f);
	static const s32 fontSize{72};
	static const f32 nameDrawSize{0.4f};
	static const f32 descDrawSize{0.18f};

	void GunPickUI::Init(InGame::PlayerCharacter* InPC)
	{
		PC = InPC;
		weaponOptions = {
			InGame::GunType::M1911,
			InGame::GunType::SAWEDOFFSHOTGUN,
			InGame::GunType::CZ75 
		};
		for (int i = 0; i < weaponOptionButtons.size(); i++)
		{
			SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/pistol_reload.WAV", "pistol_reload");
			SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/machinegun_reload.WAV", "rifle_reload");
			SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/shotgun_reload.WAV", "shotgun_reload");
			
			weaponOptionButtons[i].size = { buttonWidth, buttonHeight };
			weaponOptionButtons[i].Init();
			weaponOptionButtons[i].position = {
				0,
				startY + i * (buttonHeight + spacingY)
			};
			weaponOptionButtons[i].SetCallback([this, i](){
				if (weaponOptions[i] != InGame::GunType::NOGUN)
				{
					delete PC->GunData;
					PC->HoldingGun->Destroy();
					delete PC->HoldingGun;
					PC->HoldingGun = nullptr;
					switch (weaponOptions[i])
					{
					case InGame::GunType::M1911:
						PC->GunData = new InGame::M1911Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("pistol_reload");
						break;
					case InGame::GunType::CZ75:
						PC->GunData = new InGame::CZ75Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("pistol_reload");
						break;
					case InGame::GunType::DESERTEGLE:
						PC->GunData = new InGame::DESERTEGLEStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("pistol_reload");
						break;
					case InGame::GunType::MP5:
						PC->GunData = new InGame::MP5Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("pistol_reload");
						break;
					case InGame::GunType::MPX:
						PC->GunData = new InGame::MPXStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("pistol_reload");
						break;
					case InGame::GunType::VECTOR:
						PC->GunData = new InGame::VECTORStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("pistol_reload");
						break;
					case InGame::GunType::BEOWOLF:
						PC->GunData = new InGame::BEOWOLFStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::P90:
						PC->GunData = new InGame::P90Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("pistol_reload");
						break;
					case InGame::GunType::MOSINNAGAT:
						PC->GunData = new InGame::MOSINNAGATStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::M24:
						PC->GunData = new InGame::M24Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::RAILGUN:
						PC->GunData = new InGame::RAILGUNStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::NITRO700:
						PC->GunData = new InGame::NITRO700Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("shotgun_reload");
						break;
					case InGame::GunType::FNFAL:
						PC->GunData = new InGame::FNFALStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::M82BARRETT:
						PC->GunData = new InGame::M82BARRETTStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::AR15:
						PC->GunData = new InGame::AR15Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::M110:
						PC->GunData = new InGame::M110Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::BREN:
						PC->GunData = new InGame::BRENStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::MICROGUN:
						PC->GunData = new InGame::MICROGUNStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("pistol_reload");
						break;
					case InGame::GunType::M249:
						PC->GunData = new InGame::M249Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::M2:
						PC->GunData = new InGame::M2Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("rifle_reload");
						break;
					case InGame::GunType::SAWEDOFFSHOTGUN:
						PC->GunData = new InGame::SAWEDOFFSHOTGUNStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("shotgun_reload");
						break;
					case InGame::GunType::DOUBLEBARREL:
						PC->GunData = new InGame::DOUBLEBARRELStruct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("shotgun_reload");
						break;
					case InGame::GunType::KS23:
						PC->GunData = new InGame::KS23Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("shotgun_reload");
						break;
					case InGame::GunType::M1897:
						PC->GunData = new InGame::M1897Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("shotgun_reload");
						break;
					case InGame::GunType::BENELLIM4:
						PC->GunData = new InGame::BENELLIM4Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("shotgun_reload");
						break;
					case InGame::GunType::SAIGA12:
						PC->GunData = new InGame::SAIGA12Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("shotgun_reload");
						break;
					case InGame::GunType::AA12:
						PC->GunData = new InGame::AA12Struct();
						PC->HoldingGun = new InGame::Gun();
						PC->HoldingGun->Init(PC);
						SFXManager.Play("shotgun_reload");
						break;
					}
					stageIdx++;
					isActive = false;
					gm.Resume();
				}
				});
			gunIcons[i].size = { buttonWidth / 5.f, buttonHeight };
			gunIcons[i].position.x = weaponOptionButtons[i].position.x-weaponOptionButtons[i].size.x/2.f+gunIcons[i].size.x/2.f;
			gunIcons[i].position.y = weaponOptionButtons[i].position.y;
			gunIcons[i].Texture = { nullptr };
		}
		iconMesh = Utils::CreateMesh();
		ButtonMesh = Utils::CreateNinePatchMesh();
		ButtonTexture = AEGfxTextureLoad("Assets/SelectItem_LevelUp.png");
		colors[0] = { 1.0f, 1.0f, 1.0f }, colors[1] = { 0.7f, 0.7f, 1.0f }, colors[2] = { 1.0f, 0.5f, 1.0f }, colors[3] = { 0, 0, 0 };
		stageIdx = 0;
		pFont = AEGfxCreateFont("Assets/Fonts/buggy-font.ttf", fontSize);
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
			if (weaponOptions[i] != InGame::GunType::NOGUN)
			{
				f32 sizeX = gunIcons[i].size.x, sizeY = gunIcons[i].size.y;
				f32 startX = gunIcons[i].position.x + sizeX/1.7f, Y = gunIcons[i].position.y + sizeY/2.f;
				f32 lw, lh;
				AEGfxGetPrintSize(pFont, GunNames[i].c_str(), nameDrawSize, &lw, &lh);
				lw *= w, lh *= h;

				Utils::DrawObject(gunIcons[i], gunIcons[i].Texture, iconMesh);
				AEGfxPrint(pFont, GunNames[i].c_str(), startX/(w/2.f), (Y-lh-padding)/(h/2.f), nameDrawSize,
					colors[stageIdx].r,
					colors[stageIdx].g,
					colors[stageIdx].b,
					1.f
				);
				AEGfxGetPrintSize(pFont, GunDescriptions[i].c_str(), descDrawSize, &lw, &lh);
				lh *= 2.f;
				AEGfxPrint(pFont, GunDescriptions[i].c_str(), startX/(w/2.f), (Y-lh-padding)/(h/2.f), descDrawSize,
					1.f, 1.f, 1.f, 1.f);
			}
		}
	}
	void GunPickUI::Show()
	{
		isActive = true;
		InGame::GetNextType(PC->HoldingGun->gunType, weaponOptions[0], weaponOptions[1], weaponOptions[2]);
		for (int i = 0; i < weaponOptions.size(); i++)
		{
			if (gunIcons[i].Texture)
			{
				AEGfxTextureUnload(gunIcons[i].Texture);
				gunIcons[i].Texture = nullptr;
			}
			switch (weaponOptions[i])
			{
			case InGame::GunType::M1911:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M1911.png");
				GunNames[i] = "M1911";
				GunDescriptions[i] = "HighPowerPistol";
				break;
			case InGame::GunType::CZ75:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/CZ75.png");
				GunNames[i] = "CZ75";
				GunDescriptions[i] = "AutoMaticPistol";
				break;
			case InGame::GunType::DESERTEGLE:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/DESERTEGLE.png");
				GunNames[i] = "DESERT EGLE";
				GunDescriptions[i] = "High Power Pistol";
				break;
			case InGame::GunType::MP5:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/MP5.png");
				GunNames[i] = "MP5";
				GunDescriptions[i] = "Pistol caliber high rate of fire SMG";
				break;
			case InGame::GunType::MPX:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/MPX.png");
				GunNames[i] = "MPX";
				GunDescriptions[i] = "Additional Penetration";
				break;
			case InGame::GunType::VECTOR:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/VECTOR.png");
				GunNames[i] = "VECTOR";
				GunDescriptions[i] = "High rate of fire";
				break;
			case InGame::GunType::BEOWOLF:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/BEOWOLF.png");
				GunNames[i] = "BEOWOLF";
				GunDescriptions[i] = "High power rifle";
				break;
			case InGame::GunType::P90:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/P90.png");
				GunNames[i] = "P90";
				GunDescriptions[i] = "More Penetration";
				break;
			case InGame::GunType::MOSINNAGAT:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/MOSINNAGAT.png");
				GunNames[i] = "MOSINNAGAT";
				GunDescriptions[i] = "Slow Rate of fire Sniper rifle";
				break;
			case InGame::GunType::M24:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M24.png");
				GunNames[i] = "M24";
				GunDescriptions[i] = "Better Rate of fire Sniper rifle";
				break;
			case InGame::GunType::RAILGUN:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/RAILGUN.png");
				GunNames[i] = "RAILGUN";
				GunDescriptions[i] = "Low Rate of fire, high Damage Sniper rifle";
				break;
			case InGame::GunType::NITRO700:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/NITRO700.png");
				GunNames[i] = "NITRO700";
				GunDescriptions[i] = "Extream Power";
				break;
			case InGame::GunType::FNFAL:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/FNFAL.png");
				GunNames[i] = "FNFAL";
				GunDescriptions[i] = "High Damage High penetration Battle Rifle";
				break;
			case InGame::GunType::M82BARRETT:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M82BARRETT.png");
				GunNames[i] = "M82BARRETT";
				GunDescriptions[i] = "High Damage, High penetration";
				break;
			case InGame::GunType::AR15:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/AR15.png");
				GunNames[i] = "AR15";
				GunDescriptions[i] = "Small Damage, High rate of fire";
				break;
			case InGame::GunType::M110:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M110.png");
				GunNames[i] = "M110";
				GunDescriptions[i] = "High Damage, Medium rate of fire";
				break;
			case InGame::GunType::BREN:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/BREN.png");
				GunNames[i] = "BREN";
				GunDescriptions[i] = "High Damage, Medium rate of fire";
				break;
			case InGame::GunType::MICROGUN:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/MICROGUN.png");
				GunNames[i] = "MICROGUN";
				GunDescriptions[i] = "Low Damage, Extream rate of fire";
				break;
			case InGame::GunType::M249:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M249.png");
				GunNames[i] = "M249";
				GunDescriptions[i] = "Medium Damage, High rate of fire";
				break;
			case InGame::GunType::M2:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M2.png");
				GunNames[i] = "M2";
				GunDescriptions[i] = "High Damage, High penetration, Slow rate of fire";
				break;
			case InGame::GunType::SAWEDOFFSHOTGUN:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/SAWEDOFFSHOTGUN.png");
				GunNames[i] = "SAWEDOFFSHOTGUN";
				GunDescriptions[i] = "Shot Gun, Slow rate of fire";
				break;
			case InGame::GunType::DOUBLEBARREL:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/DOUBLEBARREL.png");
				GunNames[i] = "DOUBLEBARREL";
				GunDescriptions[i] = "Higher Damage";
				break;
			case InGame::GunType::KS23:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/KS23.png");
				GunNames[i] = "KS23";
				GunDescriptions[i] = "Extream Damage, Low rate of fire";
				break;
			case InGame::GunType::M1897:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M1897.png");
				GunNames[i] = "M1897";
				GunDescriptions[i] = "Medium Damage, Medium rate of fire";
				break;
			case InGame::GunType::BENELLIM4:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/BENELLIM4.png");
				GunNames[i] = "BENELLIM4";
				GunDescriptions[i] = "Well Balenced";
				break;
			case InGame::GunType::SAIGA12:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/SAIGA12.png");
				GunNames[i] = "SAIGA12";
				GunDescriptions[i] = "LowDamage, High rate of fire";
				break;
			case InGame::GunType::AA12:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/AA12.png");
				GunNames[i] = "AA12";
				GunDescriptions[i] = "BulletStorm";
				break;
			}
			gunIcons[i].position.x = weaponOptionButtons[i].position.x - weaponOptionButtons[i].size.x / 2.f + gunIcons[i].size.x / 2.f;
			gunIcons[i].position.y = weaponOptionButtons[i].position.y;
		}
		gm.Pause();
	}
	void GunPickUI::Destroy()
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
		AEGfxDestroyFont(pFont);
	}
	bool GunPickUI::IsActive() const
	{
		return isActive;
	}
}