#pragma once

#include "engine.hpp"

class Engine;

class IGame
{
public:
    virtual ~IGame() = default;
    virtual void OnInit(Engine& engine) {}
    virtual void OnUpdate(Engine& engine, float dt) {}
    virtual void OnRender(Engine& engine) {}
};
