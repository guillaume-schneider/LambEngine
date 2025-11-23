#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include <mtl_parser.hpp>
#include <material.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Core/IGame.hpp"
#include "Core/MyGame.hpp"
#include "Core/engine.hpp"

#include "log.hpp"

int main(int argc, char *argv[])
{
    Logger::Init(Logger::Mode::File, "engine.log");

    MyGame game;
    EngineConfig cfg;
    Engine engine{cfg};
    engine.Run(&game);

    return 0;
}
