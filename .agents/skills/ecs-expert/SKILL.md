---
id: ecs-expert
subsystem: ecs
version: 1.1.0
author_persona: Senior Systems Architect
---

# Entity Component System (ECS) Skill

<guiding_principles>
- **Registry as Source of Truth**: All entity and component lifecycles must be managed exclusively through the `engine::ecs::Registry`.
- **Data-Oriented Composition**: Components should be Plain Old Data (POD) structures. Systems should operate on `Registry::View` to maximize cache hits and maintain clear separation of data and logic.
- **Pointer-Free Component Storage**: Never store pointers or long-lived references to components, as the underlying `ComponentStorage` may reallocate memory.
- **Transform-First Architecture**: Almost all entities should have a `TransformComponent` (engine/core/transform.h). Systems like `PhysicsSystem` and `SpriteRenderSystem` rely on it.
</guiding_principles>

## Contextual Instructions

### 🛠️ Engine Contributor Context (Core Development)
- **Goal**: Maintain and optimize `ComponentStorage` or the `Registry`.
- **Constraint**: Ensure `ComponentStorage` remains a contiguous block of memory.
- **Gotcha**: Adding a component type for the first time lazily initializes a `ComponentStorage` instance. Be wary of thread safety in `Registry::GetStorage<T>`.

### 🎮 Game Developer Context (Application Logic)
- **Goal**: Implement game logic using the ECS API.
- **Constraint**: Use `Registry::View` for all entity iteration. Avoid manual `ComponentStorage` access.
- **Golden Sample**: <golden_sample file="demos/ecsdemo/main.cpp" />

## Subsystem Architecture

The ECS is built around three core classes:
- `EntityManager`: Manages the creation, destruction, and recycling of `EntityID` values.
- `ComponentStorage<T>`: A contiguous array-based storage for components of type `T`.
- `Registry`: The main coordinator that links entities to their components and provides the `View` API for iteration.

## Gotchas

- **System Ordering**: The `Application` loop processes `PhysicsSystem` before `OnUpdate`, and `SpriteRenderSystem` after. Ensure game logic in `OnUpdate` accounts for this (e.g., input should set velocity, which is then integrated by physics).
- **Reference Invalidation**: Storing a pointer or reference to a component across multiple `Registry` operations (like `AddComponent` or `DeleteEntity`) is strictly forbidden. Always re-fetch the component using `GetComponent<T>(entity)` if needed after a registry modification.
- **Deferred Destruction**: Removing components or destroying entities during a `ForEach` or `View` loop can invalidate iterators or lead to processing "ghost" entities. Prefer marking entities for destruction and processing deletions at the end of the frame.
- **Entity ID Recycling**: The `EntityManager` may recycle IDs after an entity is destroyed. Systems must not assume an ID's permanence across long durations (e.g., multiple scenes) without validation via `IsAlive(entity)`.

## Validation

To verify changes to the ECS subsystem, refer to the validation guide in `scripts/validate.sh`.
