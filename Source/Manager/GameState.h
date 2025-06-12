#pragma once

namespace Manager
{
	enum class EGameState
	{
		TEMP,
		INTRO,
		MAINMENU,
		PLAYING,
	};

	class AGameState
	{
	public:
		virtual ~AGameState() {}
		virtual void Init();
		virtual void Update();
		virtual void Draw();
		virtual void Destroy();

	};
}