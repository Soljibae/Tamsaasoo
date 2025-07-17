#include "SettingUI.h"
#include "../Utils/Utils.h"
#include "../InGame/SFX.h"
#include "GameManager.h"
#include <algorithm>

namespace Manager
{
	SettingUI SettingPanel;

	AEGfxVertexList* SettingUI::soundBarBackgroundMesh;
	AEGfxTexture* SettingUI::soundBarBackgroundTexture;
	AEGfxVertexList* SettingUI::soundBarFillMesh;
	AEGfxTexture* SettingUI::soundBarFillTexture;
	AEGfxVertexList* SettingUI::soundBarButtonMesh;
	AEGfxTexture* SettingUI::soundBarButtonTexture;

	AEGfxVertexList* SettingUI::checkBoxMesh;
	AEGfxTexture* SettingUI::checkBoxTexture;
	AEGfxVertexList* SettingUI::checkSignMesh;
	AEGfxTexture* SettingUI::checkSignTexture;

	f32 paddingForText = 25.f;

	void SettingUI::Init()
	{
		f32 padding = 4.f;

		pFont = AEGfxCreateFont("Assets/Fonts/buggy-font.ttf", fontSize);

		UI.size = {300.f, 500.f};
		UI.position = { 0.f, 0.f };

		UI.Texture = AEGfxTextureLoad("Assets/black.png");
		UI.Mesh = Utils::CreateMesh();

		sfxBarFrame.position = { 0.f,170.f };
		sfxBarFrame.size = { 80.f * 3.f, 9.f * 2.f };

		sfxBarBackground.position = { 0.f, sfxBarFrame.position.y };
		sfxBarBackground.size = { sfxBarFrame.size.x - padding, sfxBarFrame.size.y - padding };

		sfxBarFill.position = { 0.f,sfxBarFrame.position.y };
		sfxBarFill.size = { sfxBarFrame.size.x - padding, sfxBarFrame.size.y - padding };

		bgmBarFrame.position = { 0.f,70.f };
		bgmBarFrame.size = { 80.f * 3.f, 9.f * 2.f };

		bgmBarBackground.position = { 0.f, bgmBarFrame.position.y };
		bgmBarBackground.size = { bgmBarFrame.size.x - padding, bgmBarFrame.size.y - padding };

		bgmBarFill.position = { 0.f,bgmBarFrame.position.y };
		bgmBarFill.size = { bgmBarFrame.size.x - padding, bgmBarFrame.size.y - padding };

		uiBarFrame.position = { 0.f,-30.f };
		uiBarFrame.size = { 80.f * 3.f, 9.f * 2.f };

		uiBarBackground.position = { 0.f, uiBarFrame.position.y };
		uiBarBackground.size = { uiBarFrame.size.x - padding, uiBarFrame.size.y - padding };

		uiBarFill.position = { 0.f,uiBarFrame.position.y };
		uiBarFill.size = { uiBarFrame.size.x - padding, uiBarFrame.size.y - padding };

		border.position = { 0.f, -100.f };
		border.size = { 250.f, 4.f };

		checkBox.position = { 80.f, -160.f };
		checkBox.size = { 24.f, 24.f };

		checkSign.position = { 80.f, -160.f };
		checkSign.size = { 24.f, 24.f };

		AEAudioSetGroupVolume(SFXManager.sound_group[InGame::SFX], sfxBarFill.size.x / sfxBarBackground.size.x * SFXManager.SFXOriginVol);
		AEAudioSetGroupVolume(SFXManager.sound_group[InGame::BGM], bgmBarFill.size.x / bgmBarBackground.size.x * SFXManager.BGMOriginVol);
		AEAudioSetGroupVolume(SFXManager.sound_group[InGame::UI], uiBarFill.size.x / uiBarBackground.size.x * SFXManager.UIOriginVol);

		isButtonPressed[InGame::SFX] = false;
		isButtonPressed[InGame::BGM] = false;
		isButtonPressed[InGame::UI] = false;
	}

