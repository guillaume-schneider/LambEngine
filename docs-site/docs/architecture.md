---
title: Architecture Overview
sidebar_position: 2
---

This page describes the high-level architecture of LambEngine. Use it to
understand which modules depend on each other, and where to place new features.

## Core modules

- Core: engine lifecycle, logging, timing, and service bootstrap.
- Renderer: frame graph, render passes, materials, and GPU resources.
- ECS: entities, components, and system execution.
- Input: device polling, actions, and callbacks.
- Resources: asset loading, caching, and hot reload.

## Runtime flow (typical frame)

1. Poll platform events and input.
2. Update simulation and ECS systems.
3. Prepare render data (culling, sorting, staging).
4. Execute render passes.
5. Present the final frame.

## Data ownership

Keep data ownership explicit. Components should be data-only and owned by the
ECS. Renderer modules should consume immutable render data derived from ECS.

## Dependency rules

- Core is the only module allowed to be referenced everywhere.
- Renderer should not depend on Input or ECS directly.
- ECS systems should depend on Renderer and Input through interfaces.

## TODO

- Add a diagram showing module dependencies.
- Document the threading model once finalized.
- Document resource lifetime and hot reload behavior.
