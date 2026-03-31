/**
 * @file graphics_components.h
 * @brief Common components for graphics.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_GRAPHICS_COMPONENTS_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_GRAPHICS_COMPONENTS_H_

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

/** @brief Component for rendering a simple colored quad. */
struct Quad {
  glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
  float z_index = 0.0f;
};

/** @brief Component for rendering a texture. */
struct Sprite {
  std::string texture_name;
  std::string sprite_sheet_name;
  int sprite_index = 0;
  glm::vec4 tint = {1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec2 origin = {0.0f, 0.0f};
  float z_index = 0.0f;
  bool visible = true;
};

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

/** @brief Component for an object that casts shadows. */
struct Occluder {
  glm::vec2 size = {32.0f, 32.0f};
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_GRAPHICS_COMPONENTS_H_