	void SettingUI::StaticInit()
	{
		soundBarBackgroundMesh = Utils::CreateMesh();
		soundBarBackgroundTexture = AEGfxTextureLoad("Assets/black.png");
		soundBarFillMesh = Utils::CreateMesh();
		soundBarFillTexture = AEGfxTextureLoad("Assets/white.png");
		soundBarButtonMesh = Utils::CreateMesh();
		soundBarButtonTexture = AEGfxTextureLoad("Assets/UI/soundbarButton.png");
		checkBoxMesh = Utils::CreateMesh();
		checkBoxTexture = AEGfxTextureLoad("Assets/UI/checkbox.png");
		checkSignMesh = Utils::CreateMesh();
		checkSignTexture = AEGfxTextureLoad("Assets/UI/checksign.png");
	}

	void SettingUI::Update()
	{
		if (SettingPanel.isSettingOn)
		{
			if (AEInputCheckTriggered(AEVK_ESCAPE))
				SettingPanel.isSettingOn = false;

			if (!AEInputCheckPrev(AEVK_LBUTTON) && AEInputCheckCurr(AEVK_LBUTTON)) {
				if (Utils::IsMouseInSquare(sfxBarBackground.position.x, sfxBarBackground.position.y, sfxBarBackground.size.x, sfxBarBackground.size.y))
				{
					isButtonPressed[InGame::SFX] = true;
					isButtonPressed[InGame::BGM] = false;
					isButtonPressed[InGame::UI] = false;
				}

				if (Utils::IsMouseInSquare(bgmBarBackground.position.x, bgmBarBackground.position.y, bgmBarBackground.size.x, bgmBarBackground.size.y))
				{
					isButtonPressed[InGame::SFX] = false;
					isButtonPressed[InGame::BGM] = true;
					isButtonPressed[InGame::UI] = false;
				}

				if (Utils::IsMouseInSquare(uiBarBackground.position.x, uiBarBackground.position.y, uiBarBackground.size.x, uiBarBackground.size.y))
				{
					isButtonPressed[InGame::SFX] = false;
					isButtonPressed[InGame::BGM] = false;
					isButtonPressed[InGame::UI] = true;
				}

				if (!AEInputCheckPrev(AEVK_LBUTTON))
				{
					if (Utils::IsMouseInSquare(checkBox.position.x, checkBox.position.y, checkBox.size.x, checkBox.size.y))
					{
						if (!isFullScreen)
							isFullScreen = true;
						else
							isFullScreen = false;
					}
				}
				
			}

			if (isButtonPressed[InGame::SFX])
			{
				f32 startPos = sfxBarBackground.position.x - sfxBarBackground.size.x / 2.f;
				f32 endPos = sfxBarBackground.position.x;
				s32 mx = 0;
				s32 my = 0;

				AEInputGetCursorPosition(&mx, &my);

				f32 mouse_x = static_cast<f32>(mx) - AEGfxGetWindowWidth() / 2.0f;

				sfxBarFill.position.x = std::clamp((mouse_x + startPos) / 2.f, startPos, endPos);
				sfxBarFill.size.x = std::clamp(mouse_x - startPos, 0.f, sfxBarBackground.size.x);
			}
			if (isButtonPressed[InGame::BGM])
			{
				f32 startPos = bgmBarBackground.position.x - bgmBarBackground.size.x / 2.f;
				f32 endPos = bgmBarBackground.position.x;
				s32 mx = 0;
				s32 my = 0;

				AEInputGetCursorPosition(&mx, &my);

				f32 mouse_x = static_cast<f32>(mx) - AEGfxGetWindowWidth() / 2.0f;

				bgmBarFill.position.x = std::clamp((mouse_x + startPos) / 2.f, startPos, endPos);
				bgmBarFill.size.x = std::clamp(mouse_x - startPos, 0.f, bgmBarBackground.size.x);
			}
			if (isButtonPressed[InGame::UI])
			{
				f32 startPos = uiBarBackground.position.x - uiBarBackground.size.x / 2.f;
				f32 endPos = uiBarBackground.position.x;
				s32 mx = 0;
				s32 my = 0;

				AEInputGetCursorPosition(&mx, &my);

				f32 mouse_x = static_cast<f32>(mx) - AEGfxGetWindowWidth() / 2.0f;

				uiBarFill.position.x = std::clamp((mouse_x + startPos) / 2.f, startPos, endPos);
				uiBarFill.size.x = std::clamp(mouse_x - startPos, 0.f, uiBarBackground.size.x);
			}

			if (AEInputCheckReleased(AEVK_LBUTTON)) {
				isButtonPressed[InGame::SFX] = false;
				isButtonPressed[InGame::BGM] = false;
				isButtonPressed[InGame::UI] = false;
			}

		}
		

		if (global::isVolumeReduced)
		{
			AEAudioSetGroupVolume(SFXManager.sound_group[InGame::SFX], sfxBarFill.size.x / sfxBarBackground.size.x * SFXManager.SFXReduceVol);
			AEAudioSetGroupVolume(SFXManager.sound_group[InGame::BGM], bgmBarFill.size.x / bgmBarBackground.size.x * SFXManager.BGMReduceVol);
		}
		else
		{
			AEAudioSetGroupVolume(SFXManager.sound_group[InGame::SFX], sfxBarFill.size.x / sfxBarBackground.size.x * SFXManager.SFXOriginVol);
			AEAudioSetGroupVolume(SFXManager.sound_group[InGame::BGM], bgmBarFill.size.x / bgmBarBackground.size.x * SFXManager.BGMOriginVol);
		}

		AEAudioSetGroupVolume(SFXManager.sound_group[InGame::UI], uiBarFill.size.x / uiBarBackground.size.x * SFXManager.UIOriginVol);
	}

