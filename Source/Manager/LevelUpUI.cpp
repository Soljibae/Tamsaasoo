#include "LevelUpUI.h"
#include "HUDController.h"
#include "GameOver.h"
#include <random>
#include <unordered_set>
namespace Manager
{
	LevelUpUI pickPanel;
	AEGfxTexture* LevelUpUI::windowTexture{ nullptr };
	AEGfxVertexList* LevelUpUI::rerollMesh{ nullptr };
	AEGfxTexture* LevelUpUI::rerollTexture{ nullptr };
	AEGfxTexture* LevelUpUI::ItemSlotTexture{ nullptr };
	const static f32 fontSize = 72.f;
	const static f32 textDrawSize = 0.2f;
	const static f32 windowWidth = 350.f;
	const static f32 windowHeight = 550.f;
	const static f32 spacingX = 130.0f;
	const static f32 padding = 50.f;
	const static f32 probEpic = 0.04f;
	const static f32 probRare = 0.24f;
	static f32 w{ static_cast<f32>(global::ScreenWidth) }, h{ static_cast<f32>(global::ScreenHeight) };
	void LevelUpUI::ResetGotEpic()
	{
		epicAlpha = resetAlpha;
		gotEpic = false;
	}
	void LevelUpUI::Init(InGame::PlayerCharacter* InPC)
	{
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/pick.wav", "pick");
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/reroll.wav", "reroll");
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/epic.wav", "epic");
		currentOptions.reserve(3);
		PC = InPC;

		f32 totalWidth = 3 * windowWidth + 2 * spacingX;
		f32 margin = (global::ScreenWidth - totalWidth) * 0.5f;
		f32 startX = -global::ScreenWidth * 0.5f + margin + windowWidth * 0.5f;
		f32 rerollSize = 100.f;
		for (s8 i = 0; i < ItemWindow.size(); ++i)
		{
			ItemSlot[i].Texture = AEGfxTextureLoad("Assets/ItemSlots/itemSlot.png");
			ItemWindow[i].position = {
				startX + i * (windowWidth + spacingX),
				0
			};
			ItemWindow[i].size = { windowWidth, windowHeight };
			ItemWindow[i].Texture = AEGfxTextureLoad("Assets/black.png");
			ItemWindow[i].SetCallback([this, i]()
				{
					SFXManager.Play("pick");
					PC->AddItemToInventory(currentOptions[i]->Clone());
					this->isActive = false;
					for (auto& cost : rerollCost)
					{
						cost = 20;
					}
					delayTime = 0.f;
					ResetGotEpic();
					gm.Resume();
				});
			f32 winHalfW{ windowWidth / 2.f }, winHalfH{ windowHeight / 2.f };
			rerollButton[i].position.x = ItemWindow[i].position.x - winHalfW + (rerollSize / 2.f) + padding;
			rerollButton[i].position.y = ItemWindow[i].position.y - winHalfH + padding;
			rerollButton[i].size = { rerollSize, rerollSize };
			rerollButton[i].SetCallback([this, i]() {Reroll(i); });
			f32 baseX{ rerollButton[i].position.x }, baseY{ rerollButton[i].position.y };
			rerollCostIcon[i].position.x = baseX + rerollSize;
			rerollCostIcon[i].position.y = baseY;
			rerollCostIcon[i].size = { rerollSize / 2.f,rerollSize / 2.f };
			rerollButton[i].Init();
			ItemWindow[i].Init();
		}
		ItemSlotMesh = Utils::CreateNinePatchMesh();
		windowMesh = Utils::CreateNinePatchMesh();

		epicWhite.Mesh = Utils::CreateMesh();
		epicWhite.Texture = AEGfxTextureLoad("Assets/white.png");
		epicWhite.size = { w, h };
		epicWhite.position = { 0.f, 0.f };

		rerollMesh = Utils::CreateMesh();
		rerollTexture = AEGfxTextureLoad("Assets/dice.png");

		rerollCostMesh = Utils::CreateMesh();
		rerollCostTexture = AEGfxTextureLoad("Assets/Coin.png");;

		pauseDimmer.Mesh = Utils::CreateMesh();
		pauseDimmer.Texture = AEGfxTextureLoad("Assets/black.png");
		pauseDimmer.position = { 0, 0 };
		pauseDimmer.size = { static_cast<f32>(global::ScreenWidth), static_cast<f32>(global::ScreenHeight) };
		pFont = AEGfxCreateFont("Assets/Fonts/buggy-font.ttf", fontSize);
		for (auto& cost : rerollCost)
		{
			cost = 20;
		}


		const Playing* game = static_cast<Playing*>(gm.currStateREF);

		for (size_t i = 1; i <= game->ITDB->itemList.size(); i++)
		{
			containerForGrade[game->ITDB->itemList[i]->grade].push_back(game->ITDB->itemList[i]->id);
		}
	}

