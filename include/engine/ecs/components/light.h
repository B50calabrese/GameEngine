/**
 * @file light.h
 * @brief Component for a light source.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_LIGHT_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_LIGHT_H_

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for a light source. */
struct Light {
  glm::vec3 color = {1.0f, 1.0f, 1.0f};
  float intensity = 1.0f;
  float radius = 100.0f;
  float angle = 360.0f;
  float direction = 0.0f;
  bool is_directional = false;
  glm::vec2 dir_vector = {0.0f, -1.0f};
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_LIGHT_H_
