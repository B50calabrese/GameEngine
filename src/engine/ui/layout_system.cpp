/**
 * @file layout_system.cpp
 * @brief Implementation of the UI layout system.
 */

#include <engine/ui/layout_system.h>

#include <algorithm>
#include <vector>

#include <engine/ecs/components/ui_hierarchy.h>
#include <engine/ecs/components/ui_transform.h>

namespace engine::ui {

using namespace engine::ecs::components;

void UiLayoutSystem::Update(ecs::Registry& reg, int window_width,
                            int window_height) {
  auto roots = reg.GetView<UiTransform, UiHierarchy>().Filter(
      [&](auto& t, auto& h) { return h.parent == ecs::kInvalidEntity; });

  // If no roots with UIHierarchy, try entities with just UiTransform but no
  // UIHierarchy
  auto all_transforms = reg.GetView<UiTransform>();
  std::vector<ecs::EntityID> all_roots = roots;
  for (auto entity : all_transforms) {
    bool has_hierarchy = reg.HasComponent<UiHierarchy>(entity);
    if (!has_hierarchy ||
        reg.GetComponent<UiHierarchy>(entity).parent == ecs::kInvalidEntity) {
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
}

void UiLayoutSystem::UpdateBranch(ecs::Registry& reg, ecs::EntityID entity,
                                  const glm::vec2& parent_global_pos,
                                  const glm::vec2& parent_size) {
  if (!reg.HasComponent<UiTransform>(entity)) {
    return;
  }
  auto& ui = reg.GetComponent<UiTransform>(entity);

  // Calculate anchor position
  glm::vec2 anchor_pos = parent_global_pos + ui.anchor_min * parent_size;

  // Global position is anchor + local offset
  ui.global_pos = anchor_pos + ui.local_pos;

  ui.is_dirty = false;

  if (reg.HasComponent<UiHierarchy>(entity)) {
    auto& hierarchy = reg.GetComponent<UiHierarchy>(entity);
    for (auto child : hierarchy.children) {
      if (reg.HasComponent<UiTransform>(child)) {
        UpdateBranch(reg, child, ui.global_pos, ui.size);
      }
    }
  }
}

}  // namespace engine::ui
