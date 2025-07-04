#include "LevelUpUI.h"
#include "HUDController.h"
#include <random>
#include <unordered_set>
namespace Manager
{
	LevelUpUI pickPanel;
	AEGfxTexture* LevelUpUI::windowTexture{ nullptr };
	AEGfxVertexList* LevelUpUI::rerollMesh{ nullptr };
	AEGfxTexture* LevelUpUI::rerollTexture{ nullptr };
	AEGfxTexture* LevelUpUI::ItemSlotTexture{ nullptr };
	const f32 fontSize = 72.f;
	const f32 textDrawSize = 0.2f;
	const f32 windowWidth = 400.f;
	const f32 windowHeight = 600.f;
	const f32 spacingX = 80.0f;
	const f32 padding = 30.f;
	void LevelUpUI::Init(InGame::PlayerCharacter* InPC)
	{
		currentOptions.reserve(3);
		PC = InPC;

		f32 totalWidth = 3 * windowWidth + 2 * spacingX;
		f32 margin = (global::ScreenWidth - totalWidth) * 0.5f;
		f32 startX = -global::ScreenWidth * 0.5f + margin + windowWidth * 0.5f;
		const f32 startY = 0;
		f32 rerollSize = 100.f;
		for (s8 i = 0; i < ItemWindow.size(); ++i)
		{
			ItemWindow[i].position = {
				startX + i * (windowWidth + spacingX),
				startY
			};

			ItemWindow[i].size = { windowWidth, windowHeight };
			ItemWindow[i].SetCallback([this, i]() 
				{
					PC->AddItemToInventory(currentOptions[i]->Clone());
					this->isActive = false;
					for (auto& cost : rerollCost)
					{
						cost = 100;
					}
					gm.Resume();
				});
			f32 winHalfW{ windowWidth / 2.f }, winHalfH{ windowHeight / 2.f };
			rerollButton[i].position.x = ItemWindow[i].position.x - winHalfW + (rerollSize / 2.f) + padding;
			rerollButton[i].position.y = ItemWindow[i].position.y - winHalfH + (rerollSize / 2.f) + padding;
			rerollButton[i].size = { rerollSize, rerollSize };
			rerollButton[i].SetCallback([this, i]() {Reroll(i); });
			f32 baseX{ rerollButton[i].position.x }, baseY{ rerollButton[i].position.y };
			rerollCostIcon[i].position.x = baseX + rerollSize;
			rerollCostIcon[i].position.y = baseY;
			rerollCostIcon[i].size = { rerollSize / 2.f,rerollSize / 2.f };
		}
		windowMesh = Utils::CreateNinePatchMesh();
		windowTexture = AEGfxTextureLoad("Assets/SelectItem_LevelUp.png");

		ItemSlotMesh = Utils::CreateNinePatchMesh();
		ItemSlotTexture = AEGfxTextureLoad("Assets/ItemSlotTexture.png"); 

		rerollMesh = Utils::CreateMesh();
		rerollTexture = AEGfxTextureLoad("Assets/dice.png");

		rerollCostMesh = Utils::CreateMesh();
		rerollCostTexture = AEGfxTextureLoad("Assets/Coin.png");;

		pauseDimmer.Mesh = Utils::CreateMesh();
		pauseDimmer.Texture = AEGfxTextureLoad("Assets/black.png");
		pauseDimmer.position = { 0, 0 };
		pauseDimmer.size = { static_cast<f32>(global::ScreenWidth), static_cast<f32>(global::ScreenHeight) };
		pFont = AEGfxCreateFont("Assets/buggy-font.ttf", fontSize);
		for (auto& cost : rerollCost)
		{
			cost = 100;
		}
	}

	void LevelUpUI::Update()
	{
		if (!IsActive())
		{
			return;
		}
		delayTime += global::DeltaTime;
		if (delayTime < pickDelay)
		{
			return;
		}
		for (s8 i = 0; i < ItemWindow.size(); i++)
		{
			if (ItemWindow[i].IsClicked() && !rerollButton[i].IsHovered())
			{
				ItemWindow[i].OnClick();
				delayTime = 0.f;
			}
			if (rerollButton[i].IsClicked())
			{
				rerollButton[i].OnClick();

			}
		}
	}

	void LevelUpUI::Show()
	{
		currentOptions = GenerateRandomRewards();
		for (int i = 0; i < currentOptions.size(); i++)
		{
			f32 baseX{ ItemWindow[i].position.x }, baseY{ ItemWindow[i].position.y };
			f32 pSizeX{ ItemWindow[i].size.x/2.f }, pSizeY{ ItemWindow[i].size.y/2.f };//p=parents
			f32 cSizeX{ currentOptions[i]->size.x/2.f }, cSizeY{ currentOptions[i]->size.y/2.f };//c=child
			currentOptions[i]->iconPosition = { baseX - pSizeX + cSizeX + padding, baseY + pSizeY - cSizeY - padding*2.f };
			ItemSlot[i].position = currentOptions[i]->iconPosition;
			ItemSlot[i].size.x = currentOptions[i]->size.x * 0.8f;
			ItemSlot[i].size.y = currentOptions[i]->size.y * 0.8f;
		}
		isActive = true;
		gm.Pause();
	}

