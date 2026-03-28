/**
 * @file sprite_render_system.cpp
 * @brief Implementation of the sprite render system.
 */

#include <engine/core/transform.h>
#include <engine/graphics/graphics_components.h>
#include <engine/graphics/render_queue.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_render_system.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/texture.h>
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
          if (sheet && sheet->texture()) {
            glm::vec2 uv_min, uv_max;
            sheet->GetUVs(sprite.sprite_index, &uv_min, &uv_max);

            RenderCommand cmd;
            cmd.z_order = sprite.z_index;
            cmd.texture_id = sheet->texture()->renderer_id();
            cmd.position = transform.position;
            cmd.size = transform.scale;
            cmd.rotation = transform.rotation;
            cmd.color = sprite.tint;
            cmd.uv_min = uv_min;
            cmd.uv_max = uv_max;
            cmd.origin = sprite.origin;
            RenderQueue::Default().Submit(cmd);
          }
        } else if (!sprite.texture_name.empty()) {
          auto tex = util::AssetManager<Texture>::Get(sprite.texture_name);
          if (tex) {
            RenderCommand cmd;
            cmd.z_order = sprite.z_index;
            cmd.texture_id = tex->renderer_id();
            cmd.position = transform.position;
            cmd.size = transform.scale;
            cmd.rotation = transform.rotation;
            cmd.color = sprite.tint;
            cmd.uv_min = {0.0f, 0.0f};
            cmd.uv_max = {1.0f, 1.0f};
            cmd.origin = sprite.origin;
            RenderQueue::Default().Submit(cmd);
          }
        }
      }
    }
    // Quad Component (fallback)
    else if (registry->HasComponent<QuadComponent>(entity)) {
      auto& quad = registry->GetComponent<QuadComponent>(entity);
      RenderCommand cmd;
      cmd.z_order = quad.z_index;
      cmd.texture_id = 0;  // White texture slot
      cmd.position = transform.position;
      cmd.size = transform.scale;
      cmd.rotation = transform.rotation;
      cmd.color = quad.color;
      RenderQueue::Default().Submit(cmd);
    }

    // Text Component (can be combined with Sprite/Quad)
    if (registry->HasComponent<TextComponent>(entity)) {
      auto& text = registry->GetComponent<TextComponent>(entity);
      Renderer::Get().DrawText(text.font_name, text.content, transform.position,
                               transform.rotation, text.scale, text.color,
                               text.z_index);
    }
  }
}

}  // namespace engine::graphics
