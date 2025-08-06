#pragma once
#include "../InGame/Actor.h"
#include <vector>
#include <array>
#include <memory>
#include "../InGame//SFX.h"

namespace Manager
{
	struct SoundBar
	{

	};

	class SettingUI
	{
	public:
		void Init();
		static void StaticInit();
		void Update();
		void Draw();
		void Destroy();
		static void StaticDestory();

		s8 pFont;
		f32 fontSize = 72.f;
		f32 padding = 15.f;
		f32 textDrawSize = 0.2f;

		static AEGfxVertexList* soundBarBackgroundMesh;
		static AEGfxTexture* soundBarBackgroundTexture;
		static AEGfxVertexList* soundBarFillMesh;
		static AEGfxTexture* soundBarFillTexture;
		static AEGfxVertexList* soundBarButtonMesh;
		static AEGfxTexture* soundBarButtonTexture;

		static AEGfxVertexList* checkBoxMesh;
		static AEGfxTexture* checkBoxTexture;
		static AEGfxVertexList* checkSignMesh;
		static AEGfxTexture* checkSignTexture;
		InGame::Actor UI;

		InGame::Actor sfxBarFrame;
		InGame::Actor sfxBarBackground;
		InGame::Actor sfxBarFill;

		InGame::Actor bgmBarFrame;
		InGame::Actor bgmBarBackground;
		InGame::Actor bgmBarFill;

		InGame::Actor uiBarFrame;
		InGame::Actor uiBarBackground;
		InGame::Actor uiBarFill;

		InGame::Actor checkBox;
		InGame::Actor checkSign;

		InGame::Actor border;

		std::map<InGame::SFXType, bool> isButtonPressed;

		bool isSettingOn = false;
		bool isFullScreen = true;

	private:
	};
	extern SettingUI SettingPanel;
}