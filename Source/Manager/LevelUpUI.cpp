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

	void LevelUpUI::Init(InGame::PlayerCharacter* InPC)
	{
		currentOptions.reserve(3);
		PC = InPC;
		const f32 windowWidth = 200.f;
		const f32 windowHeight = 500.f;
		const f32 spacingX = 50.0f; // 가로 간격
		const f32 spacingY = 50.0f; // 세로 간격
		const f32 startX = -250.f; // 전체 위치 조정
		const f32 startY = -600.f;
		const f32 rerollSize = 60.f;
		for (s8 i = 0; i < ItemWindow.size(); ++i)
		{
			s8 row = i;
			s8 col = i;

			ItemWindow[i].position = {
				startX + col * (windowWidth + spacingX),
				startY + (windowHeight + spacingY)
			};

			ItemWindow[i].size = { windowWidth, windowHeight };
			ItemWindow[i].SetCallback([this, i]() {PC->AddItemToInventory(currentOptions[i]->Clone()); this->isActive = false; gm.Resume(); });
			rerollButton[i].position = ItemWindow[i].position;
			rerollButton[i].position.y -= windowHeight / 2;
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
		rerollCost = 100;
	}

	void LevelUpUI::Update()
	{
		if (!IsActive())
		{
			return;
		}
		for (s8 i = 0; i < ItemWindow.size(); i++)
		{

			ItemSlot[i].position = currentOptions[i]->iconPosition;
			ItemSlot[i].size = currentOptions[i]->size;
			if (ItemWindow[i].IsClicked() && !rerollButton[i].IsHovered())
			{
				ItemWindow[i].OnClick();
				std::cout << "choosed item: " << currentOptions[i]->name << std::endl;
			}
			if (rerollButton[i].IsClicked())
			{
				rerollButton[i].OnClick();
			}
			if (ItemSlot[i].IsHovered())
			{
				HUD.TooltipUpdate(*currentOptions[i]);
			}
		}
	}

	void LevelUpUI::Draw()
	{
		Utils::DrawObject(pauseDimmer, false, 0.5f);
		for (int i = 0; i < ItemWindow.size(); i++)
		{
			Utils::DrawObject(ItemWindow[i], windowTexture, windowMesh, 1.f);
			Utils::DrawObject(rerollButton[i], rerollTexture, rerollMesh, 1.f);
			Utils::DrawItem(*currentOptions[i]);
		}
		for (int i = 0; i < ItemWindow.size(); i++)
		{
			if (ItemSlot[i].IsHovered())
			{
				HUD.ShowTooltip(*currentOptions[i]);
			}
		}
	}

	void LevelUpUI::Show()
	{
		currentOptions = GenerateRandomRewards();
		for (int i = 0; i < currentOptions.size(); i++)
		{
			currentOptions[i]->iconPosition = ItemWindow[i].position;
		}
		isActive = true;
		gm.Pause();
	}
	
	void LevelUpUI::Reroll(s8 thisbutton)
	{
		if (PC->Stats.Money < rerollCost)
		{
			return;
		}
		else
		{
			PC->Stats.Money -= rerollCost;
		}
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

		currentOptions[thisbutton] = game->ITDB->itemList[idx];
		currentOptions[thisbutton]->iconPosition = ItemWindow[thisbutton].position;
		std::cout << "Rerolled!!" << std::endl;
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
	}

}