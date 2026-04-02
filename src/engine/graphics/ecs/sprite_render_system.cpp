/**
 * @file sprite_render_system.cpp
 * @brief SpriteRenderSystem implementation.
 */

#include <engine/ecs/components/graphics_components.h>
#include <engine/ecs/components/transform.h>
#include <engine/graphics/ecs/sprite_render_system.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/utils/render_queue.h>
#include <engine/util/asset_manager.h>

namespace engine::graphics::ecs {

void SpriteRenderSystem::Render(engine::ecs::Registry* registry) {
  if (!registry) return;

  // Render Quads
  registry->ForEach<engine::ecs::components::Transform,
                    engine::ecs::components::Quad>(
      [](engine::ecs::components::Transform& t,
         engine::ecs::components::Quad& q) {
        utils::RenderCommand cmd;
        cmd.position = t.position;
        cmd.size = t.scale;
        cmd.color = q.color;
        cmd.rotation = t.rotation;
        cmd.z_order = q.z_index;
        utils::RenderQueue::Default().Submit(cmd);
      });

  // Render Sprites
  registry->ForEach<engine::ecs::components::Transform,
                    engine::ecs::components::Sprite>(
      [](engine::ecs::components::Transform& t,
         engine::ecs::components::Sprite& s) {
        if (!s.visible) return;

        utils::RenderCommand cmd;
        cmd.position = t.position;
        cmd.size = t.scale;
        cmd.color = s.tint;
        cmd.rotation = t.rotation;
        cmd.origin = s.origin;
        cmd.z_order = s.z_index;

        if (!s.sprite_sheet_name.empty()) {
          auto sheet = util::AssetManager<graphics::SpriteSheet>::Get(
              s.sprite_sheet_name);
          if (sheet && sheet->texture()) {
            cmd.texture_id = sheet->texture()->renderer_id();
            sheet->GetUVs(s.sprite_index, &cmd.uv_min, &cmd.uv_max);
          }
        } else if (!s.texture_name.empty()) {
          auto tex = util::AssetManager<graphics::Texture>::Get(s.texture_name);
          if (tex) {
            cmd.texture_id = tex->renderer_id();
          }
        }

        utils::RenderQueue::Default().Submit(cmd);
      });

  // Render Text
  registry->ForEach<engine::ecs::components::Transform,
                    engine::ecs::components::Text>(
      [](engine::ecs::components::Transform& t,
         engine::ecs::components::Text& tx) {
        graphics::Renderer::Get().DrawText(tx.font_name, tx.content, t.position,
                                           t.rotation, tx.scale, tx.color,
                                           tx.z_index);
      });
}

}  // namespace engine::graphics::ecs
