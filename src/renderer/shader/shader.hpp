#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @struct Shader
 * @brief Represents a shader with its source code and ID.
 *
 * This struct defines a shader used in rendering, including its source code
 * and an ID that uniquely identifies the shader in the graphics pipeline.
 */
struct Shader {
    std::string source; /**< The source code of the shader. */
    unsigned int id; /**< The ID of the shader. */
};

/**
 * @class ShaderFactory
 * @brief Factory class for creating shaders.
 *
 * This class provides a static method for creating shaders from file paths.
 * It handles the loading and compilation of shader source code.
 */
class ShaderFactory {
public:
    /**
     * @brief Creates a shader from a file.
     *
     * This method loads the shader source code from the specified file path,
     * compiles it, and returns a Shader object containing the source code
     * and the shader ID.
     *
     * @param filePath The file path to the shader source code.
     * @param shaderType The type of shader to create (e.g., vertex, fragment).
     * @return A Shader object containing the source code and ID of the created shader.
     *
     * @throw std::runtime_error If the shader cannot be created or compiled.
     */
    static Shader createShader(const std::string& filePath, unsigned int shaderType);
};

#endif
