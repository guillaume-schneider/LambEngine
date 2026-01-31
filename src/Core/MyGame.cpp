// MyGame.cpp
#include "MyGame.hpp"

#include <format> // pour std::format
#include <vector>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "camera.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "input.hpp"
#include "materials.hpp"
#include "model.hpp"
#include "primitive.hpp"
#include "shader.hpp"
#include "shader_engine.hpp"
#include "time.hpp"

// Pas de STB_IMAGE_IMPLEMENTATION ici !
// Place-le dans un seul .cpp autre (ex: Texture.cpp).

// -----------------------------
// OnInit : création des ressources
// -----------------------------
void MyGame::OnInit(Engine& engine)
{
    // Aspect ratio (si tu peux le récupérer depuis Engine)
    // m_CurrentAspectRatio = engine.GetAspectRatio();

    // Shaders pour la scène
    m_LightingShader = new ShaderEngine();
    m_LightShader = new ShaderEngine();

    Shader lightingVertexShader = ShaderFactory::createShader(".\\shaders\\lighting_vertex.glsl", GL_VERTEX_SHADER);
    m_LightingShader->addShader(lightingVertexShader);

    Shader lightingFragmentShader =
        ShaderFactory::createShader(".\\shaders\\lighting_fragment.glsl", GL_FRAGMENT_SHADER);
    m_LightingShader->addShader(lightingFragmentShader);
    m_LightingShader->compile();

    Shader lightVertexShader = ShaderFactory::createShader(".\\shaders\\light_vertex.glsl", GL_VERTEX_SHADER);
    m_LightShader->addShader(lightVertexShader);

    Shader lightFragmentShader = ShaderFactory::createShader(".\\shaders\\light_fragment.glsl", GL_FRAGMENT_SHADER);
    m_LightShader->addShader(lightFragmentShader);
    m_LightShader->compile();

    m_BasicShader = new ShaderEngine(ShaderEngineFactory::createEngine(
        ".\\shaders\\basic_vertex.glsl", ".\\shaders\\shader_single_color_fragment.glsl"));

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glEnable(GL_STENCIL_TEST);

    // Géométrie
    m_LightCube = new Cube(1.0f);
    m_LightCube->setShaderEngine(*m_LightShader);

    m_Sphere = new Sphere(1.0f);

    m_LitCube = new Cube(1.0f);
    m_LitCube->setShaderEngine(*m_LightingShader);
    m_LitCube->setTexture(".\\res\\box.bmp", TextureType::DIFFUSE);
    m_LitCube->setTexture(".\\res\\box_specular_map.png", TextureType::SPECULAR);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    m_Teapot = new Model(".\\res\\teapot.fbx");
    m_Teapot->setShaderEngine(*m_BasicShader);

    m_Camera = new Camera();

    // Positions des cubes
    m_CubePositions = {glm::vec3(0.0f, 0.0f, 0.0f),     glm::vec3(2.0f, 5.0f, -15.0f), glm::vec3(-1.5f, -2.2f, -2.5f),
                       glm::vec3(-3.8f, -2.0f, -12.3f), glm::vec3(2.4f, -0.4f, -3.5f), glm::vec3(-1.7f, 3.0f, -7.5f),
                       glm::vec3(1.3f, -2.0f, -2.5f),   glm::vec3(1.5f, 2.0f, -2.5f),  glm::vec3(1.5f, 0.2f, -1.5f),
                       glm::vec3(-1.3f, 1.0f, -1.5f)};

    // Positions des lights
    m_PointLightPositions = {glm::vec3(0.7f, 0.2f, 2.0f), glm::vec3(2.3f, -3.3f, -4.0f), glm::vec3(-4.0f, 2.0f, -12.0f),
                             glm::vec3(0.0f, 0.0f, -3.0f)};

    // Input caméra
    InputHandler::CursorMovementCallback callback =
        std::bind(&Camera::computeCursorCameraMovements, m_Camera, std::placeholders::_1, std::placeholders::_2);
    InputHandlerFactory::createInputHandler(callback);

    // Matériaux (pour plus tard)
    Material goldMaterial = MaterialManager::getInstance()->getMaterial(MaterialType::GOLD);
    Material silverMaterial = MaterialManager::getInstance()->getMaterial(MaterialType::SILVER);
    Material copperMaterial = MaterialManager::getInstance()->getMaterial(MaterialType::COPPER);
}

