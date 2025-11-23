#pragma once

#include "string"
#include <SDL2/SDL.h>

struct EngineConfig {
    unsigned int width = 1980;
    unsigned int height = 1080;
    std::string title = "My Engine";
    bool vsync = true;
    bool enablePhysics = false;
    bool enableImGui = true;
};

class IGame;

class Engine {
public:
    explicit Engine(const EngineConfig& cfg);
    ~Engine();

    void Run(IGame* game);

private:
    void initSDL(const EngineConfig& cfg);
    void initOpenGL();
    void initImGui();
    void shutdownImGui();
    void shutdownSDL();

    EngineConfig m_Config;

    SDL_Window*   m_Window = nullptr;
    SDL_GLContext m_Context = nullptr;
    float         m_AspectRatio = 16.0f / 9.0f;
};

