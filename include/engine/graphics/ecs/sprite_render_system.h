/**
 * @file sprite_render_system.h
 * @brief Automated sprite rendering system.
 */

#ifndef INCLUDE_ENGINE_GRAPHICS_ECS_SPRITE_RENDER_SYSTEM_H_
#define INCLUDE_ENGINE_GRAPHICS_ECS_SPRITE_RENDER_SYSTEM_H_

#include <engine/ecs/registry.h>

namespace engine::graphics::ecs {

/**
 * @brief System that renders entities with Transform and Sprite components.
 */
class SpriteRenderSystem {
 public:
  /**
   * @brief Renders all visible sprites.
   * @param registry The ECS registry.
   */
  static void Render(engine::ecs::Registry* registry);
};

}  // namespace engine::graphics::ecs

#endif  // INCLUDE_ENGINE_GRAPHICS_ECS_SPRITE_RENDER_SYSTEM_H_
