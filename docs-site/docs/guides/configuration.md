---
title: Configuration
sidebar_position: 5
---

This guide describes how configuration is stored and loaded.

## File formats

LambEngine uses JSON configuration files via nlohmann-json. Keep configs in the
`config/` folder and avoid embedding hard-coded constants in code.

## Suggested categories

- Renderer settings (resolution, vsync, MSAA)
- Asset paths and search roots
- Input bindings and controller profiles

## TODO

- Document the configuration loader API.
- Add a sample configuration file with defaults.
