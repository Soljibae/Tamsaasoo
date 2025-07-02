#include "ItemParser.h"
#include <fstream>
#include <sstream>
#include <vector>

namespace Manager
{
    std::unordered_map<int, ItemData> ItemParser::Parse(const std::string& filePath) {
        std::unordered_map<int, ItemData> database;
        std::ifstream file(filePath);
        std::string line;

        if (!file.is_open()) {
            return database;
        }

        int currentId = 0;
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }

            try {
                currentId = std::stoi(line);
            }
            catch (const std::invalid_argument& e) {
                continue;
            }

            std::string junk, nameLine, descriptionLine, dataLine;
            if (std::getline(file, junk) &&
                std::getline(file, nameLine) &&
                std::getline(file, descriptionLine) &&
                std::getline(file, junk) &&
                std::getline(file, dataLine)) {

                std::stringstream ss(dataLine);
                std::string segment;
                std::vector<float> values;
                while (std::getline(ss, segment, ',')) {
                    values.push_back(std::stof(segment));
                }

                ItemData data;

                data.name = nameLine;
                data.description = descriptionLine;

                if (values.size() >= 7) {
                    data.value1 = values[0];
                    data.value2 = values[1];
                    data.value3 = values[2];
                    data.cooldown = values[3];
                    data.duration = values[4];
                    data.procChance = values[5];
                    data.range = values[6];
                }

                database[currentId] = data;
            }
        }

        return database;
    }
}