	void SettingUI::Draw()
	{

		Utils::DrawObject(UI, false, 0.8f);
		//Utils::DrawNinePatchMesh(UI, UI.Texture, uiMesh, padding);
		
		Utils::DrawObject(sfxBarFrame.position, { 0.f, 0.f }, sfxBarFrame.size, soundBarBackgroundTexture, soundBarBackgroundMesh, 1.f, false);

		Utils::DrawObject(sfxBarBackground.position, { 0.f, 0.f }, sfxBarBackground.size, soundBarFillTexture, soundBarFillMesh, 0.5f, false);

		Utils::DrawObject(sfxBarFill.position, {0.f, 0.f}, sfxBarFill.size, soundBarFillTexture, soundBarFillMesh,1.f,false);

		Utils::DrawObject(bgmBarFrame.position, { 0.f, 0.f }, bgmBarFrame.size, soundBarBackgroundTexture, soundBarBackgroundMesh, 1.f, false);

		Utils::DrawObject(bgmBarBackground.position, { 0.f, 0.f }, bgmBarBackground.size, soundBarFillTexture, soundBarFillMesh, 0.5f, false);

		Utils::DrawObject(bgmBarFill.position, { 0.f, 0.f }, bgmBarFill.size, soundBarFillTexture, soundBarFillMesh, 1.f, false);

		Utils::DrawObject(uiBarFrame.position, { 0.f, 0.f }, uiBarFrame.size, soundBarBackgroundTexture, soundBarBackgroundMesh, 1.f, false);

		Utils::DrawObject(uiBarBackground.position, { 0.f, 0.f }, uiBarBackground.size, soundBarFillTexture, soundBarFillMesh, 0.5f, false);

		Utils::DrawObject(uiBarFill.position, { 0.f, 0.f }, uiBarFill.size, soundBarFillTexture, soundBarFillMesh, 1.f, false);

		Utils::DrawObject(checkBox.position, { 0.f, 0.f }, checkBox.size, checkBoxTexture, checkBoxMesh, 1.f, false);

		Utils::DrawObject(border.position, { 0.f, 0.f }, border.size, soundBarFillTexture, soundBarFillMesh, 1.f, false);

		if (isFullScreen)
		{
			Utils::DrawObject(checkSign.position, { 0.f, 0.f }, checkSign.size, checkSignTexture, checkSignMesh, 1.f, false);
		}

		f32 wh = global::ScreenWidth / 2.f;
		f32 hh = global::ScreenHeight / 2.f;

		f32 textW, textH;
		AEGfxGetPrintSize(pFont, "SFX", textDrawSize, &textW, &textH);
		AEGfxPrint(pFont, "SFX", (sfxBarFrame.position.x - sfxBarFrame.size.x / 2.f) / wh,	(sfxBarFrame.position.y - sfxBarFrame.size.y / 2.f + paddingForText) / hh + textH / 2.f, textDrawSize, 1.f, 1.f, 1.f, 1.f);

		AEGfxGetPrintSize(pFont, "BGM", textDrawSize, &textW, &textH);
		AEGfxPrint(pFont, "BGM", (bgmBarFrame.position.x - bgmBarFrame.size.x / 2.f) / wh, (bgmBarFrame.position.y - bgmBarFrame.size.y / 2.f + paddingForText) / hh + textH / 2.f, textDrawSize, 1.f, 1.f, 1.f, 1.f);

		AEGfxGetPrintSize(pFont, "UI", textDrawSize, &textW, &textH);
		AEGfxPrint(pFont, "UI", (uiBarFrame.position.x - uiBarFrame.size.x / 2.f) / wh, (uiBarFrame.position.y - uiBarFrame.size.y / 2.f + paddingForText) / hh + textH / 2.f, textDrawSize, 1.f, 1.f, 1.f, 1.f);

		AEGfxGetPrintSize(pFont, "FULLSCREEN", textDrawSize, &textW, &textH);
		AEGfxPrint(pFont, "FULLSCREEN", (uiBarFrame.position.x - uiBarFrame.size.x / 2.f) / wh, (checkBox.position.y - checkBox.size.y / 2.f) / hh + textH / 2.f, textDrawSize, 1.f, 1.f, 1.f, 1.f);
	}

