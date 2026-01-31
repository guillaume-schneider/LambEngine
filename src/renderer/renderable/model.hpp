#ifndef MODEL_H_
#define MODEL_H_

#include <iostream>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <glm/glm.hpp>

#include "primitive.hpp"
#include "renderable.hpp"
#include "shader.hpp"
#include "shader_engine.hpp"
#include "texture.hpp"

/**
 * @class Mesh
 * @brief Represents a mesh that can be rendered.
 *
 * This class extends Renderable and represents a mesh with vertices, indices,
 * and textures. It is used to define the geometry and appearance of a 3D object.
 */
class Mesh : public Renderable
{
public:
    /**
     * @brief Constructor for Mesh.
     *
     * Initializes a Mesh object with the given vertices, indices, and textures.
     *
     * @param vertices A vector of Vertex objects representing the vertices of the mesh.
     * @param indices A vector of unsigned int representing the indices of the mesh.
     * @param textures A vector of Texture objects representing the textures of the mesh.
     */
    Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);
};

/**
 * @class Model
 * @brief Represents a 3D model composed of multiple meshes.
 *
 * This class manages a collection of meshes and their associated textures.
 * It provides functionality for loading models from files and rendering them.
 */
class Model
{
public:
    /**
     * @brief Constructor for Model.
     *
     * Loads a model from the specified file path.
     *
     * @param path The file path to the model file.
     */
    Model(std::string const path);

    /**
     * @brief Constructor for Model.
     *
     * Initializes a Model object with the given primitive.
     *
     * @param primitive The primitive to use for the model.
     */
    Model(Primitive& primitive);

    /**
     * @brief Draws the model.
     *
     * Renders all the meshes in the model using the current OpenGL context.
     */
    void draw();

    /**
     * @brief Sets the shader engine for the model.
     *
     * @param engine The shader engine to use for rendering.
     */
    void setShaderEngine(ShaderEngine engine);

    /**
     * @brief Gets the meshes of the model.
     *
     * @return A vector of Renderable objects representing the meshes of the model.
     */
    std::vector<Renderable> getMeshes() { return m_meshes; }

private:
    std::vector<Renderable> m_meshes;      /**< The meshes of the model. */
    std::string m_directory;               /**< The directory containing the model files. */
    std::vector<Texture> m_texturesLoaded; /**< The textures loaded for the model. */

    /**
     * @brief Loads a model from the specified file path.
     *
     * @param path The file path to the model file.
     */
    void loadModel(std::string path);

    /**
     * @brief Processes a node in the model's scene.
     *
     * @param node The node to process.
     * @param scene The scene containing the node.
     */
    void processNode(aiNode* node, const aiScene* scene);

    /**
     * @brief Processes a mesh in the model's scene.
     *
     * @param mesh The mesh to process.
     * @param scene The scene containing the mesh.
     * @return A Mesh object representing the processed mesh.
     */
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    /**
     * @brief Loads material textures for a mesh.
     *
     * @param mat The material to load textures for.
     * @param assimpTextureType The type of texture to load using Assimp.
     * @param lambTextureType The type of texture to load using the custom engine.
     * @return A vector of Texture objects representing the loaded textures.
     */
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType assimpTextureType,
                                              TextureType lambTextureType);
};

#endif