	void LevelUpUI::Update()
	{
		if (!IsActive() || gameOverScreen.isGameOver)
		{
			return;
		}
		delayTime += global::DeltaTime;
		epicAlpha -= global::DeltaTime/2.f;
		if (delayTime < pickDelay)
		{
			return;
		}
		for (s8 i = 0; i < ItemWindow.size(); i++)
		{
			rerollButton[i].Update();
			if(!rerollButton[i].IsHovered() || !rerollButton[i].IsSelected())
				ItemWindow[i].Update();
		}
	}

	void LevelUpUI::Show()
	{
		if (gameOverScreen.isGameOver)
			return;
		ResetGotEpic();
		currentOptions = GenerateRandomRewards();
		for (int i = 0; i < currentOptions.size(); i++)
		{
			f32 baseX{ ItemWindow[i].position.x }, baseY{ ItemWindow[i].position.y };
			f32 pSizeX{ ItemWindow[i].size.x/2.f }, pSizeY{ ItemWindow[i].size.y/2.f };//p=parents
			f32 cSizeX{ currentOptions[i]->size.x/2.f }, cSizeY{ currentOptions[i]->size.y/2.f };//c=child
			currentOptions[i]->iconPosition = { baseX - pSizeX + cSizeX, baseY + pSizeY - cSizeY - padding*1.5f };
			ItemSlot[i].position = currentOptions[i]->iconPosition;
			ItemSlot[i].size.x = currentOptions[i]->size.x * 0.8f;
			ItemSlot[i].size.y = currentOptions[i]->size.y * 0.8f;

			if (ItemWindow[i].Texture)
			{
				AEGfxTextureUnload(ItemWindow[i].Texture);
				ItemWindow[i].Texture = nullptr;
			}
			switch (currentOptions[i]->grade)
			{
			case COMMON:
				ItemWindow[i].Texture = AEGfxTextureLoad("Assets/ItemSlots/SlotC.png");
				break;
			case RARE:
				ItemWindow[i].Texture = AEGfxTextureLoad("Assets/ItemSlots/SlotR.png");
				break;
			case EPIC:
				ItemWindow[i].Texture = AEGfxTextureLoad("Assets/ItemSlots/SlotE.png");
				gotEpic = true;
				SFXManager.Play("epic");
				break;
			}
		}
		isActive = true;
		gm.Pause();
	}

