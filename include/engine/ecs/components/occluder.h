/**
 * @file occluder.h
 * @brief Component for an object that casts shadows.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_OCCLUDER_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_OCCLUDER_H_

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for an object that casts shadows. */
struct Occluder {
  glm::vec2 size = {32.0f, 32.0f};
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_OCCLUDER_H_
