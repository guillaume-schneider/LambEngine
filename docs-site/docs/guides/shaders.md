---
title: Shaders
sidebar_position: 2
---

This guide explains how shaders are authored and integrated into LambEngine.

## Shader layout

- Keep shader sources in `shaders/`.
- Organize by stage (vertex, fragment) or feature (pbr, shadow, ui).
- Keep shared includes in a dedicated folder (for example, `shaders/common`).

## Compilation and reload

Provide a single entry point for compiling shader programs. If hot reload is
supported, ensure file watchers map to shader variants deterministically.

## Naming conventions

- Use consistent naming for uniforms and bindings.
- Keep per-frame constants separated from per-object constants.

## TODO

- Document the engine shader wrapper API.
- Add a minimal shader example with uniforms and textures.
