#include "Background.h"
#include "../Global/GlobalVariables.h"
#include "../Utils/Utils.h"

namespace InGame
{
	class Background : public Actor
	{
		virtual void Init() override;
		virtual void Update() override;
		virtual void Draw() override;
		virtual void Destroy() override;
	};
}