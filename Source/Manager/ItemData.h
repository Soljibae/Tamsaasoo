#pragma once
#include "AEEngine.h"

namespace Manager
{
    enum ItemGrade
    {
        COMMON,
        RARE,
        EPIC
    };
    struct ItemData {
     std::string name = " ";
     std::string description = " ";
     f32 value1 = -1;
     f32 value2 = -1;
     f32 value3 = -1;
     f32 cooldown = -1;
     f32 duration = -1;
     f32 range = -1;
     f32 procChance = -1;
    ItemGrade grade;
 };
}

