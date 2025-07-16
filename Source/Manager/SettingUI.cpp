#include "SettingUI.h"
#include "../Utils/Utils.h"

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

	void SettingUI::Init()
	{
		f32 padding = 4.f;

		UI.size = {300.f, 500.f};
		UI.position = { 0.f, 0.f };

		UI.Texture = AEGfxTextureLoad("Assets/SelectItem_LevelUp.png");
		uiMesh = Utils::CreateNinePatchMesh();

		sfxBarFrame.position = { 0.f,0.f };
		sfxBarFrame.size = { 80.f * 3.f, 9.f * 2.f };

		sfxBarBackground.position = { 0.f, sfxBarFrame.position.y };
		sfxBarBackground.size = { sfxBarFrame.size.x - padding, sfxBarFrame.size.y - padding };

		sfxBarFill.position = { 0.f,sfxBarFrame.position.y };
		sfxBarFill.size = { 0.f, sfxBarFrame.size.y - padding };
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
		if (AEInputCheckCurr(AEVK_ESCAPE))
			isSettingOn = false;
		
		if (AEInputCheckCurr(AEVK_LBUTTON)) {
			if (Utils::IsMouseInSquare(sfxBarBackground.position.x, sfxBarBackground.position.y, sfxBarBackground.size.x, sfxBarBackground.size.y))
			{
				f32 startPos = sfxBarBackground.position.x - sfxBarBackground.size.x / 2.f;
				s32 mx = 0;
				s32 my = 0;

				AEInputGetCursorPosition(&mx, &my);

				f32 mouse_x = static_cast<f32>(mx) - AEGfxGetWindowWidth() / 2.0f;

				sfxBarFill.position.x = (mouse_x + startPos) / 2.f;
				sfxBarFill.size.x = mouse_x - startPos;
			}
		}
		
	}

	void SettingUI::Draw()
	{
		Utils::DrawNinePatchMesh(UI, UI.Texture, uiMesh, padding);
		
		Utils::DrawObject(sfxBarFrame.position, { 0.f, 0.f }, sfxBarFrame.size, soundBarBackgroundTexture, soundBarBackgroundMesh, 1.f, false);

		Utils::DrawObject(sfxBarBackground.position, { 0.f, 0.f }, sfxBarBackground.size, soundBarFillTexture, soundBarFillMesh, 0.5f, false);

		Utils::DrawObject(sfxBarFill.position, {0.f, 0.f}, sfxBarFill.size, soundBarFillTexture, soundBarFillMesh,1.f,false);
	}

	void SettingUI::Destroy()
	{
		for (auto mesh : uiMesh)
		{
			if (mesh)
				AEGfxMeshFree(mesh);
		}

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