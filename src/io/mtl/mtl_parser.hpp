#ifndef MTL_PARSER_H_
#define MTL_PARSER_H_

#include <unordered_map>
#include <string>
#include <material.hpp>

namespace IO {
    /**
     * @brief Parses a Material Template Library (MTL) file and returns a map of materials.
     *
     * This function reads an MTL file from the specified path and parses it to extract
     * material definitions. Each material is associated with a MaterialType and stored
     * in an unordered_map for easy access.
     *
     * @param path The file path to the MTL file to be parsed.
     * @return An unordered_map where the key is the MaterialType and the value is the
     *         corresponding Material object.
     *
     * @throw std::ios_base::failure If the file cannot be opened or read.
     * @throw std::runtime_error If there is an error parsing the file.
     */
    std::unordered_map<MaterialType, Material> parseMTL(const std::string& path);
};

#endif
