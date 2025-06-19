#include "ItemDatabase.h"


namespace InGame
{
    void ItemDatabase::Init() {

        //
        auto item1_prototype = std::make_unique<Item_1>();
        item1_prototype->Init();
        itemList[1] = std::move(item1_prototype);
        //
        auto item2_prototype = std::make_unique<Item_2>();
        item2_prototype->Init();
        itemList[2] = std::move(item2_prototype);

        auto item3_prototype = std::make_unique<Item_3>();
        item3_prototype->Init();
        itemList[3] = std::move(item3_prototype);
    }
}