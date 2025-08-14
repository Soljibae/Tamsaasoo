#include "LevelUpUI.h"
#include "HUDController.h"
#include "GameOver.h"
#include <random>
#include <unordered_set>
#include <cstring>

namespace Manager
{
	LevelUpUI pickPanel;
	AEGfxTexture* LevelUpUI::windowTexture{ nullptr };
	//std::array<AEGfxVertexList*, 9> LevelUpUI::rerollButtonMesh{ nullptr };
	AEGfxTexture* LevelUpUI::rerollButtonTexture{ nullptr };
	AEGfxTexture* LevelUpUI::ItemSlotTexture{ nullptr };
	const static f32 fontSize = 72.f;
	const static f32 textDrawSize = 0.2f;
	const static f32 windowWidth = 350.f;
	const static f32 windowHeight = 550.f;
	const static f32 spacingX = 150.f;
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
		isActive = false;
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/pick.wav", "pick");
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/reroll.wav", "reroll");
		SFXManager.AddNewSFX(InGame::UI, "Assets/SFX/UI/epic.wav", "epic");
		currentOptions.reserve(3);
		PC = InPC;

		f32 totalWidth = 3 * windowWidth + 2 * spacingX;
		f32 margin = (global::ScreenWidth - totalWidth) * 0.5f;
		f32 startX = -global::ScreenWidth * 0.5f + margin + windowWidth * 0.5f;
		f32 rerollSizeX = windowWidth * 0.5f;
		f32 rerollSizeY = rerollSizeX * 0.4f;
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
			rerollButton[i].position.x = ItemWindow[i].position.x;
			rerollButton[i].position.y = ItemWindow[i].position.y - winHalfH + padding;
			rerollButton[i].size = { rerollSizeX, rerollSizeY };
			rerollButton[i].SetCallback([this, i]() {Reroll(i); });

			f32 baseX{ rerollButton[i].position.x }, baseY{ rerollButton[i].position.y };
			rerollIcon[i].size = { rerollSizeY / 1.7f, rerollSizeY / 1.7f };
			rerollIcon[i].position.x = baseX + rerollButton[i].size.x/2.f - rerollIcon[i].size.x/1.18f;
			rerollIcon[i].position.y = baseY + 2.f;

