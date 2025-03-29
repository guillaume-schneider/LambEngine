#ifndef MATERIAL_H_
#define MATERIAL_H_

#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <shader.hpp>


/**
 * @enum MaterialType
 * @brief Enumeration of different material types.
 *
 * This enum defines the types of materials that can be used in rendering.
 * Each type corresponds to a specific material with predefined properties.
 */
enum class MaterialType {
    NOT_MATERIAL = -1, /**< Represents an invalid or uninitialized material type. */
    SILVER,            /**< Represents a silver material. */
    GOLD,              /**< Represents a gold material. */
    COPPER             /**< Represents a copper material. */
};

/**
 * @struct Material
 * @brief Represents a material with optical properties for rendering.
 *
 * This struct defines a material with properties such as ambient, diffuse,
 * and specular colors, shininess, and associated shaders. It also includes
 * methods for converting between material types and their string representations.
 */
struct Material {
    /**
     * @brief Default constructor.
     *
     * Initializes a material with default values.
     */
    Material()
        : ambient(glm::vec3(0.0f)), diffuse(glm::vec3(0.0f)), specular(glm::vec3(0.0f)),
          type(MaterialType::NOT_MATERIAL), shininess(0.0f), vertex(nullptr), fragment(nullptr) {}

    /**
     * @brief Constructor with parameters.
     *
     * @param ambient The ambient color of the material.
     * @param diffuse The diffuse color of the material.
     * @param specular The specular color of the material.
     * @param shininess The shininess of the material.
     * @param vertex Pointer to the vertex shader.
     * @param fragment Pointer to the fragment shader.
     */
    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess,
             const Shader* vertex, const Shader* fragment)
        : ambient(ambient), diffuse(diffuse), specular(specular), shininess(shininess),
          type(MaterialType::NOT_MATERIAL), vertex(vertex), fragment(fragment) {}

    MaterialType type; /**< The type of the material. */
    glm::vec3 ambient; /**< The ambient color of the material. */
    glm::vec3 diffuse; /**< The diffuse color of the material. */
    glm::vec3 specular; /**< The specular color of the material. */
    float shininess; /**< The shininess of the material. */

    const Shader* vertex; /**< Pointer to the vertex shader. */
    const Shader* fragment; /**< Pointer to the fragment shader. */

    /**
     * @brief Converts a string representation of a material type to a MaterialType enum.
     *
     * @param mtl_type The string representation of the material type.
     * @return The corresponding MaterialType enum value.
     */
    static MaterialType getMaterialType(const std::string& mtl_type) {
        if (mtl_type == "Silver") {
            return MaterialType::SILVER;
        } else if (mtl_type == "Gold") {
            return MaterialType::GOLD;
        } else if (mtl_type == "Copper") {
            return MaterialType::COPPER;
        } else {
            return MaterialType::NOT_MATERIAL;
        }
    }

    /**
     * @brief Converts a MaterialType enum to its string representation.
     *
     * @param type The MaterialType enum value.
     * @return The string representation of the material type.
     */
    static std::string getMaterialString(MaterialType type) {
        switch (type) {
            case MaterialType::GOLD:
                return "Gold";
            case MaterialType::SILVER:
                return "Silver";
            case MaterialType::COPPER:
                return "Copper";
            default:
                return "Not material";
        }
    }

    /**
     * @brief Output stream operator for printing Material objects.
     *
     * @param os The output stream.
     * @param material The Material object to print.
     * @return The output stream with the Material object printed.
     */
    friend std::ostream& operator<<(std::ostream& os, const Material& material) {
        os << "Material(type: " << Material::getMaterialString(material.type)
           << ", ambient: [" << material.ambient[0] << ", " << material.ambient[1] << ", " << material.ambient[2] << "]"
           << ", diffuse: [" << material.diffuse[0] << ", " << material.diffuse[1] << ", " << material.diffuse[2] << "]"
           << ", specular: [" << material.specular[0] << ", " << material.specular[1] << ", " << material.specular[2] << "])";
        return os;
    }
};

#endif
