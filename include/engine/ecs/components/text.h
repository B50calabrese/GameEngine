/**
 * @file text.h
 * @brief Component for rendering text.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_TEXT_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_TEXT_H_

#include <string>

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for rendering text. */
struct Text {
  std::string content;
  std::string font_name = "default";
  float scale = 1.0f;
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  float z_index = 0.0f;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_TEXT_H_
