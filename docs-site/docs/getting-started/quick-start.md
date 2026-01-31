---
title: Quick Start
sidebar_position: 2
---

This is a minimal setup to run a game loop. Adjust namespaces and include paths
based on your project layout.

## 1. Create a game class

```cpp
#include "LambEngine/Engine.hpp"
#include "LambEngine/IGame.hpp"

class MyGame final : public IGame {
public:
  void OnInit() override;
  void OnUpdate(float deltaTime) override;
  void OnRender() override;
};
```

## 2. Boot the engine

```cpp
int main() {
  Engine engine;
  engine.Run(new MyGame());
  return 0;
}
```

## 3. Next steps

- Initialize your renderer in `OnInit`.
- Register systems and entities in `OnInit`.
- Update simulation in `OnUpdate` and submit render work in `OnRender`.

## TODO

- Replace this example with the real include paths and namespaces.
- Add a minimal asset load and camera setup example.
