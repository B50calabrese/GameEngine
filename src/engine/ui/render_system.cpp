/**
 * @file render_system.cpp
 * @brief Implementation of the UI render system.
 */

#include <engine/ui/render_system.h>

#include <engine/core/window.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/sprite.h>
#include <engine/ecs/components/text.h>
#include <engine/ecs/components/ui_transform.h>
#include <engine/graphics/primitive_renderer.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/texture.h>
#include <engine/util/asset_manager.h>

namespace engine::ui {

using namespace engine::ecs::components;

UiRenderSystem::UiRenderSystem() {
  ui_camera_ = std::make_unique<graphics::Camera>(0, 1, 0, 1);
}

void UiRenderSystem::Render(ecs::Registry& reg, int window_width,
                            int window_height) {
  ui_camera_->set_projection(0, (float)window_width, 0, (float)window_height);
  ui_render_queue_.Clear();

  graphics::PrimitiveRenderer::StartBatch(ui_camera_->view_projection_matrix());

  auto view = reg.GetView<UiTransform>();
  for (auto entity : view) {
    if (!reg.IsAlive(entity) || !reg.HasComponent<UiTransform>(entity))
      continue;
    auto& transform = reg.GetComponent<UiTransform>(entity);

    if (reg.HasComponent<Text>(entity)) {
      auto& text = reg.GetComponent<Text>(entity);
      graphics::Renderer::Get().DrawText(text.font_name, text.content,
                                         transform.global_pos, 0.0f, text.scale,
                                         text.color);
    }

    if (reg.HasComponent<Quad>(entity)) {
      auto& quad = reg.GetComponent<Quad>(entity);
      graphics::utils::RenderCommand cmd;
      cmd.position = transform.global_pos;
      cmd.size = transform.size;
      cmd.color = quad.color;
      cmd.z_order = (float)transform.z_index;
      ui_render_queue_.Submit(cmd);
    }

    if (reg.HasComponent<Sprite>(entity)) {
      auto& sprite = reg.GetComponent<Sprite>(entity);
      graphics::utils::RenderCommand cmd;
      cmd.position = transform.global_pos;
      cmd.size = transform.size;
      cmd.color = sprite.tint;
      cmd.z_order = (float)transform.z_index;

      if (!sprite.sprite_sheet_name.empty()) {
        auto sheet = util::AssetManager<graphics::SpriteSheet>::Get(
            sprite.sprite_sheet_name);
        if (sheet && sheet->texture()) {
          cmd.texture_id = sheet->texture()->renderer_id();
          sheet->GetUVs(sprite.sprite_index, &cmd.uv_min, &cmd.uv_max);
        }
      } else if (!sprite.texture_name.empty()) {
        auto tex =
            util::AssetManager<graphics::Texture>::Get(sprite.texture_name);
        if (tex) {
          cmd.texture_id = tex->renderer_id();
        }
      }

      ui_render_queue_.Submit(cmd);
    }
  }

  ui_render_queue_.Flush();
  graphics::PrimitiveRenderer::FinalizeBatch();
  graphics::PrimitiveRenderer::RenderBatch();
}

}  // namespace engine::ui