// -----------------------------
// OnUpdate : logique (input, mouvements)
// -----------------------------
void MyGame::OnUpdate(Engine& engine, float dt)
{
    const Uint8* keystate = SDL_GetKeyboardState(nullptr);
    std::vector<Action> actions = getActions(keystate);

    m_Camera->computeActions(actions);

    // Ici tu peux mettre d'autres updates (animations, timers, etc.)
}

// -----------------------------
// OnRender : tout le rendu OpenGL
// -----------------------------
void MyGame::OnRender(Engine& engine)
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);

    glm::mat4 view = m_Camera->getViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), m_CurrentAspectRatio, 0.1f, 100.0f);

    // ---- Light cubes ----
    // m_LightShader->use();
    // m_LightShader->setMat4("projection", projection);
    // m_LightShader->setMat4("view", view);

    // glm::mat4 modelMatrix;
    // for (std::size_t i = 0; i < m_PointLightPositions.size(); ++i) {
    //     modelMatrix = glm::mat4(1.0f);
    //     modelMatrix = glm::translate(modelMatrix, m_PointLightPositions[i]);
    //     modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2f));
    //     m_LightShader->setMat4("model", modelMatrix);
    //     m_LightCube->draw();
    // }

    // ---- Lighting shader ----
    // m_LightingShader->use();

    // for (int i = 0; i < 4; ++i) {
    //     std::string lightPositionID = std::format("pointLights[{}]", i);
    //     m_LightingShader->setVec3(lightPositionID + ".position", m_PointLightPositions[i]);
    //     m_LightingShader->setVec3(lightPositionID + ".ambient", 0.05f, 0.05f, 0.05f);
    //     m_LightingShader->setVec3(lightPositionID + ".diffuse", 0.8f, 0.8f, 0.8f);
    //     m_LightingShader->setVec3(lightPositionID + ".specular", 1.0f, 1.0f, 1.0f);

    //     m_LightingShader->setFloat(lightPositionID + ".constant",  1.0f);
    //     m_LightingShader->setFloat(lightPositionID + ".linear",    0.09f);
    //     m_LightingShader->setFloat(lightPositionID + ".quadratic", 0.032f);
    // }

    // m_LightingShader->setVec3("directionalLight.direction", -0.2f, -1.0f, -0.3f);
    // m_LightingShader->setVec3("directionalLight.ambient", 0.05f, 0.05f, 0.05f);
    // m_LightingShader->setVec3("directionalLight.diffuse", 0.4f, 0.4f, 0.4f);
    // m_LightingShader->setVec3("directionalLight.specular", 0.5f, 0.5f, 0.5f);

    // m_LightingShader->setFloat("material.shininess", 64.0f);

    // m_LightingShader->setVec3("spotlight.ambient", 0.05f, 0.05f, 0.05f);
    // m_LightingShader->setVec3("spotlight.diffuse", 0.8f, 0.8f, 0.8f);
    // m_LightingShader->setVec3("spotlight.specular", 1.0f, 1.0f, 1.0f);
    // m_LightingShader->setFloat("spotlight.constant",  1.0f);
    // m_LightingShader->setFloat("spotlight.linear",    0.09f);
    // m_LightingShader->setFloat("spotlight.quadratic", 0.032f);
    // m_LightingShader->setVec3("spotlight.position", m_Camera->getPosition());
    // m_LightingShader->setVec3("spotlight.direction", m_Camera->getDirection());
    // m_LightingShader->setFloat("spotlight.radius",      glm::cos(glm::radians(12.5f)));
    // m_LightingShader->setFloat("spotlight.outerRadius", glm::cos(glm::radians(17.5f)));

    // m_LightingShader->setVec3("cameraPosition", m_Camera->getPosition());

    // m_LightingShader->setMat4("view", view);
    // m_LightingShader->setMat4("projection", projection);

    // for (int i = 0; i < 10; ++i) {
    //     glm::mat4 model(1.0f);
    //     model = glm::translate(model, m_CubePositions[i]);
    //     float angle = 20.0f * i;
    //     model = glm::rotate(model,
    //                         glm::radians(angle),
    //                         glm::vec3(1.0f, 0.3f, 0.5f));
    //     m_LightingShader->setMat4("model", model);
    //     m_LitCube->draw();
    // }

    // Teapot avec basic engine
    m_BasicShader->use();
    glm::mat4 model(1.0f);
    m_BasicShader->setMat4("model", model);
    m_BasicShader->setMat4("view", view);
    m_BasicShader->setMat4("projection", projection);

    m_Teapot->draw();
}
