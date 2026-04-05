/**
 * @file collider.h
 * @brief Basic collider for detection and resolution.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_COLLIDER_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_COLLIDER_H_

#include <functional>

#include <glm/vec2.hpp>

#include <engine/ecs/entity_manager.h>

namespace engine::ecs::components {

/** @brief Basic collider for detection and resolution. */
struct Collider {
  glm::vec2 size = {1.0f, 1.0f};
  glm::vec2 offset = {0.0f, 0.0f};
  bool is_static = false;
  bool is_trigger = false;

  // Callback for collision events
  std::function<void(ecs::EntityID, ecs::EntityID)> on_collision;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_COLLIDER_H_
