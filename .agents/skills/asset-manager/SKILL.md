---
id: asset-manager
subsystem: util
version: 0.1.0
author_persona: Senior Systems Architect
---

# Asset Management Skill (STUB)

<guiding_principles>
- **Centralized Ownership**: Assets (textures, fonts, shaders) must be managed by the `AssetManager<T>` to avoid redundant loading and to centralize lifecycle management.
- **Thread Safety**: Access to the `AssetManager` must be thread-safe for parallel resource loading.
</guiding_principles>

## Subsystem Architecture

- `AssetManager<T>`: A template-based singleton for managing assets of type `T`. It uses `std::weak_ptr` for caching and `std::shared_ptr` for ownership.

## Gotchas

- **Resource Leakage**: Ensure assets are correctly disposed of when no longer needed by managing their `shared_ptr` lifecycle.
- **Reference Counting**: Circular references or held `shared_ptr`s can prevent assets from being unloaded.

## Validation

(TBD) Refer to `include/engine/util/asset_manager.h`.
