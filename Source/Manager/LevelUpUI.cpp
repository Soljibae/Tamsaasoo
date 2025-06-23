#include "LevelUpUI.h"
#include <random>
#include <unordered_set>
namespace Manager
{
	LevelUpUI pickPanel;
	AEGfxVertexList* LevelUpUI::windowMesh{ nullptr };
	AEGfxTexture* LevelUpUI::windowTexture{ nullptr };

	void LevelUpUI::Init(InGame::PlayerCharacter* InPC)
	{
		currentOptions.reserve(3);
		PC = InPC;
		const float windowWidth = 200.f;
		const float windowHeight = 500.f;
		const float spacingX = 50.0f; // 가로 간격
		const float spacingY = 50.0f; // 세로 간격
		const float startX = -250.f; // 전체 위치 조정
		const float startY = -600.f;
		for (int i = 0; i < ItemWindow.size(); ++i)
		{
			int row = static_cast<int>(i);
			int col = static_cast<int>(i);

			ItemWindow[i].position = {
				startX + col * (windowWidth + spacingX),
				startY + (windowHeight + spacingY)
			};

			ItemWindow[i].size = { windowWidth, windowHeight };
			ItemWindow[i].SetCallback([this, i]() {PC->AddItemToInventory(currentOptions[i]->Clone()); this->isActive = false; gm.Resume(); });
		}
		//todo: load mesh, texture
		windowMesh = Utils::CreateMesh();
		windowTexture = AEGfxTextureLoad("Assets/SelectItem_LevelUp.png");
	}

	void LevelUpUI::Update()
	{
		if (!IsActive())
		{
			return;
		}
		for (size_t i = 0; i < ItemWindow.size(); i++)
		{
			if (ItemWindow[i].IsClicked())
			{
				ItemWindow[i].OnClick();
				std::cout << "choosed item: " << currentOptions[i]->name << std::endl;
			}
		}
	}

	void LevelUpUI::Draw()
	{
		for (int i = 0; i < ItemWindow.size(); i++)
		{
			Utils::DrawObject(ItemWindow[i], windowTexture, windowMesh, 1.f);
			Utils::DrawItem(*currentOptions[i]);
		}
	}

	void LevelUpUI::Show(const std::vector<std::shared_ptr<InGame::Item>>& options)
	{
		currentOptions = options;
		for (int i = 0; i < currentOptions.size(); i++)
		{
			currentOptions[i]->iconPosition = ItemWindow[i].position;
		}
		isActive = true;
		gm.Pause();
	}

	std::vector<std::shared_ptr<InGame::Item>> LevelUpUI::GenerateRandomRewards()
	{
		const Playing* game = static_cast<Playing*>(gm.currStateREF);

		std::random_device rd;

		std::mt19937 gen(rd());

		//range of item id
		size_t min = 1;

		size_t max = game->ITDB->itemList.size();

		std::uniform_int_distribution<> dis(min, max);

		std::unordered_set<int> used_indices;
		std::vector<std::shared_ptr<InGame::Item>> options;
		options.reserve(3);
		for (int i = 0; i < 3; i++)
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
	}

}