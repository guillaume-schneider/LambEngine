#include "config_manager.hpp"


ConfigurationManager* ConfigurationManager::instance = nullptr;


json ConfigurationManager::parseJSON(const std::string& path) {
    std::ifstream inputFile(path);
    if (!inputFile.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
        return nullptr;
     }

    json buffer;
    inputFile >> buffer;
    return buffer;
}
