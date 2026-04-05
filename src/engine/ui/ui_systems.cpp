/**
 * @file ui_systems.cpp
 * @brief Implementation of UI systems.
 */

#include <engine/ecs/components/light.h>
#include <engine/ecs/components/occluder.h>
#include <engine/ecs/components/quad.h>
#include <engine/ecs/components/sprite.h>
#include <engine/ecs/components/text.h>
#include <engine/graphics/primitive_renderer.h>
#include <engine/graphics/renderer.h>
#include <engine/graphics/sprite_sheet.h>
#include <engine/graphics/texture.h>
#include <engine/input/input_manager.h>
#include <engine/ui/ui_components.h>
#include <engine/ui/ui_systems.h>
#include <engine/util/asset_manager.h>

namespace engine::ui {

void UILayoutSystem::Update(ecs::Registry& reg, int window_width,
                            int window_height) {
  auto roots = reg.GetView<UITransform, UIHierarchy>().Filter(
      [&](auto& t, auto& h) { return h.parent == ecs::INVALID_ENTITY; });

  // If no roots with UIHierarchy, try entities with just UITransform but no
  // UIHierarchy
  auto all_transforms = reg.GetView<UITransform>();
  std::vector<ecs::EntityID> all_roots = roots;
  for (auto entity : all_transforms) {
    bool has_hierarchy = reg.HasComponent<UIHierarchy>(entity);
    if (!has_hierarchy ||
        reg.GetComponent<UIHierarchy>(entity).parent == ecs::INVALID_ENTITY) {
      // Check if already in all_roots
      if (std::find(all_roots.begin(), all_roots.end(), entity) ==
          all_roots.end()) {
        all_roots.push_back(entity);
      }
    }
  }

  glm::vec2 screen_size(window_width, window_height);

  for (auto entity : all_roots) {
    UpdateBranch(reg, entity, glm::vec2(0.0f, 0.0f), screen_size);
  }

  // Mark all as dirty if root moved
  // (This is a simplified layout system)
}

void UILayoutSystem::UpdateBranch(ecs::Registry& reg, ecs::EntityID entity,
                                  const glm::vec2& parent_global_pos,
                                  const glm::vec2& parent_size) {
  if (!reg.HasComponent<UITransform>(entity)) {
    return;
  }
  auto& ui = reg.GetComponent<UITransform>(entity);

  // Calculate anchor position
  glm::vec2 anchor_pos = parent_global_pos + ui.anchor_min * parent_size;

  // Global position is anchor + local offset
  ui.global_pos = anchor_pos + ui.local_pos;

  // Clamp to window bounds for safety
  // ui.global_pos = glm::clamp(ui.global_pos, glm::vec2(0.0f),
  // glm::vec2(800.0f, 600.0f));
  ui.is_dirty = false;

  if (reg.HasComponent<UIHierarchy>(entity)) {
    auto& hierarchy = reg.GetComponent<UIHierarchy>(entity);
    for (auto child : hierarchy.children) {
      if (reg.HasComponent<UITransform>(child)) {
        UpdateBranch(reg, child, ui.global_pos, ui.size);
      }
    }
  }
}

void UISyncSystem::Update(ecs::Registry& reg) {
  auto view = reg.GetView<UIBinding, engine::ecs::components::Text>();
  for (auto entity : view) {
    if (!reg.IsAlive(entity) || !reg.HasComponent<UIBinding>(entity) ||
        !reg.HasComponent<engine::ecs::components::Text>(entity))
      continue;
    auto& binding = reg.GetComponent<UIBinding>(entity);
    auto& text = reg.GetComponent<engine::ecs::components::Text>(entity);
    if (binding.get_text) {
      std::string current = binding.get_text();
      if (current != binding.last_value) {
        text.content = current;
        binding.last_value = current;
      }
    }
  }
}

void UIInputSystem::Update(ecs::Registry& reg) {
  auto& input = InputManager::Get();
  glm::vec2 mouse_pos = input.mouse_screen_pos();
  bool mouse_pressed = input.IsKeyDown(KeyCode::kMouseLeft);

  auto view = reg.GetView<UITransform, UIInteractable>();
  for (auto entity : view) {
    if (!reg.IsAlive(entity) || !reg.HasComponent<UITransform>(entity) ||
        !reg.HasComponent<UIInteractable>(entity))
      continue;
    auto& transform = reg.GetComponent<UITransform>(entity);
    auto& interactable = reg.GetComponent<UIInteractable>(entity);

    bool was_hovered = interactable.is_hovered;

    interactable.is_hovered =
        mouse_pos.x >= transform.global_pos.x &&
        mouse_pos.x <= transform.global_pos.x + transform.size.x &&
        mouse_pos.y >= transform.global_pos.y &&
        mouse_pos.y <= transform.global_pos.y + transform.size.y;

    if (interactable.is_hovered != was_hovered) {
      if (interactable.on_hover_changed) {
        interactable.on_hover_changed(interactable.is_hovered);
      }
    }

    if (interactable.is_hovered) {
      input.Consume();

      if (mouse_pressed && !interactable.is_pressed) {
        interactable.is_pressed = true;
      } else if (!mouse_pressed && interactable.is_pressed) {
        interactable.is_pressed = false;
        if (interactable.on_click) {
          interactable.on_click();
        }
      }
    } else {
      interactable.is_pressed = false;
    }
  }
}

UIRenderSystem::UIRenderSystem() {
  ui_camera_ = std::make_unique<graphics::Camera>(0, 800, 0, 600);
}

void UIRenderSystem::Render(ecs::Registry& reg, int window_width,
                            int window_height) {
  ui_camera_->set_projection(0, (float)window_width, 0, (float)window_height);
  ui_render_queue_.Clear();

  graphics::PrimitiveRenderer::StartBatch(ui_camera_->view_projection_matrix());

  auto view = reg.GetView<UITransform>();
  for (auto entity : view) {
    if (!reg.IsAlive(entity) || !reg.HasComponent<UITransform>(entity))
      continue;
    auto& transform = reg.GetComponent<UITransform>(entity);

    if (reg.HasComponent<engine::ecs::components::Text>(entity)) {
      auto& text = reg.GetComponent<engine::ecs::components::Text>(entity);
      graphics::Renderer::Get().DrawText(text.font_name, text.content,
                                         transform.global_pos, 0.0f, text.scale,
                                         text.color);
    }

    if (reg.HasComponent<engine::ecs::components::Quad>(entity)) {
      auto& quad = reg.GetComponent<engine::ecs::components::Quad>(entity);
      graphics::utils::RenderCommand cmd;
      cmd.position = transform.global_pos;
      cmd.size = transform.size;
      cmd.color = quad.color;
      cmd.z_order = (float)transform.z_index;
      ui_render_queue_.Submit(cmd);
    }

    if (reg.HasComponent<engine::ecs::components::Sprite>(entity)) {
      auto& sprite = reg.GetComponent<engine::ecs::components::Sprite>(entity);
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
