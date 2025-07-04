#include "LevelUpUI.h"
#include "HUDController.h"
#include <random>
#include <unordered_set>
namespace Manager
{
	LevelUpUI pickPanel;
	AEGfxVertexList* LevelUpUI::windowMesh{ nullptr };
	AEGfxTexture* LevelUpUI::windowTexture{ nullptr };
	AEGfxVertexList* LevelUpUI::rerollMesh{ nullptr };
	AEGfxTexture* LevelUpUI::rerollTexture{ nullptr };
	const f32 fontSize = 72.f;
	const f32 textDrawSize = 0.2f;
	const f32 windowWidth = 400.f;
	const f32 windowHeight = 600.f;
	const f32 padding = 20.f;
	void LevelUpUI::Init(InGame::PlayerCharacter* InPC)
	{
		currentOptions.reserve(3);
		PC = InPC;
		const f32 spacingX = 20.0f;

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
					this->isActive = false; gm.Resume();
					for (auto& cost : rerollCost)
					{
						cost = 100; // 리롤 코스트 초기화 부분!! 여기에다가 변경될 기본 코스트 계산 값 넣으면 될 듯 여기는 아이템 선택 시점에 호출되는 부분이고 초기화 하는 곳은 아래에 있음
					}
				});
			f32 winHalfW{ windowWidth / 2.f }, winHalfH{ windowHeight / 2.f };
			rerollButton[i].position = ItemWindow[i].position;
			rerollButton[i].position.x = ItemWindow[i].position.x - winHalfW + (rerollSize / 2.f) + padding;
			rerollButton[i].position.y = ItemWindow[i].position.y - winHalfH + (rerollSize / 2.f) + padding;
			rerollButton[i].size = { rerollSize, rerollSize };
			rerollButton[i].SetCallback([this, i]() {Reroll(i); });
		}
		windowMesh = Utils::CreateMesh();
		windowTexture = AEGfxTextureLoad("Assets/SelectItem_LevelUp.png");
		rerollMesh = Utils::CreateMesh();
		rerollTexture = AEGfxTextureLoad("Assets/dice.png");
		pauseDimmer.Mesh = Utils::CreateMesh();
		pauseDimmer.Texture = AEGfxTextureLoad("Assets/black.png");
		pauseDimmer.position = { 0, 0 };
		pauseDimmer.size = { static_cast<f32>(global::ScreenWidth), static_cast<f32>(global::ScreenHeight) };
		pFont = AEGfxCreateFont("Assets/buggy-font.ttf", fontSize);
		for (auto& cost : rerollCost)
		{
			cost = 100; // 여기도
		}
	}

	void LevelUpUI::Update()
	{
		if (!IsActive())
		{
			return;
		}
		for (s8 i = 0; i < ItemWindow.size(); i++)
		{
			if (ItemWindow[i].IsClicked() && !rerollButton[i].IsHovered())
			{
				ItemWindow[i].OnClick();
				std::cout << "choosed item: " << currentOptions[i]->name << std::endl;
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
			currentOptions[i]->iconPosition = { baseX - pSizeX + cSizeX + padding, baseY + pSizeY - cSizeY - padding };
		}
		isActive = true;
		gm.Pause();
	}

	void LevelUpUI::Draw()
	{
		Utils::DrawObject(pauseDimmer, false, 0.5f);
		for (int i = 0; i < ItemWindow.size(); i++)
		{
			Utils::DrawObject(ItemWindow[i], windowTexture, windowMesh, 1.f);
			Utils::DrawObject(rerollButton[i], rerollTexture, rerollMesh, 1.f);
			Utils::DrawItem(*currentOptions[i]);

			f32 lw, lh;
			f32 baseX{ currentOptions[i]->iconPosition.x }, baseY{ currentOptions[i]->iconPosition.y };
			f32 pSizeX{ currentOptions[i]->size.x * 0.8f }, pSizeY{ currentOptions[i]->size.y };//p=parents
			f32 halfW{ global::ScreenWidth / 2.f }, halfH{ global::ScreenHeight / 2.f};
			AEGfxGetPrintSize(pFont, currentOptions[i]->name.c_str(), textDrawSize, &lw, &lh);
			AEGfxPrint(pFont, currentOptions[i]->name.c_str(), (baseX + pSizeX) / halfW, baseY / halfH + (lh / 1.5f), textDrawSize, 1.f, 1.f, 1.f, 1.f);
			const char* tag;
			switch (currentOptions[i]->tag)
			{
			case InGame::ItemTag::ENVY:
				tag = "ENVY";
				break;
			case InGame::ItemTag::GLUTTONY:
				tag = "GLUTTONY";
				break;
			case InGame::ItemTag::GREED:
				tag = "GREED";
				break;
			case InGame::ItemTag::LUST:
				tag = "LUST";
				break;
			case InGame::ItemTag::SLOTH:
				tag = "SLOTH";
				break;
			case InGame::ItemTag::WRATH:
				tag = "WRATH";
				break;
			case InGame::ItemTag::PRIDE:
				tag = "PRIDE";
				break;
			default:
				tag = "NONE";
				break;
			}
			f32 r{ 0.5f }, g{ 0.5f }, b{ 0.5f };
			AEGfxPrint(pFont, tag, (baseX + pSizeX) / halfW, baseY / halfH - (lh * 1.3f), textDrawSize, r, g, b, 1.f);
			std::vector<std::string> ItemDesc = HUD.SplitTextIntoLines(currentOptions[i]->description, windowWidth - padding);
			AEVec2 windowPos{ ItemWindow[i].position };

			for (int j = 0; j < ItemDesc.size(); j++)
			{
				f32 xStart{ windowPos.x - windowWidth / 2.f };
				f32 yStart{ currentOptions[i]->iconPosition.y - currentOptions[i]->size.y };
				f32 lx = (xStart + padding) / halfW;
				f32 ly = (yStart - (lh*global::ScreenHeight) * j) / halfH;
				AEGfxPrint(pFont, ItemDesc[j].c_str(), lx, ly, textDrawSize, 1.f, 1.f, 1.f, 1.f);
			}
		}
	}

	void LevelUpUI::Reroll(s8 thisbutton)
	{
		// implement here!!!
		rerollCost[thisbutton] = rerollCost[thisbutton] * global::item23RerollCostRatio * global::StageRerollCostRatio[global::CurrentStageNumber - 1];
		
		if (PC->Stats.Money < rerollCost[thisbutton])
		{
			return;
		}
		PC->Stats.Money -= rerollCost[thisbutton];

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
		rerollCost[thisbutton] *= 1.7f;
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
		AEGfxMeshFree(windowMesh);
		AEGfxTextureUnload(windowTexture);

		AEGfxMeshFree(pauseDimmer.Mesh);
		AEGfxTextureUnload(pauseDimmer.Texture);

		AEGfxMeshFree(rerollMesh);
		AEGfxTextureUnload(rerollTexture);

		AEGfxDestroyFont(pFont);
	}

}