			rerollCostIcon[i].size = { rerollSizeY / 1.5f * 0.6f, rerollSizeY / 1.5f * 0.6f };
			rerollCostIcon[i].position.x = baseX - rerollCostIcon[i].size.x * 2.f;
			rerollCostIcon[i].position.y = baseY;
			rerollButton[i].Init();
			ItemWindow[i].Init();
		}
		ItemSlotMesh = Utils::CreateNinePatchMesh();
		windowMesh = Utils::CreateNinePatchMesh();

		epicWhite.Mesh = Utils::CreateMesh();
		epicWhite.Texture = AEGfxTextureLoad("Assets/white.png");
		epicWhite.size = { w, h };
		epicWhite.position = { 0.f, 0.f };

		//rerollButtonMesh = Utils::CreateNinePatchMesh();
		rerollButtonTexture = AEGfxTextureLoad("Assets/UI/rerollButton.png");

		rerollIconMesh = Utils::CreateMesh();
		rerollIconTexture = AEGfxTextureLoad("Assets/UI/reroll.png");

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
		static  std::array<bool, 3> iconScaled{};
		if (!IsActive() || gameOverScreen.isGameOver)
		{
			return;
		}
		delayTime += global::DeltaTime;
		epicAlpha -= global::DeltaTime / 2.f;
		if (delayTime < pickDelay)
		{
			return;
		}
		for (s8 i = 0; i < ItemWindow.size(); i++)
		{
			rerollButton[i].Update();

			if (rerollButton[i].IsHovered() && !iconScaled[i])
			{
				rerollIcon[i].size.x *= 1.04f;
				rerollIcon[i].size.y *= 1.04f;
				rerollCostIcon[i].size.x *= 1.04f;
				rerollCostIcon[i].size.y *= 1.04f;
				costTextDrawSize[i] *= 1.04f;
				iconScaled[i] = true;
			}
			else if (!rerollButton[i].IsHovered() && iconScaled[i])
			{
				rerollIcon[i].size.x /= 1.04f;
				rerollIcon[i].size.y /= 1.04f;
				rerollCostIcon[i].size.x /= 1.04f;
				rerollCostIcon[i].size.y /= 1.04f;
				costTextDrawSize[i] /= 1.04f;
				iconScaled[i] = false;
			}
			if (!rerollButton[i].IsHovered() || !rerollButton[i].IsSelected())
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
			f32 pSizeX{ ItemWindow[i].size.x / 2.f }, pSizeY{ ItemWindow[i].size.y / 2.f };//p=parents
			f32 cSizeX{ currentOptions[i]->size.x / 2.f }, cSizeY{ currentOptions[i]->size.y / 2.f };//c=child
			currentOptions[i]->iconPosition = { baseX - pSizeX + cSizeX, baseY + pSizeY - cSizeY - padding * 1.5f };
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
			//Utils::DrawNinePatchMesh(rerollButton[i], rerollButtonTexture, rerollButtonMesh, 20.f);
			Utils::DrawObject(rerollButton[i], rerollButtonTexture, rerollCostMesh);
			Utils::DrawObject(rerollIcon[i], rerollIconTexture, rerollIconMesh, 1.f);
			Utils::DrawObject(rerollCostIcon[i], rerollCostTexture, rerollCostMesh, 1.f);

			f32 lw, lh;
			f32 baseX{ currentOptions[i]->iconPosition.x }, baseY{ currentOptions[i]->iconPosition.y };
			f32 pSizeX{ currentOptions[i]->size.x }, pSizeY{ currentOptions[i]->size.y };//p=parents
			f32 halfW{ global::ScreenWidth / 2.f }, halfH{ global::ScreenHeight / 2.f };
			f32 downForName = 5.f;
			AEGfxGetPrintSize(pFont, currentOptions[i]->name.c_str(), textDrawSize, &lw, &lh);
			//AEGfxPrint(pFont, currentOptions[i]->name.c_str(), (baseX + pSizeX) / halfW, baseY / halfH + (lh / 1.5f), textDrawSize, 1.f, 1.f, 1.f, 1.f);
			
			Manager::Atlas.RenderTextUTF8(currentOptions[i]->name.c_str(), (baseX + pSizeX), baseY + Manager::Atlas.GetPrintMetricsUTF8(currentOptions[i]->name.c_str(), 1.f).height * 1.5f - downForName, 1.f, 0xFFFFFFFF);

			f32 CbaseX = rerollCostIcon[i].position.x, CbaseY = rerollCostIcon[i].position.y;
			f32 CpSizeX = rerollCostIcon[i].size.x;
			AEGfxPrint(pFont, std::to_string(rerollCost[i]).c_str(), (CbaseX + CpSizeX * 0.7f) / halfW, CbaseY / halfH - lh, costTextDrawSize[i], 1.f, 1.f, 1.f, 1.f);
			const char* tag;
			u32 col = 0xFFFFFFFF;
			switch (currentOptions[i]->tag)
			{
			case InGame::ItemTag::ENVY:
				tag = "ENVY";
				col = 0x800080FF;
				break;
			case InGame::ItemTag::GLUTTONY:
				tag = "GLUTTONY";
				col = 0x008000FF;
				break;
			case InGame::ItemTag::GREED:
				tag = "GREED";
				col = 0x0000FFFF;
				break;
			case InGame::ItemTag::LUST:
				tag = "LUST";
				col = 0x0D00A6FF;
				break;
			case InGame::ItemTag::SLOTH:
				tag = "SLOTH";
				col = 0xFFFF00FF;
				break;
			case InGame::ItemTag::WRATH:
				tag = "WRATH";
				col = 0xFF8000FF;
				break;
			case InGame::ItemTag::PRIDE:
				tag = "PRIDE";
				col = 0xFF0000FF;
				break;
			default:
				tag = "NONE";
				col = 0x555555FF;
				break;
			}
			AEMtx33 I; AEMtx33Identity(&I);
			AEGfxSetTransform(I.m);
			AEGfxSetRenderMode(AE_GFX_RM_TEXTURE);
			AEGfxSetBlendMode(AE_GFX_BM_BLEND);

			//AEGfxPrint(pFont, tag, (baseX + pSizeX) / halfW, baseY / halfH - (lh * 1.5f), textDrawSize, r, g, b, 1.f);
			//std::vector<std::string> ItemDesc = HUD.SplitTextIntoLines(currentOptions[i]->description, windowWidth);
			Manager::Atlas.RenderTextUTF8(tag, (baseX + pSizeX), baseY, 1.f, col);

			f32 paddingForKR = -10.f;
			f32 upForKR = 30.f;
			std::vector<std::string> ItemDesc =
				HUD.SplitTextIntoLines_UTF8_KR(currentOptions[i]->description, windowWidth - paddingForKR * 2.f, 1.f);

			auto mBase = Manager::Atlas.GetPrintMetricsUTF8(global::stringForKRGap, 1.f);
			float lineH = mBase.lineHeight;
			float asc = mBase.ascender;

			AEVec2 windowPos{ ItemWindow[i].position };
			float xLeft = windowPos.x - windowWidth * 0.5f + paddingForKR;
			float yTop = currentOptions[i]->iconPosition.y - currentOptions[i]->size.y * 2.f + upForKR;

			float baseYFirst = yTop - asc;

			for (size_t j = 0; j < ItemDesc.size(); ++j)
			{
				float lx = xLeft;
				float ly = baseYFirst - lineH * static_cast<float>(j);
				Manager::Atlas.RenderTextUTF8(ItemDesc[j], lx, ly, 1.f, 0xFFFFFFFF);
			}
		}
		if (gotEpic)
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
		rerollCost[thisbutton] = std::clamp(rerollCost[thisbutton], 0, 999);

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
			else if (1.f - probEpic - probRare < prob && prob <= 1.f - probEpic)
				currGrade = RARE;
			else if (1.f - probEpic < prob && prob <= 1.f)
				currGrade = EPIC;

			int idx;

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
		for (auto& mesh : windowMesh)
		{
			if (mesh)
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

		if (epicWhite.Mesh)
			AEGfxMeshFree(epicWhite.Mesh);
		if (epicWhite.Texture)
			AEGfxTextureUnload(epicWhite.Texture);

		if (pauseDimmer.Mesh)
			AEGfxMeshFree(pauseDimmer.Mesh);
		if (pauseDimmer.Texture)
			AEGfxTextureUnload(pauseDimmer.Texture);

		//for (auto& mesh : rerollButtonMesh)
		//{
		//	if (mesh)
		//		AEGfxMeshFree(mesh);
		//}
		if (rerollButtonTexture)
			AEGfxTextureUnload(rerollButtonTexture);

		if(rerollIconMesh)
			AEGfxMeshFree(rerollIconMesh);
		if (rerollIconTexture)
			AEGfxTextureUnload(rerollIconTexture);

		if (rerollCostMesh)
			AEGfxMeshFree(rerollCostMesh);
		if (rerollCostTexture)
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