	void LevelUpUI::Draw()
	{
		if (gameOverScreen.isGameOver)
			return;
		Utils::DrawObject(pauseDimmer, false, 0.5f);
		for (int i = 0; i < ItemWindow.size(); i++)
		{
			Utils::DrawNinePatchMesh(ItemWindow[i], ItemWindow[i].Texture, windowMesh, 50.f);
			Utils::DrawNinePatchMesh(ItemSlot[i], ItemSlot[i].Texture, ItemSlotMesh, 20.f);
			Utils::DrawItem(*currentOptions[i]);
			Utils::DrawObject(rerollButton[i], rerollTexture, rerollMesh, 1.f);
			Utils::DrawObject(rerollCostIcon[i], rerollCostTexture, rerollCostMesh, 1.f);

			f32 lw, lh;
			f32 baseX{ currentOptions[i]->iconPosition.x }, baseY{ currentOptions[i]->iconPosition.y };
			f32 pSizeX{ currentOptions[i]->size.x }, pSizeY{ currentOptions[i]->size.y };//p=parents
			f32 halfW{ global::ScreenWidth / 2.f }, halfH{ global::ScreenHeight / 2.f };
			AEGfxGetPrintSize(pFont, currentOptions[i]->name.c_str(), textDrawSize, &lw, &lh);

			AEGfxPrint(pFont, currentOptions[i]->name.c_str(), (baseX + pSizeX) / halfW, baseY / halfH + (lh / 1.5f), textDrawSize, 1.f, 1.f, 1.f, 1.f);
			f32 CbaseX = rerollCostIcon[i].position.x, CbaseY = rerollCostIcon[i].position.y;
			f32 CpSizeX = rerollCostIcon[i].size.x;
			AEGfxPrint(pFont, std::to_string(rerollCost[i]).c_str(), (CbaseX + CpSizeX * 0.5f) / halfW, CbaseY / halfH - lh, textDrawSize + 0.2f, 1.f, 1.f, 1.f, 1.f);
			const char* tag;
			f32 r{ 0.f }, g{ 0.f }, b{ 0.f };
			switch (currentOptions[i]->tag)
			{
			case InGame::ItemTag::ENVY:
				tag = "ENVY";
				r = 0.5f;
				b = 0.5f;
				break;
			case InGame::ItemTag::GLUTTONY:
				tag = "GLUTTONY";
				g = 0.5f;
				break;
			case InGame::ItemTag::GREED:
				tag = "GREED";
				b = 1.f;
				break;
			case InGame::ItemTag::LUST:
				tag = "LUST";
				b = 0.5f;
				break;
			case InGame::ItemTag::SLOTH:
				tag = "SLOTH";
				r = 1.f;
				g = 1.f;
				break;
			case InGame::ItemTag::WRATH:
				tag = "WRATH";
				r = 1.f;
				g = 0.5f;
				break;
			case InGame::ItemTag::PRIDE:
				tag = "PRIDE";
				r = 1.f;
				break;
			default:
				tag = "NONE";
				break;
			}
			AEGfxPrint(pFont, tag, (baseX + pSizeX) / halfW, baseY / halfH - (lh * 1.5f), textDrawSize, r, g, b, 1.f);
			std::vector<std::string> ItemDesc = HUD.SplitTextIntoLines(currentOptions[i]->description, windowWidth);
			AEVec2 windowPos{ ItemWindow[i].position };

			for (int j = 0; j < ItemDesc.size(); j++)
			{
				f32 xStart{ windowPos.x - windowWidth / 2.f - padding };
				f32 yStart{ currentOptions[i]->iconPosition.y - currentOptions[i]->size.y * 2.f};
				f32 lx = (xStart + padding) / halfW;
				f32 ly = (yStart - (lh*global::ScreenHeight) * j) / halfH;
				AEGfxPrint(pFont, ItemDesc[j].c_str(), lx, ly, textDrawSize, 1.f, 1.f, 1.f, 1.f);
			}
		}
		if(gotEpic)
			Utils::DrawObject(epicWhite, false, epicAlpha);
	}

