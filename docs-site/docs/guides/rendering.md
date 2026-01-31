---
title: Rendering
sidebar_position: 1
---

This guide covers how the rendering system is organized, from scene data to
frame output.

## Pipeline overview

- Collect renderable data from ECS.
- Build draw lists and GPU buffers.
- Execute render passes (geometry, lighting, post).
- Present the final color buffer.

## Materials and textures

Define materials as small, immutable objects that reference shader programs and
texture sets. Keep material creation centralized to avoid redundant GPU state.

## Models and meshes

Use a loader to import mesh data into GPU buffers. The engine uses assimp for
model imports through vcpkg. Store model metadata alongside GPU handles.

## Cameras

Cameras should provide view and projection matrices, plus a clear ownership
model (player camera vs. editor camera).

## TODO

- Add a frame diagram and render pass ordering.
- Document material parameter binding conventions.
- Add examples for model loading and texture creation.
