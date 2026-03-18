---
id: asset-manager
subsystem: util
version: 0.2.0
author_persona: Senior Systems Architect
---

# Asset Management Skill (STUB)

<guiding_principles>
- **Centralized Ownership**: Assets (textures, fonts, shaders) must be managed by the `AssetManager<T>` to avoid redundant loading and to centralize lifecycle management.
- **Thread Safety**: Access to the `AssetManager` must be thread-safe for parallel resource loading.
</guiding_principles>

## Contextual Instructions

### 🛠️ Engine Contributor Context (Core Development)
- **Goal**: Add support for new asset types or improve caching.
- **Constraint**: Ensure thread safety using the static mutex per asset type.
- **Gotcha**: The static `Load` method in asset classes must remain compatible with the `AssetManager` template.

### 🎮 Game Developer Context (Application Logic)
- **Goal**: Load and retrieve game assets.
- **Constraint**: Use `AssetManager<T>::Get()` to fetch shared pointers. Never hold long-lived `shared_ptr`s to assets globally; prefer local scope or component-bound lifetimes.
- **Golden Sample**: <golden_sample file="include/engine/util/asset_manager.h" />

## Subsystem Architecture

- `AssetManager<T>`: A template-based singleton for managing assets of type `T`. It uses `std::weak_ptr` for caching and `std::shared_ptr` for ownership.

## Gotchas

- **Resource Leakage**: Ensure assets are correctly disposed of when no longer needed by managing their `shared_ptr` lifecycle.
- **Reference Counting**: Circular references or held `shared_ptr`s can prevent assets from being unloaded.

## Validation

(TBD) Refer to `include/engine/util/asset_manager.h`.
