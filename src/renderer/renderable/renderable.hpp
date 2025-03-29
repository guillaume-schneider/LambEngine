#ifndef RENDERABLE_H_
#define RENDERABLE_H_


#include <glad/glad.h>
#include <vector>
#include "texture.hpp"
#include "shader.hpp"
#include "shader_engine.hpp"


#define MAX_BONE_INFLUENCE 4


/**
 * @struct Vertex
 * @brief Represents a vertex with position, normal, texture coordinates, and bone influences.
 *
 * This struct defines a vertex used in rendering, including its position, normal,
 * texture coordinates, tangent, bitangent, and bone influences for skeletal animation.
 */
struct Vertex {
    glm::vec3 position; /**< The position of the vertex in 3D space. */
    glm::vec3 normal; /**< The normal vector of the vertex. */
    glm::vec2 textureCoordinates; /**< The texture coordinates of the vertex. */
    glm::vec3 tangent; /**< The tangent vector of the vertex. */
    glm::vec3 biTangent; /**< The bitangent vector of the vertex. */
    int m_BoneIDs[MAX_BONE_INFLUENCE]; /**< The bone IDs that influence this vertex. */
    float m_Weights[MAX_BONE_INFLUENCE]; /**< The weights of each bone's influence on this vertex. */
};

/**
 * @class Renderable
 * @brief Base class for renderable objects.
 *
 * This class provides the basic functionality for rendering objects, including
 * setting up OpenGL buffers, drawing, and managing textures and shaders.
 */
class Renderable {
public:
    /**
     * @brief Default constructor.
     *
     * Initializes a Renderable object with default values for VAO, VBO, and EBO.
     */
    Renderable() : m_VAO(0), m_VBO(0), m_EBO(0) {}

    /**
     * @brief Destroys the Renderable object and cleans up OpenGL resources.
     *
     * This method should be called to free the OpenGL resources associated with
     * the Renderable object.
     */
    void destroy();

    /**
     * @brief Draws the Renderable object.
     *
     * This method renders the object using the current OpenGL context.
     */
    void draw();

    /**
     * @brief Sets up the Renderable object for rendering.
     *
     * This method initializes the OpenGL buffers and configurations needed for rendering.
     */
    void setup();

    /**
     * @brief Gets the vertices of the Renderable object.
     *
     * @return A vector of Vertex objects representing the vertices of the object.
     */
    std::vector<Vertex> getVertices() { return m_vertices; }

    /**
     * @brief Gets the indices of the Renderable object.
     *
     * @return A vector of unsigned int representing the indices of the object.
     */
    std::vector<unsigned int> getIndices() { return m_indices; }

    /**
     * @brief Sets a texture for the Renderable object.
     *
     * @param path The file path to the texture.
     * @param type The type of the texture.
     */
    void setTexture(const char* path, TextureType type);

    /**
     * @brief Sets the shader engine for the Renderable object.
     *
     * @param engine The shader engine to use for rendering.
     */
    void setShaderEngine(ShaderEngine engine) { m_engine = engine; }

    /**
     * @brief Output stream operator for printing Renderable objects.
     *
     * @param os The output stream.
     * @param renderable The Renderable object to print.
     * @return The output stream with the Renderable object printed.
     */
    friend std::ostream& operator<<(std::ostream& os, const Renderable& renderable);

protected:
    GLuint m_VAO; /**< The Vertex Array Object (VAO) for the Renderable object. */
    GLuint m_VBO; /**< The Vertex Buffer Object (VBO) for the Renderable object. */
    GLuint m_EBO; /**< The Element Buffer Object (EBO) for the Renderable object. */
    ShaderEngine m_engine; /**< The shader engine used for rendering. */
    std::vector<Vertex> m_vertices; /**< The vertices of the Renderable object. */
    std::vector<unsigned int> m_indices; /**< The indices of the Renderable object. */
    std::vector<Texture> m_textures; /**< The textures of the Renderable object. */
};

#endif
