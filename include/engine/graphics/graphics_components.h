/**
 * @file graphics_components.h
 * @brief Common components for graphics.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_GRAPHICS_COMPONENTS_H_
#define INCLUDE_ENGINE_GRAPHICS_GRAPHICS_COMPONENTS_H_

#include <string>

#include <glm/glm.hpp>

namespace engine::graphics {

/** @brief Component for rendering text. */
struct TextComponent {
  std::string content;
  std::string font_name = "default";
  float scale = 1.0f;
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
};

/** @brief Component for rendering a simple colored quad. */
struct QuadComponent {
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
};

/** @brief Component for rendering a texture. */
struct SpriteComponent {
  unsigned int texture_id = 0;
  glm::vec4 tint = {1.0f, 1.0f, 1.0f, 1.0f};
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_GRAPHICS_COMPONENTS_H_
