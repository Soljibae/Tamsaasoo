#pragma once

namespace Manager
{
	enum class EGameState
	{
		TEMP,
		INTRO,
		MAINMENU,
		KEYDESCRIPTION,
		PLAYING,
		GAMEOVER,
	};

	class AGameState
	{
	public:
		virtual void Init() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void Destroy() = 0;
		virtual ~AGameState() = default;
	};
}