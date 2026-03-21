/**
 * @file sprite_render_system.h
 * @brief Automated sprite rendering system.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_SPRITE_RENDER_SYSTEM_H_
#define INCLUDE_ENGINE_GRAPHICS_SPRITE_RENDER_SYSTEM_H_

#include <engine/ecs/registry.h>

namespace engine::graphics {

/**
 * @brief System that renders entities with Transform and Sprite components.
 */
class SpriteRenderSystem {
 public:
  /**
   * @brief Renders all visible sprites.
   * @param registry The ECS registry.
   */
  static void Render(ecs::Registry* registry);
};

}  // namespace engine::graphics

#endif  // INCLUDE_ENGINE_GRAPHICS_SPRITE_RENDER_SYSTEM_H_
