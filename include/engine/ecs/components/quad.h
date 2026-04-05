/**
 * @file quad.h
 * @brief Component for rendering a simple colored quad.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_QUAD_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_QUAD_H_

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for rendering a simple colored quad. */
struct Quad {
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec2 origin = {0.0f, 0.0f};
  float z_index = 0.0f;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_QUAD_H_
