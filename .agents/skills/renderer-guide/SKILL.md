---
id: renderer-guide
subsystem: renderer
version: 1.0.0
author_persona: Senior Systems Architect
---

# Renderer Skill

<guiding_principles>
- **Queue-First Submission**: Logic should prioritize `RenderQueue::Submit` over direct `Renderer` calls to leverage automatic Z-sorting and texture-based batching.
- **Coordinate Consistency**: All spatial logic must adhere to the (0,0) bottom-left origin to remain compatible with the engine's orthographic projection.
- **Lifecycle Encapsulation**: Post-processing and state management must be wrapped within `BeginFrame` and `EndFrame` blocks.
- **Batched Rendering**: Avoid redundant `Flush` calls. Batch similar drawing commands whenever possible to minimize draw calls.
</guiding_principles>

## Subsystem Architecture

The rendering pipeline is designed for high-performance 2D drawing:
- `Renderer`: A singleton that manages the OpenGL context and provides the core primitive and textured quad drawing API.
- `RenderQueue`: A sorting and optimization layer that collects `RenderCommand` objects, sorts them by Z-order and Texture ID, and flushes them to the `Renderer`.
- `PostProcessManager`: A singleton that manages a full-screen post-processing pipeline (e.g., screen shake, flash overlays).

## Golden Samples

Proper usage of the `Renderer` and `RenderQueue` can be found in:
- <golden_sample file="demos/rendererdemo/main.cpp" /> - Basic Renderer API usage.
- <golden_sample file="demos/renderqueuedemo/main.cpp" /> - Optimized batching via `RenderQueue`.

## Gotchas

- **Context Dependency**: Any `Renderer` call made before `Window` initialization (OpenGL context creation) or after `Shutdown` will trigger an OpenGL driver crash or undefined behavior.
- **Batch Breaking**: Mixing `DrawRect` and `DrawTexturedQuad` with inconsistent Z-ordering can force redundant `Flush` calls, significantly degrading performance due to texture swaps.
- **Z-Fighting**: Transparent overlays require distinct Z-order increments. Identical Z-values for overlapping textures lead to non-deterministic flickering (z-fighting).
- **Framebuffer Resize**: When the window is resized, both the `Renderer` viewport and any `Framebuffer` objects (used in `PostProcessManager`) must be resized to prevent distortion.

## Validation

To verify changes to the Renderer subsystem, refer to the validation guide in `scripts/validate.sh`.
