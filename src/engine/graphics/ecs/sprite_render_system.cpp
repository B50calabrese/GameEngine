/**
 * @file sprite_render_system.cpp
 * @brief Implementation of the sprite render system.
 */

#include <engine/ecs/components/circle.h>
#include <engine/ecs/components/line.h>
#include <engine/ecs/components/point.h>
#include <engine/ecs/components/polygon.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/sprite.h>
#include <engine/ecs/components/text.h>
#include <engine/ecs/components/transform.h>
#include <engine/ecs/components/triangle.h>
#include <engine/graphics/ecs/sprite_render_system.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/texture.h>
#include <engine/graphics/utils/render_queue.h>
#include <engine/util/asset_manager.h>

namespace engine::graphics::ecs {

void SpriteRenderSystem::Render(engine::ecs::Registry* registry) {
  if (!registry) {
    return;
  }
  // 1. Render entities with Transform and Sprite/Quad
  auto trans_view = registry->GetView<engine::ecs::components::Transform>();
  for (auto entity : trans_view) {
    auto& transform =
        registry->GetComponent<engine::ecs::components::Transform>(entity);

    // Sprite Component (takes priority)
    if (registry->HasComponent<engine::ecs::components::Sprite>(entity)) {
      auto& sprite =
          registry->GetComponent<engine::ecs::components::Sprite>(entity);
      if (sprite.visible) {
        if (!sprite.sprite_sheet_name.empty()) {
          auto sheet =
              util::AssetManager<SpriteSheet>::Get(sprite.sprite_sheet_name);
          if (sheet && sheet->texture()) {
            glm::vec2 uv_min, uv_max;
            sheet->GetUVs(sprite.sprite_index, &uv_min, &uv_max);

            utils::RenderCommand cmd;
            cmd.z_order = sprite.z_index;
            cmd.texture_id = sheet->texture()->renderer_id();
            cmd.position = transform.position;
            cmd.size = transform.scale;
            cmd.rotation = transform.rotation;
            cmd.color = sprite.tint;
            cmd.uv_min = uv_min;
            cmd.uv_max = uv_max;
            cmd.origin = sprite.origin;
            utils::RenderQueue::Default().Submit(cmd);
          }
        } else if (!sprite.texture_name.empty()) {
          auto tex = util::AssetManager<Texture>::Get(sprite.texture_name);
          if (tex) {
            utils::RenderCommand cmd;
            cmd.z_order = sprite.z_index;
            cmd.texture_id = tex->renderer_id();
            cmd.position = transform.position;
            cmd.size = transform.scale;
            cmd.rotation = transform.rotation;
            cmd.color = sprite.tint;
            cmd.uv_min = {0.0f, 0.0f};
            cmd.uv_max = {1.0f, 1.0f};
            cmd.origin = sprite.origin;
            utils::RenderQueue::Default().Submit(cmd);
          }
        }
      }
    }
    // Quad Component (fallback)
    else if (registry->HasComponent<engine::ecs::components::Quad>(entity)) {
      auto& quad =
          registry->GetComponent<engine::ecs::components::Quad>(entity);
      utils::RenderCommand cmd;
      cmd.z_order = quad.z_index;
      cmd.texture_id = 0;  // White texture slot
      cmd.position = transform.position;
      cmd.size = transform.scale;
      cmd.rotation = transform.rotation;
      cmd.color = quad.color;
      cmd.origin = quad.origin;
      cmd.shape_type = utils::ShapeType::kQuad;
      utils::RenderQueue::Default().Submit(cmd);
    }
    // Circle Component
    else if (registry->HasComponent<engine::ecs::components::Circle>(entity)) {
      auto& circle =
          registry->GetComponent<engine::ecs::components::Circle>(entity);
      utils::RenderCommand cmd;
      cmd.z_order = circle.z_index;
      cmd.shape_type = utils::ShapeType::kCircle;
      cmd.position = transform.position;
      cmd.size = {circle.radius, circle.radius};
      cmd.color = circle.color;
      cmd.thickness = circle.thickness;
      cmd.color2 = circle.color2;
      cmd.gradient_type = circle.gradient_type;
      utils::RenderQueue::Default().Submit(cmd);
    }
    // Triangle Component
    else if (registry->HasComponent<engine::ecs::components::Triangle>(entity)) {
      auto& triangle =
          registry->GetComponent<engine::ecs::components::Triangle>(entity);
      utils::RenderCommand cmd;
      cmd.z_order = triangle.z_index;
      cmd.shape_type = utils::ShapeType::kTriangle;
      cmd.position = transform.position;
      cmd.size = triangle.size * transform.scale;
      cmd.color = triangle.color;
      cmd.rotation = transform.rotation;
      cmd.origin = triangle.origin;
      cmd.thickness = triangle.thickness;
      cmd.color2 = triangle.color2;
      cmd.gradient_type = triangle.gradient_type;
      utils::RenderQueue::Default().Submit(cmd);
    }
    // Line Component
    else if (registry->HasComponent<engine::ecs::components::Line>(entity)) {
      auto& line =
          registry->GetComponent<engine::ecs::components::Line>(entity);
      utils::RenderCommand cmd;
      cmd.z_order = line.z_index;
      cmd.shape_type = utils::ShapeType::kLine;
      cmd.position = transform.position;
      cmd.size = line.dest;
      cmd.color = line.color;
      cmd.thickness = line.thickness;
      cmd.is_dashed = line.is_dashed;
      utils::RenderQueue::Default().Submit(cmd);
    }
    // Point Component
    else if (registry->HasComponent<engine::ecs::components::Point>(entity)) {
      auto& point =
          registry->GetComponent<engine::ecs::components::Point>(entity);
      utils::RenderCommand cmd;
      cmd.z_order = point.z_index;
      cmd.shape_type = utils::ShapeType::kPoint;
      cmd.position = transform.position;
      cmd.color = point.color;
      cmd.thickness = point.size;
      utils::RenderQueue::Default().Submit(cmd);
    }
    // Polygon Component
    else if (registry->HasComponent<engine::ecs::components::Polygon>(entity)) {
      auto& polygon =
          registry->GetComponent<engine::ecs::components::Polygon>(entity);
      utils::RenderCommand cmd;
      cmd.z_order = polygon.z_index;
      cmd.shape_type = utils::ShapeType::kPolygon;
      cmd.color = polygon.color;
      cmd.polygon_vertices = polygon.vertices;
      // Transform polygon vertices to world space
      for (auto& v : cmd.polygon_vertices) {
        v += transform.position;
      }
      utils::RenderQueue::Default().Submit(cmd);
    }

    // Text Component (can be combined with Sprite/Quad)
    if (registry->HasComponent<engine::ecs::components::Text>(entity)) {
      auto& text =
          registry->GetComponent<engine::ecs::components::Text>(entity);
      Renderer::Get().DrawText(text.font_name, text.content, transform.position,
                               transform.rotation, text.scale, text.color,
                               text.z_index);
    }
  }
}

}  // namespace engine::graphics::ecs
