#include "ItemDatabase.h"
#include "ItemParser.h"

namespace Manager
{
    void ItemDatabase::Init() {
        ItemParser parser;
        itemValueDatabase = parser.Parse("../../Assets/item_data.csv");
        
        auto item1_prototype = std::make_shared<InGame::Item_1>();
        item1_prototype->Init(itemValueDatabase[1]);
        itemList[item1_prototype->id] = item1_prototype;
        
        auto item2_prototype = std::make_shared<InGame::Item_2>();
        item2_prototype->Init(itemValueDatabase[2]);
        itemList[item2_prototype->id] = item2_prototype;

        auto item3_prototype = std::make_shared<InGame::Item_3>();
        item3_prototype->Init(itemValueDatabase[3]);
        itemList[item3_prototype->id] = item3_prototype;

        auto item4_prototype = std::make_shared<InGame::Item_4>();
        item4_prototype->Init(itemValueDatabase[4]);
        itemList[item4_prototype->id] = item4_prototype;

        auto item5_prototype = std::make_shared<InGame::Item_5>();
        item5_prototype->Init(itemValueDatabase[5]);
        itemList[item5_prototype->id] = item5_prototype;

        auto item6_prototype = std::make_shared<InGame::Item_6>();
        item6_prototype->Init(itemValueDatabase[6]);
        itemList[item6_prototype->id] = item6_prototype;

        auto item7_prototype = std::make_shared<InGame::Item_7>();
        item7_prototype->Init(itemValueDatabase[7]);
        itemList[item7_prototype->id] = item7_prototype;

        auto item8_prototype = std::make_shared<InGame::Item_8>();
        item8_prototype->Init(itemValueDatabase[8]);
        itemList[item8_prototype->id] = item8_prototype;

        auto item9_prototype = std::make_shared<InGame::Item_9>();
        item9_prototype->Init(itemValueDatabase[9]);
        itemList[item9_prototype->id] = item9_prototype;

        auto item10_prototype = std::make_shared<InGame::Item_10>();
        item10_prototype->Init(itemValueDatabase[10]);
        itemList[item10_prototype->id] = item10_prototype;

        auto item11_prototype = std::make_shared<InGame::Item_11>();
        item11_prototype->Init(itemValueDatabase[11]);
        itemList[item11_prototype->id] = item11_prototype;

        auto item12_prototype = std::make_shared<InGame::Item_12>();
        item12_prototype->Init(itemValueDatabase[12]);
        itemList[item12_prototype->id] = item12_prototype;

        auto item13_prototype = std::make_shared<InGame::Item_13>();
        item13_prototype->Init(itemValueDatabase[13]);
        itemList[item13_prototype->id] = item13_prototype;

        auto item14_prototype = std::make_shared<InGame::Item_14>();
        item14_prototype->Init(itemValueDatabase[14]);
        itemList[item14_prototype->id] = item14_prototype;

        auto item15_prototype = std::make_shared<InGame::Item_15>();
        item15_prototype->Init(itemValueDatabase[15]);
        itemList[item15_prototype->id] = item15_prototype;

        auto item16_prototype = std::make_shared<InGame::Item_16>();
        item16_prototype->Init(itemValueDatabase[16]);
        itemList[item16_prototype->id] = item16_prototype;

        auto item17_prototype = std::make_shared<InGame::Item_17>();
        item17_prototype->Init(itemValueDatabase[17]);
        itemList[item17_prototype->id] = item17_prototype;

        auto item18_prototype = std::make_shared<InGame::Item_18>();
        item18_prototype->Init(itemValueDatabase[18]);
        itemList[item18_prototype->id] = item18_prototype;

        auto item19_prototype = std::make_shared<InGame::Item_19>();
        item19_prototype->Init(itemValueDatabase[19]);
        itemList[item19_prototype->id] = item19_prototype;

        auto item20_prototype = std::make_shared<InGame::Item_20>();
        item20_prototype->Init(itemValueDatabase[20]);
        itemList[item20_prototype->id] = item20_prototype;

        auto item21_prototype = std::make_shared<InGame::Item_21>();
        item21_prototype->Init(itemValueDatabase[21]);
        itemList[item21_prototype->id] = item21_prototype;

        auto item22_prototype = std::make_shared<InGame::Item_22>();
        item22_prototype->Init(itemValueDatabase[22]);
        itemList[item22_prototype->id] = item22_prototype;

        auto item23_prototype = std::make_shared<InGame::Item_23>();
        item23_prototype->Init(itemValueDatabase[23]);
        itemList[item23_prototype->id] = item23_prototype;

        auto item24_prototype = std::make_shared<InGame::Item_24>();
        item24_prototype->Init(itemValueDatabase[24]);
        itemList[item24_prototype->id] = item24_prototype;

        auto item25_prototype = std::make_shared<InGame::Item_25>();
        item25_prototype->Init(itemValueDatabase[25]);
        itemList[item25_prototype->id] = item25_prototype;

        auto item26_prototype = std::make_shared<InGame::Item_26>();
        item26_prototype->Init(itemValueDatabase[26]);
        itemList[item26_prototype->id] = item26_prototype;

        auto item27_prototype = std::make_shared<InGame::Item_27>();
        item27_prototype->Init(itemValueDatabase[27]);
        itemList[item27_prototype->id] = item27_prototype;

        auto item28_prototype = std::make_shared<InGame::Item_28>();
        item28_prototype->Init(itemValueDatabase[28]);
        itemList[item28_prototype->id] = item28_prototype;

        auto item29_prototype = std::make_shared<InGame::Item_29>();
        item29_prototype->Init(itemValueDatabase[29]);
        itemList[item29_prototype->id] = item29_prototype;

        auto item30_prototype = std::make_shared<InGame::Item_30>();
        item30_prototype->Init(itemValueDatabase[30]);
        itemList[item30_prototype->id] = item30_prototype;

        auto item31_prototype = std::make_shared<InGame::Item_31>();
        item31_prototype->Init(itemValueDatabase[31]);
        itemList[item31_prototype->id] = item31_prototype;

        auto item32_prototype = std::make_shared<InGame::Item_32>();
        item32_prototype->Init(itemValueDatabase[32]);
        itemList[item32_prototype->id] = item32_prototype;

        auto item33_prototype = std::make_shared<InGame::Item_33>();
        item33_prototype->Init(itemValueDatabase[33]);
        itemList[item33_prototype->id] = item33_prototype;

        auto item34_prototype = std::make_shared<InGame::Item_34>();
        item34_prototype->Init(itemValueDatabase[34]);
        itemList[item34_prototype->id] = item34_prototype;

        auto item35_prototype = std::make_shared<InGame::Item_35>();
        item35_prototype->Init(itemValueDatabase[35]);
        itemList[item35_prototype->id] = item35_prototype;

        auto item36_prototype = std::make_shared<InGame::Item_36>();
        item36_prototype->Init(itemValueDatabase[36]);
        itemList[item36_prototype->id] = item36_prototype;

        auto item37_prototype = std::make_shared<InGame::Item_37>();
        item37_prototype->Init(itemValueDatabase[37]);
        itemList[item37_prototype->id] = item37_prototype;

        auto item38_prototype = std::make_shared<InGame::Item_38>();
        item38_prototype->Init(itemValueDatabase[38]);
        itemList[item38_prototype->id] = item38_prototype;

        auto item39_prototype = std::make_shared<InGame::Item_39>();
        item39_prototype->Init(itemValueDatabase[39]);
        itemList[item39_prototype->id] = item39_prototype;

        auto item40_prototype = std::make_shared<InGame::Item_40>();
        item40_prototype->Init(itemValueDatabase[40]);
        itemList[item40_prototype->id] = item40_prototype;

        auto item41_prototype = std::make_shared<InGame::Item_41>();
        item41_prototype->Init(itemValueDatabase[41]);
        itemList[item41_prototype->id] = item41_prototype;

        auto item42_prototype = std::make_shared<InGame::Item_42>();
        item42_prototype->Init(itemValueDatabase[42]);
        itemList[item42_prototype->id] = item42_prototype;

        auto item43_prototype = std::make_shared<InGame::Item_43>();
        item43_prototype->Init(itemValueDatabase[43]);
        itemList[item43_prototype->id] = item43_prototype;

        auto item44_prototype = std::make_shared<InGame::Item_44>();
        item44_prototype->Init(itemValueDatabase[44]);
        itemList[item44_prototype->id] = item44_prototype;

        auto item45_prototype = std::make_shared<InGame::Item_45>();
        item45_prototype->Init(itemValueDatabase[45]);
        itemList[item45_prototype->id] = item45_prototype;
    }
}