/**
 * @file physics_system.h
 * @brief System for handling physics and collisions.
 */

#ifndef INCLUDE_ENGINE_ECS_SYSTEMS_PHYSICS_SYSTEM_H_
#define INCLUDE_ENGINE_ECS_SYSTEMS_PHYSICS_SYSTEM_H_

#include <engine/ecs/registry.h>

namespace engine::ecs::systems {

/**
 * @brief Processes physics components and handles simple AABB collision
 * resolution.
 */
class PhysicsSystem {
 public:
  /**
   * @brief Updates the physics state for all relevant entities.
   * @param registry The ECS registry containing entities to update.
   * @param dt Delta time since last frame.
   */
  static void Update(Registry* registry, float dt);

 private:
  /** @brief Resolves collisions between two entities. */
  static void ResolveCollision(Registry& registry, EntityID entity_a,
                               EntityID entity_b);
};

}  // namespace engine::ecs::systems

#endif  // INCLUDE_ENGINE_ECS_SYSTEMS_PHYSICS_SYSTEM_H_
