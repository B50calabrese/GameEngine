/**
 * @file sprite_render_system.cpp
 * @brief Implementation of the sprite render system.
 */

#include <engine/core/transform.h>
#include <engine/graphics/graphics_components.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_render_system.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/util/asset_manager.h>

namespace engine::graphics {

void SpriteRenderSystem::Render(ecs::Registry* registry) {
  if (!registry) {
    return;
  }
  // 1. Render entities with Transform and Sprite/Quad
  auto trans_view = registry->GetView<core::TransformComponent>();
  for (auto entity : trans_view) {
    auto& transform = registry->GetComponent<core::TransformComponent>(entity);

    // Sprite Component (takes priority)
    if (registry->HasComponent<SpriteComponent>(entity)) {
      auto& sprite = registry->GetComponent<SpriteComponent>(entity);
      if (sprite.visible) {
        if (!sprite.sprite_sheet_name.empty()) {
          auto sheet =
              util::AssetManager<SpriteSheet>::Get(sprite.sprite_sheet_name);
          if (sheet) {
            Renderer::Get().DrawSprite(sheet.get(), sprite.sprite_index,
                                       transform.position, transform.scale,
                                       transform.rotation, sprite.tint, {0, 0});
          }
        }
      }
    }
    // Quad Component (fallback)
    else if (registry->HasComponent<QuadComponent>(entity)) {
      auto& quad = registry->GetComponent<QuadComponent>(entity);
      Renderer::Get().DrawQuad(transform.position, transform.scale, quad.color,
                               transform.rotation);
    }

    // Text Component (can be combined with Sprite/Quad)
    if (registry->HasComponent<TextComponent>(entity)) {
      auto& text = registry->GetComponent<TextComponent>(entity);
      Renderer::Get().DrawText(text.font_name, text.content, transform.position,
                               transform.rotation, text.scale, text.color);
    }
  }
}

}  // namespace engine::graphics
