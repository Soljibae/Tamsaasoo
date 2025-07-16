#pragma once
#include "../InGame/Actor.h"
#include <vector>
#include <array>
#include <memory>

namespace Manager
{
	class SettingUI
	{
	public:
		void Init();
		static void StaticInit();
		void Update();
		void Draw();
		void Destroy();
		static void StaticDestory();

		std::array<AEGfxVertexList*, 9> uiMesh;
		s8 pFont;
		f32 padding = 15.f;

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

		std::vector<std::shared_ptr<InGame::Actor>> borders;

		bool isSettingOn = false;
		bool isFullScreen = false;

	private:
	};
	extern SettingUI SettingPanel;
}