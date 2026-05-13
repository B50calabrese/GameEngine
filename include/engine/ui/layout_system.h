/**
 * @file layout_system.h
 * @brief System for managing UI layout.
 */

#ifndef INCLUDE_ENGINE_UI_LAYOUT_SYSTEM_H_
#define INCLUDE_ENGINE_UI_LAYOUT_SYSTEM_H_

#include <glm/glm.hpp>

#include <engine/ecs/entity_manager.h>
#include <engine/ecs/registry.h>

namespace engine::ui {

/**
 * @brief Handles hierarchical layout propagation and anchoring.
 */
class UiLayoutSystem {
 public:
  static void Update(ecs::Registry& reg, int window_width, int window_height);

 private:
  static void UpdateBranch(ecs::Registry& reg, ecs::EntityID entity,
                           const glm::vec2& parent_global_pos,
                           const glm::vec2& parent_size);
};

}  // namespace engine::ui

#endif  // INCLUDE_ENGINE_UI_LAYOUT_SYSTEM_H_