	void LevelUpUI::Draw()
	{
		Utils::DrawObject(pauseDimmer, false, 0.5f);
		for (int i = 0; i < ItemWindow.size(); i++)
		{
			Utils::DrawNinePatchMesh(ItemWindow[i], windowTexture, windowMesh, 30.f);
			Utils::DrawNinePatchMesh(ItemSlot[i], ItemSlotTexture, ItemSlotMesh, 20.f);
			Utils::DrawItem(*currentOptions[i]);
			Utils::DrawObject(rerollButton[i], rerollTexture, rerollMesh, 1.f);
			Utils::DrawObject(rerollCostIcon[i], rerollCostTexture, rerollCostMesh, 1.f);

			f32 lw, lh;
			f32 baseX{ currentOptions[i]->iconPosition.x }, baseY{ currentOptions[i]->iconPosition.y };
			f32 pSizeX{ currentOptions[i]->size.x }, pSizeY{ currentOptions[i]->size.y };//p=parents
			f32 halfW{ global::ScreenWidth / 2.f }, halfH{ global::ScreenHeight / 2.f };
			AEGfxGetPrintSize(pFont, currentOptions[i]->name.c_str(), textDrawSize, &lw, &lh);

			AEGfxPrint(pFont, currentOptions[i]->name.c_str(), (baseX + pSizeX) / halfW, baseY / halfH + (lh / 1.5f), textDrawSize + 0.1f, 1.f, 1.f, 1.f, 1.f);
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
			std::vector<std::string> ItemDesc = HUD.SplitTextIntoLines(currentOptions[i]->description, windowWidth - padding*1.5f);
			AEVec2 windowPos{ ItemWindow[i].position };

			for (int j = 0; j < ItemDesc.size(); j++)
			{
				f32 xStart{ windowPos.x - windowWidth / 2.f };
				f32 yStart{ currentOptions[i]->iconPosition.y - currentOptions[i]->size.y * 2.f};
				f32 lx = (xStart + padding) / halfW;
				f32 ly = (yStart - (lh*global::ScreenHeight) * j) / halfH;
				AEGfxPrint(pFont, ItemDesc[j].c_str(), lx, ly, textDrawSize, 1.f, 1.f, 1.f, 1.f);
			}
		}
	}

	void LevelUpUI::Reroll(s8 thisbutton)
	{
		rerollCost[thisbutton] = rerollCost[thisbutton] * global::item23RerollCostRatio * global::StageRerollCostRatio[global::CurrentStageNumber - 1];
		
		if (PC->Stats.Money < rerollCost[thisbutton])
		{
			return;
		}
		PC->Stats.Money -= rerollCost[thisbutton];

		rerollCost[thisbutton] *= 1.7f;

		std::shared_ptr<InGame::Item> option;

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
			idx = dis(gen);
		} while ((idx == currentOptions[one]->id) || (idx == currentOptions[two]->id) || (idx == previdx));
		AEVec2 originPos = currentOptions[thisbutton]->iconPosition;
		currentOptions[thisbutton] = game->ITDB->itemList[idx];
		currentOptions[thisbutton]->iconPosition = originPos;
	}

	std::vector<std::shared_ptr<InGame::Item>> LevelUpUI::GenerateRandomRewards()
	{
		const Playing* game = static_cast<Playing*>(gm.currStateREF);

		std::random_device rd;

		std::mt19937 gen(rd());

		//range of item id
		s8 min = 1;

		size_t max = game->ITDB->itemList.size();

		std::uniform_int_distribution<> dis(min, max);

		std::unordered_set<int> used_indices;
		std::vector<std::shared_ptr<InGame::Item>> options;
		options.reserve(3);
		for (s8 i = 0; i < 3; i++)
		{
			int idx;
			while(true)
			{
				idx = dis(gen);
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
			AEGfxMeshFree(mesh);
		}
		AEGfxTextureUnload(windowTexture);

		AEGfxMeshFree(pauseDimmer.Mesh);
		AEGfxTextureUnload(pauseDimmer.Texture);

		AEGfxMeshFree(rerollMesh);
		AEGfxTextureUnload(rerollTexture);

		AEGfxMeshFree(rerollCostMesh);
		AEGfxTextureUnload(rerollCostTexture);

		for (auto mesh : ItemSlotMesh)
		{
			if (mesh)
			{
				AEGfxMeshFree(mesh);
			}
		}
		AEGfxTextureUnload(ItemSlotTexture);
		AEGfxDestroyFont(pFont);
	}

}