#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <stdexcept>
#include <string>

#include <glad/glad.h>

/**
 * @enum TextureType
 * @brief Enumeration of different texture types.
 *
 * This enum defines the types of textures that can be used in rendering.
 * Each type corresponds to a specific texture map used in shading calculations.
 */
enum TextureType
{
    NOT_TEXTURE = -1, /**< Represents an invalid or uninitialized texture type. */
    DIFFUSE,          /**< Represents a diffuse texture map. */
    SPECULAR          /**< Represents a specular texture map. */
};

/**
 * @brief Converts a TextureType enum to its string representation.
 *
 * This function takes a TextureType enum value and returns its corresponding
 * string representation.
 *
 * @param type The TextureType enum value to convert.
 * @return The string representation of the TextureType.
 */
std::string toString(TextureType type);

/**
 * @brief Converts a string representation to a TextureType enum.
 *
 * This function takes a string and returns the corresponding TextureType enum value.
 * If the string does not match any known texture type, it returns NOT_TEXTURE.
 *
 * @param str The string to convert.
 * @return The TextureType enum value corresponding to the string.
 */
TextureType fromString(const std::string& str);

/**
 * @struct Texture
 * @brief Represents a texture with an ID, type, and file path.
 *
 * This struct defines a texture used in rendering, including its OpenGL ID,
 * type, and the file path from which it was loaded.
 */
struct Texture
{
    GLuint id;        /**< The OpenGL ID of the texture. */
    TextureType type; /**< The type of the texture. */
    std::string path; /**< The file path to the texture image. */
};

#endif
