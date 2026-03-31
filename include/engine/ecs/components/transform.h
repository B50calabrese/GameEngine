/**
 * @file transform.h
 * @brief Common transform component.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_TRANSFORM_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_TRANSFORM_H_

#include <glm/glm.hpp>

namespace engine::ecs::components {

/**
 * @brief Component representing an entity's position, rotation, and scale.
 */
struct Transform {
  glm::vec2 position = {0.0f, 0.0f};
  glm::vec2 scale = {1.0f, 1.0f};
  float rotation = 0.0f;  // Degrees
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_TRANSFORM_H_
