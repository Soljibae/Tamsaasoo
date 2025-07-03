#pragma once

#include <string>
#include <unordered_map>
#include "ItemData.h"

namespace Manager
{
    class ItemParser {
    public:
        std::unordered_map<int, ItemData> Parse(const std::string& filePath);
    };
}