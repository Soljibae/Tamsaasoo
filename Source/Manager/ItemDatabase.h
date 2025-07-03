#pragma once
#include <unordered_map>
#include <memory>
#include "../InGame/Item.h"
#include "ItemData.h"

namespace Manager
{
    class ItemDatabase {
    private:
    public:
        void Init();
        std::map<int, std::shared_ptr<InGame::Item>> itemList;
        std::unordered_map<int, ItemData> itemValueDatabase;
    };
}
