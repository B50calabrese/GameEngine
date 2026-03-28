/**
 * @file ui_systems.h
 * @brief Systems for managing UI components.
 */

#ifndef INCLUDE_ENGINE_UI_UI_SYSTEMS_H_
#define INCLUDE_ENGINE_UI_UI_SYSTEMS_H_

#include <engine/ecs/registry.h>
#include <engine/graphics/camera.h>
#include <engine/graphics/utils/render_queue.h>

namespace engine::ui {

/**
 * @brief Handles hierarchical layout propagation and anchoring.
 */
class UILayoutSystem {
 public:
  static void Update(ecs::Registry& reg, int window_width, int window_height);

 private:
  static void UpdateBranch(ecs::Registry& reg, ecs::EntityID entity,
                           const glm::vec2& parent_global_pos,
                           const glm::vec2& parent_size);
};

/**
 * @brief Handles reactive data binding for UI elements.
 */
class UISyncSystem {
 public:
  static void Update(ecs::Registry& reg);
};

/**
 * @brief Handles mouse interactions with UI elements.
 */
class UIInputSystem {
 public:
  static void Update(ecs::Registry& reg);
};

/**
 * @brief Renders UI elements in screen-space.
 */
class UIRenderSystem {
 public:
  UIRenderSystem();
  void Render(ecs::Registry& reg, int window_width, int window_height);

 private:
  std::unique_ptr<graphics::Camera> ui_camera_;
  graphics::utils::RenderQueue ui_render_queue_;
};

}  // namespace engine::ui

#endif  // INCLUDE_ENGINE_UI_UI_SYSTEMS_H_
