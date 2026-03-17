---
id: ecs-expert
subsystem: ecs
version: 1.0.0
author_persona: Senior Systems Architect
---

# Entity Component System (ECS) Skill

<guiding_principles>
- **Registry as Source of Truth**: All entity and component lifecycles must be managed exclusively through the `engine::ecs::Registry`.
- **Data-Oriented Composition**: Components should be Plain Old Data (POD) structures. Systems should operate on `Registry::View` to maximize cache hits and maintain clear separation of data and logic.
- **Snapshot Iteration**: Views are constructed as snapshots; logic should be designed to handle entities that may have been modified within the same frame but after view creation.
- **Pointer-Free Component Storage**: Never store pointers or long-lived references to components, as the underlying `ComponentStorage` may reallocate memory.
</guiding_principles>

## Subsystem Architecture

The ECS is built around three core classes:
- `EntityManager`: Manages the creation, destruction, and recycling of `EntityID` values.
- `ComponentStorage<T>`: A contiguous array-based storage for components of type `T`.
- `Registry`: The main coordinator that links entities to their components and provides the `View` API for iteration.

## Golden Sample

Proper usage of the `Registry` for component addition and view-based system iteration can be found in:
<golden_sample file="demos/ecsdemo/main.cpp" />

## Gotchas

- **Reference Invalidation**: Storing a pointer or reference to a component across multiple `Registry` operations (like `AddComponent` or `DeleteEntity`) is strictly forbidden. Always re-fetch the component using `GetComponent<T>(entity)` if needed after a registry modification.
- **Deferred Destruction**: Removing components or destroying entities during a `ForEach` or `View` loop can invalidate iterators or lead to processing "ghost" entities. Prefer marking entities for destruction and processing deletions at the end of the frame.
- **Entity ID Recycling**: The `EntityManager` may recycle IDs after an entity is destroyed. Systems must not assume an ID's permanence across long durations (e.g., multiple scenes) without validation via `IsAlive(entity)`.
- **View Performance**: Creating a `View` involves iterating over all entities. For performance-critical systems, consider caching views or using more specialized query mechanisms if they become available.

## Validation

To verify changes to the ECS subsystem, refer to the validation guide in `scripts/validate.sh`.
