/**
 * @file sprite.h
 * @brief Component for rendering a texture.
 */

#ifndef INCLUDE_ENGINE_ECS_COMPONENTS_SPRITE_H_
#define INCLUDE_ENGINE_ECS_COMPONENTS_SPRITE_H_

#include <string>

#include <glm/glm.hpp>

namespace engine::ecs::components {

/** @brief Component for rendering a texture. */
struct Sprite {
  std::string texture_name = "";
  std::string sprite_sheet_name = "";
  int sprite_index = 0;
  glm::vec4 tint = {1.0f, 1.0f, 1.0f, 1.0f};
  glm::vec2 origin = {0.0f, 0.0f};
  float z_index = 0.0f;
  bool visible = true;
};

}  // namespace engine::ecs::components

#endif  // INCLUDE_ENGINE_ECS_COMPONENTS_SPRITE_H_
