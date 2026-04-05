/**
 * @file velocity.h
 * @brief Component for velocity in world space.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_VELOCITY_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_VELOCITY_H_

#include <glm/vec2.hpp>

namespace engine::ecs::components {

/** @brief Component for velocity in world space. */
struct Velocity {
  glm::vec2 velocity = {0.0f, 0.0f};
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_VELOCITY_H_
