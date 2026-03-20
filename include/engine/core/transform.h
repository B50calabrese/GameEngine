/**
 * @file transform.h
 * @brief Common transform component.
 */

#ifndef INCLUDE_ENGINE_CORE_TRANSFORM_H_
#define INCLUDE_ENGINE_CORE_TRANSFORM_H_

#include <glm/glm.hpp>

namespace engine::core {

/**
 * @brief Component representing an entity's position, rotation, and scale in world space.
 */
struct TransformComponent {
  glm::vec2 position = {0.0f, 0.0f};
  glm::vec2 scale = {1.0f, 1.0f};
  float rotation = 0.0f; // In degrees
};

}  // namespace engine::core

#endif  // INCLUDE_ENGINE_CORE_TRANSFORM_H_
