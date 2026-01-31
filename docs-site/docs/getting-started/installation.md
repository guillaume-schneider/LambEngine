---
title: Installation
sidebar_position: 1
---

This page covers the build requirements and the recommended setup flow.

## Requirements

- CMake 3.23 or newer.
- A C++20 compiler (Visual Studio 2022 is the default preset).
- vcpkg for third-party dependencies.

LambEngine uses the following libraries via vcpkg:

- SDL2
- OpenGL
- glad
- assimp
- glm
- imgui
- nlohmann-json
- stb

## Setup (Windows, Visual Studio presets)

1. Clone the repository and initialize vcpkg.
2. Set the `VCPKG_ROOT` environment variable.
3. Configure using a preset:

```powershell
cmake --preset debug
```

4. Build the project:

```powershell
cmake --build --preset debug
```

5. Run the executable from `build\bin`.

## Build presets

- `debug`: Debug build with tests enabled.
- `release`: Release build with tests disabled.
- `ci-debug` and `ci-release`: Presets for automation.

## Troubleshooting

- If CMake cannot find a package, verify `VCPKG_ROOT` is set.
- If you use another generator, create a new preset in `CMakePresets.json`.
