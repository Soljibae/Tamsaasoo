#include "ItemDatabase.h"


namespace InGame
{
    void ItemDatabase::Init() {

        
        auto item1_prototype = std::make_shared<Item_1>();
        item1_prototype->Init();
        itemList[1] = item1_prototype;
        
        auto item2_prototype = std::make_shared<Item_2>();
        item2_prototype->Init();
        itemList[2] = item2_prototype;

        auto item3_prototype = std::make_shared<Item_3>();
        item3_prototype->Init();
        itemList[3] = item3_prototype;

        auto item4_prototype = std::make_shared<Item_4>();
        item4_prototype->Init();
        itemList[4] = item4_prototype;
    }
}