	void LevelUpUI::Reroll(s8 thisbutton)
	{
		if (PC->PS->Money < rerollCost[thisbutton])
		{
			return;
		}
		PC->PS->Money -= rerollCost[thisbutton];
		rerollCost[thisbutton] = rerollCost[thisbutton] * global::item23RerollCostRatio * global::StageRerollCostRatio[global::CurrentStageNumber - 1];
		std::cout << global::item23RerollCostRatio << std::endl;

		SFXManager.Play("reroll");

		ResetGotEpic();

		const Playing* game = static_cast<Playing*>(gm.currStateREF);

		std::random_device rd;

		std::mt19937 gen(rd());

		//range of item id
		s8 min = 1;
		size_t max = game->ITDB->itemList.size();
		std::uniform_int_distribution<> dis(min, max);
		s8 one{ 0 }, two{ 0 };
		if (thisbutton == 0)
		{
			one = 1, two = 2;
		}
		else if (thisbutton == 1)
		{
			one = 0, two = 2;
		}
		else if (thisbutton == 2)
		{
			one = 0, two = 1;
		}
		s8 idx{ 1 };
		s8 previdx = currentOptions[thisbutton]->id;
		do
		{
			std::unordered_set<int> used_indices;
			std::vector<std::shared_ptr<InGame::Item>> options;
			options.reserve(3);
			ItemGrade currGrade;
			f32 prob = Utils::GetRandomFloat(0.f, 1.f);
			if (0.f <= prob && prob <= 1.f - probEpic - probRare)
				currGrade = COMMON;
			else if (1.f - probEpic - probRare < prob && prob <= 1.f - probEpic)
				currGrade = RARE;
			else if (1.f - probEpic < prob && prob <= 1.f)
				currGrade = EPIC;


			s8 min = 0;

			size_t max = containerForGrade[currGrade].size() - 1;

			std::uniform_int_distribution<> dis(min, max);

			while (true)
			{
				idx = containerForGrade[currGrade][dis(gen)];
				auto result = used_indices.insert(idx);
				if (result.second)
					break;
			}
		} while ((idx == currentOptions[one]->id) || (idx == currentOptions[two]->id) || (idx == previdx));

		AEVec2 originPos = currentOptions[thisbutton]->iconPosition;
		currentOptions[thisbutton] = game->ITDB->itemList[idx];
		currentOptions[thisbutton]->iconPosition = originPos;
		if (ItemWindow[thisbutton].Texture)
		{
			AEGfxTextureUnload(ItemWindow[thisbutton].Texture);
			ItemWindow[thisbutton].Texture = nullptr;
		}
		switch (currentOptions[thisbutton]->grade)
		{
		case COMMON:
			ItemWindow[thisbutton].Texture = AEGfxTextureLoad("Assets/ItemSlots/SlotC.png");
			break;
		case RARE:
			ItemWindow[thisbutton].Texture = AEGfxTextureLoad("Assets/ItemSlots/SlotR.png");
			break;
		case EPIC:
			ItemWindow[thisbutton].Texture = AEGfxTextureLoad("Assets/ItemSlots/SlotE.png");
			gotEpic = true;
			SFXManager.Play("epic");
			break;
		}
	}

	std::vector<std::shared_ptr<InGame::Item>> LevelUpUI::GenerateRandomRewards()
	{
		const Playing* game = static_cast<Playing*>(gm.currStateREF);

		std::random_device rd;

		std::mt19937 gen(rd());

		std::unordered_set<int> used_indices;
		std::vector<std::shared_ptr<InGame::Item>> options;
		options.reserve(3);
		for (s8 i = 0; i < 3; i++)
		{
			ItemGrade currGrade;
			f32 prob = Utils::GetRandomFloat(0.f, 1.f);
			if (0.f <= prob && prob <= 1.f - probEpic - probRare)
				currGrade = COMMON;
			else if(1.f - probEpic - probRare < prob && prob <= 1.f - probEpic)
				currGrade = RARE;
			else if (1.f - probEpic < prob && prob <= 1.f)
				currGrade = EPIC;
			
			int idx;

			s8 min = 0;

			size_t max = containerForGrade[currGrade].size() - 1;

			std::uniform_int_distribution<> dis(min, max);

			while(true)
			{
				idx = containerForGrade[currGrade][dis(gen)];
				auto result = used_indices.insert(idx);
				if (result.second)
					break;
			}
			options.push_back(game->ITDB->itemList[idx]);
		}
		return options;
	}

	bool LevelUpUI::IsActive() const
	{
		return isActive;
	}

	void LevelUpUI::Destroy()
	{
		for (auto mesh : windowMesh)
		{
			if(mesh)
				AEGfxMeshFree(mesh);
		}
		for (auto obj : ItemWindow)
		{
			if (obj.Texture)
			{
				AEGfxTextureUnload(obj.Texture);
				obj.Texture = nullptr;
			}
		}

		AEGfxMeshFree(epicWhite.Mesh);
		AEGfxTextureUnload(epicWhite.Texture);

		AEGfxMeshFree(pauseDimmer.Mesh);
		AEGfxTextureUnload(pauseDimmer.Texture);

		AEGfxMeshFree(rerollMesh);
		AEGfxTextureUnload(rerollTexture);

		AEGfxMeshFree(rerollCostMesh);
		AEGfxTextureUnload(rerollCostTexture);

		for (auto mesh : ItemSlotMesh)
		{
			if (mesh)
				AEGfxMeshFree(mesh);
		}
		for (auto slot : ItemSlot)
		{
			if (slot.Texture)
			{
				AEGfxTextureUnload(slot.Texture);
				slot.Texture = nullptr;
			}
		}
		AEGfxDestroyFont(pFont);
	}

}