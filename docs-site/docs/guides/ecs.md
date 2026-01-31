---
title: Entity Component System
sidebar_position: 4
---

This guide explains the ECS concepts used by LambEngine.

## Entities

Entities are lightweight identifiers. They should not own logic or resources.

## Components

Components are plain data structures. Keep them small and focused to improve
cache behavior.

## Systems

Systems operate on component sets and implement game logic. Keep them orderable
and deterministic where possible.

## Update order

- Input and events
- Simulation systems
- Render preparation

## TODO

- Document the ECS API and component registration flow.
- Add examples for Transform, Camera, and Renderable components.
