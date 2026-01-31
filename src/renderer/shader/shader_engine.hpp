#ifndef SHADER_ENGINE_HPP_
#define SHADER_ENGINE_HPP_

#include <vector>

#include <shader.hpp>

/**
 * @class ShaderEngine
 * @brief Manages shaders and their compilation into a shader program.
 *
 * This class handles the addition of shaders, compilation into a shader program,
 * and provides methods to set uniform variables in the shader program.
 */
class ShaderEngine
{
private:
    std::vector<Shader> m_shaders;  /**< The list of shaders in the engine. */
    unsigned int m_shaderProgramID; /**< The ID of the compiled shader program. */

public:
    /**
     * @brief Adds a shader to the engine.
     *
     * @param shader The shader to add.
     */
    void addShader(Shader& shader);

    /**
     * @brief Compiles the shaders into a shader program.
     *
     * This method links all added shaders into a single shader program.
     */
    void compile();

    /**
     * @brief Gets the ID of the shader program.
     *
     * @return The ID of the compiled shader program.
     */
    unsigned int getShaderProgramID() { return m_shaderProgramID; }

    /**
     * @brief Activates the shader program for use.
     *
     * This method sets the shader program as the current program in the OpenGL context.
     */
    void use();

    /**
     * @brief Sets an integer uniform in the shader program.
     *
     * @param name The name of the uniform variable.
     * @param value The value to set.
     */
    void setInt(const std::string& name, int value)
    {
        glUniform1i(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
    }

    /**
     * @brief Sets a 3D vector uniform in the shader program.
     *
     * @param name The name of the uniform variable.
     * @param x The x component of the vector.
     * @param y The y component of the vector.
     * @param z The z component of the vector.
     */
    void setVec3(const std::string& name, float x, float y, float z)
    {
        glUniform3f(glGetUniformLocation(m_shaderProgramID, name.c_str()), x, y, z);
    }

    /**
     * @brief Sets a 3D vector uniform in the shader program.
     *
     * @param name The name of the uniform variable.
     * @param value The glm::vec3 value to set.
     */
    void setVec3(const std::string& name, glm::vec3 value) { ShaderEngine::setVec3(name, value.x, value.y, value.z); }

    /**
     * @brief Sets a 4x4 matrix uniform in the shader program.
     *
     * @param name The name of the uniform variable.
     * @param mat The glm::mat4 value to set.
     */
    void setMat4(const std::string& name, glm::mat4 mat)
    {
        glUniformMatrix4fv(glGetUniformLocation(m_shaderProgramID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
    }

    /**
     * @brief Sets a float uniform in the shader program.
     *
     * @param name The name of the uniform variable.
     * @param value The value to set.
     */
    void setFloat(const std::string& name, float value)
    {
        glUniform1f(glGetUniformLocation(m_shaderProgramID, name.c_str()), value);
    }

    /**
     * @brief Gets the number of shaders in the engine.
     *
     * @return The number of shaders.
     */
    int size() { return m_shaders.size(); }
};

/**
 * @class ShaderEngineFactory
 * @brief Factory class for creating ShaderEngine instances.
 *
 * This class provides a static method for creating ShaderEngine instances
 * with vertex and fragment shaders.
 */
class ShaderEngineFactory
{
public:
    /**
     * @brief Creates a ShaderEngine with vertex and fragment shaders.
     *
     * This method creates a ShaderEngine instance, adds the specified vertex
     * and fragment shaders, compiles them, and returns the engine.
     *
     * @param vertex The file path to the vertex shader.
     * @param fragment The file path to the fragment shader.
     * @return A ShaderEngine instance with the compiled shaders.
     *
     * @throw std::runtime_error If the shaders cannot be created or compiled.
     */
    static ShaderEngine createEngine(const std::string& vertex, const std::string& fragment)
    {
        ShaderEngine engine;
        Shader vertexShader = ShaderFactory::createShader(vertex, GL_VERTEX_SHADER);
        Shader fragmentShader = ShaderFactory::createShader(fragment, GL_FRAGMENT_SHADER);
        engine.addShader(vertexShader);
        engine.addShader(fragmentShader);
        engine.compile();
        return engine;
    }
};

#endif
