---
title: Project Structure
sidebar_position: 3
---

This page explains the repository layout and where to place new code or assets.

## Root layout

- `src/` Engine source code.
- `tests/` Unit tests and integration tests.
- `shaders/` GPU shader sources.
- `res/` Runtime assets (models, textures, data).
- `config/` Engine configuration and data files.
- `docs/` Markdown documentation drafts.
- `build/` CMake output (generated).

## Tips

- Keep renderer assets in `res/` and authoring shaders in `shaders/`.
- Avoid committing `build/` output to source control.
- Create new modules under `src/` and expose public headers from a clear
  top-level folder.

## TODO

- Document the public header layout and naming conventions.
- Add a guide for adding a new engine module.
