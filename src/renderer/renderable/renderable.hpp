#ifndef RENDERABLE_H_
#define RENDERABLE_H_


#include <glad/glad.h>
#include <vector>
#include "texture.hpp"
#include "shader.hpp"
#include "shader_engine.hpp"


#define MAX_BONE_INFLUENCE 4


struct Vertex {
    // position
    glm::vec3 position;
    // normal
    glm::vec3 normal;
    // texCoords
    glm::vec2 textureCoordinates;
    // tangent
    glm::vec3 tangent;
    // bitangent
    glm::vec3 biTangent;
    // bone indexes which will influence this vertex
    int m_BoneIDs[MAX_BONE_INFLUENCE];
    // weights from each bone
    float m_Weights[MAX_BONE_INFLUENCE];
};

class Renderable {
public:
    Renderable() : m_VAO(0), m_VBO(0), m_EBO(0) {}
    //TODO: handle object destruction implicitly
    // The problem with ~Renderable is that return by copy could cause
    // OpenGL to delete OpenGL context buffers
    void destroy();
    void draw();
    void setup();
    std::vector<Vertex> getVertices() { return m_vertices; }
    std::vector<unsigned int> getIndices() { return m_indices; }
    void setTexture(const char* path, TextureType type);
    void setShaderEngine(ShaderEngine engine) { m_engine = engine; }

    friend std::ostream& operator<<(std::ostream& os, const Renderable& renderable);
protected:
    GLuint m_VAO, m_VBO, m_EBO;
    ShaderEngine m_engine;
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
    std::vector<Texture> m_textures;
};

#endif
