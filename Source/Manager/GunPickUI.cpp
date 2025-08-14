#include "GunPickUI.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"
#include "GameManager.h"
#include "GameOver.h"
#include "../InGame/Gun.h"
#include <random>
#include <unordered_set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../Manager/Playing.h"
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
					u8 idx = std::clamp(global::CurrentStageNumber, 0, 2);
					stageArrow.position = stagePosition[idx];
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
		stageBG.size = { 240.f, 300.f };
		stageBG.position = { -w / 2.f + stageBG.size.x / 1.2f , 0 }; //-h / 2.f + stageBG.size.y / 1.2f };
		bgMesh = Utils::CreateNinePatchMesh();
		stageBGTexture = AEGfxTextureLoad("Assets/SelectItem_LevelUp.png");

		stageMAP.position = stageBG.position;
		stageMAP.size = { stageBG.size.x, stageBG.size.y };
		m_Mesh = Utils::CreateMesh();
		stageMAPTexture = AEGfxTextureLoad("Assets/UI/stageMAP.png");
		//flag
		stageArrow.offset = { 0,0 };
		stageArrow.Texture = AEGfxTextureLoad("Assets/UI/RedArrow.png");
		stageArrow.position = { stageBG.position.x -stageBG.size.x/2.f + 20.f,(stageBG.position.y - stageBG.size.y / 2.f)};
		stageArrow.size = { 40.f, 30.f };
		stageArrow.TimeAcc = 0.f;
		stageArrow.FrameTime = 1.f / 59.f;
		stageArrow.MaxAnimationCount[InGame::IDLE] = 59;
		stageArrow.AnimationCount = 0;
		stageArrow.column = 59;
		stageArrow.row = 1;
		stageArrow.Mesh = Utils::CreateMesh(stageArrow.row, stageArrow.column);
		stagePosition = {
			{stageBG.position.x - stageBG.size.x / 2.f + 20.f,(stageBG.position.y - stageBG.size.y / 2.f) + (stageBG.size.y / 55.f) * 19.f},
			{stageBG.position.x - stageBG.size.x / 2.f + 20.f,(stageBG.position.y - stageBG.size.y / 2.f) + (stageBG.size.y / 55.f) * 35.f},
			{stageBG.position.x - stageBG.size.x / 2.f + 20.f,(stageBG.position.y - stageBG.size.y / 2.f) + (stageBG.size.y / 55.f) * 50.f}
		};
		std::ifstream file("Assets/gunDescription.txt", std::ios::binary);

		if (!file.is_open()) {
			std::cerr << "Error: Could not open description file: " << std::endl;
			return;
		}

		std::string line;

		for (int i = 1; i <= static_cast<int>(InGame::GunType::LAST); i++)
		{
			std::getline(file, line);
			if (!line.empty() && line.back() == '\r')
			{
				line.pop_back();
			}
			gunDescriptions[static_cast<InGame::GunType>(i)] = line;
		}

		file.close();

		std::ifstream fileStat("Assets/statKR.txt", std::ios::binary);

		if (!fileStat.is_open()) {
			std::cerr << "Error: Could not open description file: " << std::endl;
			return;
		}

		for (int i = 0; i <= static_cast<int>(StatsForUI::STAT_LAST); i++)
		{
			std::getline(fileStat, line);
			if (!line.empty() && line.back() == '\r')
			{
				line.pop_back();
			}
			statsNameKR[static_cast<StatsForUI>(i)] = line;
		}

		fileStat.close();

		InitGunStats();

	}
	void GunPickUI::InitGunStats()
	{
		gunStats[InGame::GunType::NOGUN] = { 1.f, 1.f, 1, 0 };
		gunStats[InGame::GunType::SAWEDOFFSHOTGUN] = { 1.1f, 0.7f, 1, 3 };
		gunStats[InGame::GunType::DOUBLEBARREL] = { 2.f, 1.0f, 1, 5 };
		gunStats[InGame::GunType::KS23] = { 5.f, 0.2f, 3, 5 };
		gunStats[InGame::GunType::M1897] = { 1.f, 2.0f, 1, 5 };
		gunStats[InGame::GunType::BENELLIM4] = { 1.f, 3.f, 1, 5 };
		gunStats[InGame::GunType::SAIGA12] = { 1.f, 2.0f, 1, 5 };
		gunStats[InGame::GunType::AA12] = { 1.f, 3.5f, 1, 5 };
		gunStats[InGame::GunType::M1911] = { 1.2f, 1.3f, 2, 1 };
		gunStats[InGame::GunType::MOSINNAGAT] = { 3.f, 1.0f, 5, 1 };
		gunStats[InGame::GunType::M24] = { 5.f, 2.f, 10, 1 };
		gunStats[InGame::GunType::RAILGUN] = { 10.f, 1.f, 20, 1 };
		gunStats[InGame::GunType::DESERTEGLE] = { 3.f, 1.f, 3, 1 };
		gunStats[InGame::GunType::BEOWOLF] = { 5.f, 2.f, 5, 1 };
		gunStats[InGame::GunType::NITRO700] = { 20.f, 0.5f, 40, 1 };
		gunStats[InGame::GunType::FNFAL] = { 3.f, 1.0f, 1, 1 };
		gunStats[InGame::GunType::M82BARRETT] = { 10.f, 0.5f, 10, 1 };
		gunStats[InGame::GunType::CZ75] = { 0.75f, 2.5f, 1, 1 };
		gunStats[InGame::GunType::AR15] = { 1.f, 2.4f, 2, 1 };
		gunStats[InGame::GunType::M110] = { 1.6f, 1.6f, 5, 1 };
		gunStats[InGame::GunType::MP5] = { 1.f, 3.0f, 1, 1 };
		gunStats[InGame::GunType::MPX] = { 1.f, 3.0f, 2, 1 };
		gunStats[InGame::GunType::VECTOR] = { 0.5f, 10.f, 1, 1 };
		gunStats[InGame::GunType::P90] = { 1.f, 5.f, 3, 1 };
		gunStats[InGame::GunType::BREN] = { 1.f, 2.f, 3, 1 };
		gunStats[InGame::GunType::MICROGUN] = { 1.f, 10.f, 1, 1 };
		gunStats[InGame::GunType::M249] = { 1.f, 3.f, 2, 1 };
		gunStats[InGame::GunType::M2] = { 10.f, 1.f, 5, 1 };
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
				f32 sizeX = gunIcons[i].size.x, sizeY = gunIcons[i].size.y;
				f32 startX = gunIcons[i].position.x + sizeX/1.7f, Y = gunIcons[i].position.y + sizeY/2.f;

				Utils::DrawObject(gunIcons[i], gunIcons[i].Texture, iconMesh);
				u32 col = 0xFFFFFFFF;
				switch (global::CurrentStageNumber)
				{
				case 1:
					col = 0xFFFFFFFF;
					break;
				case 2:
					col = 0xB2B2FFFF;
					break;
				case 3:
					col = 0xFF7FFFFF;
					break;
				default:
					col = 0xFFFFFFFF;
					break;
				}
				Manager::Atlas.RenderTextUTF8(gunDescriptions[weaponOptions[i]], startX, Y + padding * 1.5f , 1.f);

				if (weaponOptions[i] == InGame::GunType::NOGUN)
				{
					Manager::Atlas.RenderTextUTF8(GunNames[i], startX, Y - padding * 2.f, 2.5f, col);
					continue;
				}
				else
				{
					Manager::Atlas.RenderTextUTF8(GunNames[i], startX, Y - padding * 2.f, 3.f, col);
				}
				auto m = Manager::Atlas.GetPrintMetricsUTF8(GunNames[i], 3.f);
				f32 bigGap = m.height;
				m = Manager::Atlas.GetPrintMetricsUTF8(gunDescriptions[weaponOptions[i]], 1.f);
				f32 smallGap = m.height;


				std::stringstream ss;
				std::string inputString;

				ss << "-" << statsNameKR[StatsForUI::DAMAGE_RATIO] << ": " << gunStats[weaponOptions[i]].damage;
				inputString = ss.str();
				Manager::Atlas.RenderTextUTF8(inputString, startX, Y - padding * 2.f - smallGap * 1.5f, 1.f);
				ss.str("");

				ss << "-" << statsNameKR[StatsForUI::FIRE_RATE_RATIO] << ": " << gunStats[weaponOptions[i]].fireRate;
				inputString = ss.str();
				Manager::Atlas.RenderTextUTF8(inputString, startX, Y - padding * 2.f - smallGap * 2.5f, 1.f);
				ss.str("");

				s32 hitCount = gunStats[weaponOptions[i]].hitCount - 1;
				ss << "-" << statsNameKR[StatsForUI::HIT_COUNT] << ": " << hitCount;
				inputString = ss.str();
				Manager::Atlas.RenderTextUTF8(inputString, startX, Y - padding * 2.f - smallGap * 3.5f, 1.f);
				ss.str("");

				ss << "-" << statsNameKR[StatsForUI::PROJECTILE_COUNT] << ": " << gunStats[weaponOptions[i]].projectileCount;
				inputString = ss.str();
				Manager::Atlas.RenderTextUTF8(inputString, startX, Y - padding * 2.f - smallGap * 4.5f, 1.f);
				ss.str("");

				ss.clear();
		}
	}

	void GunPickUI::Show()
	{
		isActive = true;
		shouldShowStage = true;
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
				break;
			case InGame::GunType::CZ75:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/CZ75.png");
				GunNames[i] = "CZ75";
				break;
			case InGame::GunType::DESERTEGLE:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/DESERTEGLE.png");
				GunNames[i] = "DESERT EAGLE";
				break;
			case InGame::GunType::MP5:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/MP5.png");
				GunNames[i] = "MP5";
				break;
			case InGame::GunType::MPX:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/MPX.png");
				GunNames[i] = "MPX";
				break;
			case InGame::GunType::VECTOR:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/VECTOR.png");
				GunNames[i] = "VECTOR";
				break;
			case InGame::GunType::BEOWOLF:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/BEOWOLF.png");
				GunNames[i] = "BEOWOLF";
				break;
			case InGame::GunType::P90:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/P90.png");
				GunNames[i] = "P90";
				break;
			case InGame::GunType::MOSINNAGAT:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/MOSINNAGAT.png");
				GunNames[i] = "MOSINNAGANT";
				break;
			case InGame::GunType::M24:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M24.png");
				GunNames[i] = "M24";
				break;
			case InGame::GunType::RAILGUN:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/RAILGUN.png");
				GunNames[i] = "RAILGUN";
				break;
			case InGame::GunType::NITRO700:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/NITRO700.png");
				GunNames[i] = "NITRO700";
				break;
			case InGame::GunType::FNFAL:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/FNFAL.png");
				GunNames[i] = "FNFAL";
				break;
			case InGame::GunType::M82BARRETT:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M82BARRETT.png");
				GunNames[i] = "M82BARRETT";
				break;
			case InGame::GunType::AR15:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/AR15.png");
				GunNames[i] = "AR15";
				break;
			case InGame::GunType::M110:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M110.png");
				GunNames[i] = "M110";
				break;
			case InGame::GunType::BREN:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/BREN.png");
				GunNames[i] = "BREN";
				break;
			case InGame::GunType::MICROGUN:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/MICROGUN.png");
				GunNames[i] = "MICROGUN";
				break;
			case InGame::GunType::M249:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M249.png");
				GunNames[i] = "M249";
				break;
			case InGame::GunType::M2:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M2.png");
				GunNames[i] = "M2";
				break;
			case InGame::GunType::SAWEDOFFSHOTGUN:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/SAWEDOFFSHOTGUN.png");
				GunNames[i] = "SHORTY";
				break;
			case InGame::GunType::DOUBLEBARREL:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/DOUBLEBARREL.png");
				GunNames[i] = "DOUBLEBARREL";
				break;
			case InGame::GunType::KS23:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/KS23.png");
				GunNames[i] = "KS23";
				break;
			case InGame::GunType::M1897:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/M1897.png");
				GunNames[i] = "M1897";
				break;
			case InGame::GunType::BENELLIM4:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/BENELLIM4.png");
				GunNames[i] = "BENELLIM4";
				break;
			case InGame::GunType::SAIGA12:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/SAIGA12.png");
				GunNames[i] = "SAIGA12";
				break;
			case InGame::GunType::AA12:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/AA12.png");
				GunNames[i] = "AA12";
				break;
			default:
				gunIcons[i].Texture = AEGfxTextureLoad("Assets/Guns/Empty.png");
				GunNames[i] = "아무것도 없네요...";
				break;
			}
			gunIcons[i].position.x = weaponOptionButtons[i].position.x - weaponOptionButtons[i].size.x / 2.f + gunIcons[i].size.x / 2.f;
			gunIcons[i].position.y = weaponOptionButtons[i].position.y;
		}
		gm.Pause();
	}

	void GunPickUI::ShowStageUpdate()
	{
		f32 bottomY{ (stageBG.position.y - stageBG.size.y / 2.f) };
		f32 destinationY{ 0.f };

		switch (global::CurrentStageNumber)
		{
		case 1:
			destinationY = stagePosition[0].y;
			break;
		case 2:
			destinationY = stagePosition[1].y;
			break;
		case 3:
			destinationY = stagePosition[2].y;
			break;
		}

		if (stageArrow.position.y < destinationY)
		{
			stageArrow.position.y += global::DeltaTime*50;
		}
		else if (stageArrow.position.y > destinationY)
		{
			stageArrow.position.y = destinationY;
		}
		Utils::UpdateOffset(stageArrow);
	}

	void GunPickUI::ShowStageDraw()
	{
		Utils::DrawNinePatchMesh(stageBG, stageBGTexture, bgMesh, 20.f, 1.f);
		Utils::DrawObject(stageMAP, stageMAPTexture, m_Mesh, 1.f);
		//Utils::DrawObject(stageArrow.position, stageArrow.offset, stageArrow.size, stageArrow.Texture, stageArrow.Mesh, 1.f, false);
		Utils::DrawObject(stageArrow, false);
	}

	void GunPickUI::Destroy()
	{
		for (auto mesh : ButtonMesh)
		{
			if(mesh)
				AEGfxMeshFree(mesh);
		}
		AEGfxTextureUnload(ButtonTexture);

		AEGfxMeshFree(iconMesh);
		for (int i = 0; i < gunIcons.size(); i++)
		{
			if(gunIcons[i].Texture)
				AEGfxTextureUnload(gunIcons[i].Texture);
		}

		for (auto& mesh : bgMesh)
		{
			if (mesh)
			{
				AEGfxMeshFree(mesh);
				mesh = nullptr;
			}
		}
		if (stageBGTexture)
		{
			AEGfxTextureUnload(stageBGTexture);
			stageBGTexture = nullptr;
		}

		if (m_Mesh)
		{
			AEGfxMeshFree(m_Mesh);
			m_Mesh = nullptr;
		}
		if (stageMAPTexture)
		{
			AEGfxTextureUnload(stageMAPTexture);
			stageMAPTexture = nullptr;
		}

		if (stageArrow.Mesh)
		{
			AEGfxMeshFree(stageArrow.Mesh);
		}
		if (stageArrow.Texture)
		{
			AEGfxTextureUnload(stageArrow.Texture);
			stageArrow.Texture = nullptr;
		}

		AEGfxDestroyFont(pFont);
	}
	bool GunPickUI::IsActive() const
	{
		return isActive;
	}
}