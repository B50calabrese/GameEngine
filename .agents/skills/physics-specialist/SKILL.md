---
id: physics-specialist
subsystem: physics
version: 1.0.0
author_persona: Senior Systems Architect
---

# Physics Specialist Skill

<guiding_principles>
- **Component-Driven Simulation**: Use `VelocityComponent`, `GravityComponent`, and `ColliderComponent` to define physical behavior.
- **AABB Centric**: The engine currently only supports Axis-Aligned Bounding Box (AABB) collision detection and resolution.
- **Resolution vs Detection**: Use `is_static` for immovable objects and `is_trigger` for detection-only logic (callbacks).
- **Callback Safety**: Collision callbacks (`on_collision`) are fired during the `PhysicsSystem::Update` pass. Avoid modifying the registry in ways that invalidate the current iteration inside these callbacks.
</guiding_principles>

## Contextual Instructions

### 🎮 Game Developer Context (Application Logic)
- **Goal**: Implement character movement and environmental interaction.
- **Constraint**: Set `velocity` in `OnUpdate` based on input; let the `PhysicsSystem` handle the integration and resolution.
- **Trigger Usage**: Use `is_trigger = true` for collectibles, goals, or areas where you want to handle the response manually without physical push-back.
- **Golden Sample**: <golden_sample file="demos/games/platformer/gameplay_scene.cpp" />

## Subsystem Architecture

The physics pipeline is integrated into the `Application` loop:
- `PhysicsSystem`: A static system that processes movement and AABB resolution in two passes (Horizontal then Vertical) to handle axis-aligned sliding and stepping.
- `ColliderComponent`: Defines the bounds (size and offset relative to `Transform`) and behavior (static, trigger) of an entity.

## Gotchas

- **Resolution Invalidation**: If you manually set an entity's `position` in `OnUpdate`, you might override the `PhysicsSystem`'s resolution (which runs earlier in the frame).
- **Trigger Redundancy**: The `on_collision` callback may be fired multiple times per frame if an entity overlaps in both the horizontal and vertical resolution passes.
- **Static Resolution**: Static colliders (`is_static = true`) will never be moved by the physics system, even if they have a velocity or gravity component.

## Validation

To verify changes to the Physics subsystem, refer to the validation guide in `scripts/validate.sh`.
