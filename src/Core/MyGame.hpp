// MyGame.hpp
#pragma once

#include "IGame.hpp"
#include <glm/glm.hpp>
#include <array>

// Forward declarations pour éviter les includes lourds ici
class ShaderEngine;
class Camera;
class Cube;
class Sphere;
class Model;

class MyGame : public IGame {
public:
    void OnInit(Engine& engine) override;
    void OnUpdate(Engine& engine, float dt) override;
    void OnRender(Engine& engine) override;

private:
    // Shaders
    ShaderEngine* m_LightingShader = nullptr;
    ShaderEngine* m_LightShader    = nullptr;
    ShaderEngine* m_BasicShader    = nullptr;

    // Géométrie
    Cube*  m_LightCube      = nullptr;
    Cube*  m_LitCube        = nullptr;
    Sphere* m_Sphere        = nullptr;
    Model* m_Teapot         = nullptr;

    // Caméra
    Camera* m_Camera        = nullptr;

    // Positions
    std::array<glm::vec3, 4>  m_PointLightPositions;
    std::array<glm::vec3, 10> m_CubePositions;

    float m_CurrentAspectRatio = 16.0f / 9.0f;
};
