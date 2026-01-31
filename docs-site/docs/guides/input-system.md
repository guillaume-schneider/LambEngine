---
title: Input System
sidebar_position: 3
---

The input system translates device events into actions the game can consume.

## Concepts

- Devices: keyboard, mouse, and gamepad inputs.
- Actions: logical bindings that drive gameplay.
- Callbacks: event handlers for immediate responses.

## Suggested flow

1. Define actions and bindings during initialization.
2. Poll or dispatch input events each frame.
3. Query action state in gameplay systems.

## TODO

- Document the input mapping API once finalized.
- Add examples for camera controls and UI focus.
