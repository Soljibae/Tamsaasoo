#pragma once
#include <map>
#include <memory>
#include "Item.h"

namespace InGame
{
    class ItemDatabase {
    private:
    public:
        void Init();
        std::map<int, std::shared_ptr<Item>> itemList;
    };
}
