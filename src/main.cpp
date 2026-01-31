#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <material.hpp>
#include <mtl_parser.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "Core/IGame.hpp"
#include "Core/MyGame.hpp"
#include "Core/engine.hpp"
#include "log.hpp"
#include "stb_image.h"

int main(int argc, char* argv[])
{
    Logger::Init(false, true, false);

    Logger::RegisterSubsystemFile("Global", "Logs/Global.log");
    Logger::RegisterSubsystemFile("Engine", "Logs/Engine/Engine.log");
    Logger::RegisterSubsystemFile("Renderer", "Logs/Renderer/Renderer.log");
    Logger::RegisterSubsystemFile("Physics", "Logs/Physics/Physics.log");

    MyGame game;
    EngineConfig cfg;
    Engine engine{cfg};
    engine.Run(&game);

    Logger::Shutdown();

    return 0;
}
