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
  std::string texture_name = "";
  std::string sprite_sheet_name = "";
  int sprite_index = 0;
  glm::vec4 tint = {1.0f, 1.0f, 1.0f, 1.0f};
  bool visible = true;
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_GRAPHICS_COMPONENTS_H_