	void SettingUI::Destroy()
	{
		if (UI.Mesh)
			AEGfxMeshFree(UI.Mesh);

		if (UI.Texture)
		{
			AEGfxTextureUnload(UI.Texture);
			UI.Texture = nullptr;
		}
	}

	void SettingUI::StaticDestory()
	{
		
		if (soundBarBackgroundMesh)
			AEGfxMeshFree(soundBarBackgroundMesh);


		if (soundBarBackgroundTexture)
		{
			AEGfxTextureUnload(soundBarBackgroundTexture);
			soundBarBackgroundTexture = nullptr;
		}

		if (soundBarFillMesh)
			AEGfxMeshFree(soundBarFillMesh);

		if (soundBarFillTexture)
		{
			AEGfxTextureUnload(soundBarFillTexture);
			soundBarFillTexture = nullptr;
		}

		if (soundBarButtonMesh)
			AEGfxMeshFree(soundBarButtonMesh);

		if (soundBarButtonTexture)
		{
			AEGfxTextureUnload(soundBarButtonTexture);
			soundBarButtonTexture = nullptr;
		}

		if (checkBoxMesh)
			AEGfxMeshFree(checkBoxMesh);

		if (checkBoxTexture)
		{
			AEGfxTextureUnload(checkBoxTexture);
			checkBoxTexture = nullptr;
		}

		if (checkSignMesh)
			AEGfxMeshFree(checkSignMesh);

		if (checkSignTexture)
		{
			AEGfxTextureUnload(checkSignTexture);
			checkSignTexture = nullptr;
		}
	}
	
}