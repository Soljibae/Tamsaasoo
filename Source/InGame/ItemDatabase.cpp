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

        auto item5_prototype = std::make_shared<Item_5>();
        item5_prototype->Init();
        itemList[5] = item5_prototype;

        auto item6_prototype = std::make_shared<Item_6>();
        item6_prototype->Init();
        itemList[6] = item6_prototype;

        auto item7_prototype = std::make_shared<Item_7>();
        item7_prototype->Init();
        itemList[7] = item7_prototype;

        auto item8_prototype = std::make_shared<Item_8>();
        item8_prototype->Init();
        itemList[8] = item8_prototype;

        auto item9_prototype = std::make_shared<Item_9>();
        item9_prototype->Init();
        itemList[9] = item9_prototype;

        auto item10_prototype = std::make_shared<Item_10>();
        item10_prototype->Init();
        itemList[10] = item10_prototype;

        auto item11_prototype = std::make_shared<Item_11>();
        item11_prototype->Init();
        itemList[11] = item11_prototype;

        auto item12_prototype = std::make_shared<Item_12>();
        item12_prototype->Init();
        itemList[12] = item12_prototype;

        auto item13_prototype = std::make_shared<Item_13>();
        item13_prototype->Init();
        itemList[13] = item13_prototype;

        auto item14_prototype = std::make_shared<Item_14>();
        item14_prototype->Init();
        itemList[14] = item14_prototype;

        auto item15_prototype = std::make_shared<Item_15>();
        item15_prototype->Init();
        itemList[15] = item15_prototype;

        auto item16_prototype = std::make_shared<Item_16>();
        item16_prototype->Init();
        itemList[16] = item16_prototype;

        auto item17_prototype = std::make_shared<Item_17>();
        item17_prototype->Init();
        itemList[17] = item17_prototype;

        auto item18_prototype = std::make_shared<Item_18>();
        item18_prototype->Init();
        itemList[18] = item18_prototype;

        auto item19_prototype = std::make_shared<Item_19>();
        item19_prototype->Init();
        itemList[19] = item19_prototype;

        auto item20_prototype = std::make_shared<Item_20>();
        item20_prototype->Init();
        itemList[20] = item20_prototype;

        auto item21_prototype = std::make_shared<Item_21>();
        item21_prototype->Init();
        itemList[21] = item21_prototype;
    }
}