#include "Button.h"
#include "../Utils/Utils.h"
#include "../Global/GlobalVariables.h"

namespace Manager
{
    void Button::Update()
    {
        if (IsHovered() && !wasHovered)
        {
            this->size.x *= 1.1f;
            this->size.y *= 1.1f;
            wasHovered = true;
        }
        else if (!IsHovered() && wasHovered)
        {
            this->size.x /= 1.1f;
            this->size.y /= 1.1f;
            wasHovered = false;
        }

        if (IsClicked())
        {
            OnClick();
        }
    }

    void Button::SetCallback(std::function<void()> func)
    {
        callback = func;
    }

    void Button::OnClick()
    {
        if (callback)
            callback();
    }

    bool Button::IsHovered()
    {
        return Utils::IsMouseInSquare(*this);
    }

    bool Button::IsClicked()
    {
        return IsHovered() && global::KeyInput(AEVK_LBUTTON);
    }
}