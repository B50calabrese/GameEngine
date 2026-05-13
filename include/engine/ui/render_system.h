/**
 * @file render_system.h
 * @brief System for rendering UI elements.
 */

#ifndef INCLUDE_ENGINE_UI_RENDER_SYSTEM_H_
#define INCLUDE_ENGINE_UI_RENDER_SYSTEM_H_

#include <memory>

#include <engine/ecs/registry.h>
#include <engine/graphics/camera.h>
#include <engine/graphics/utils/render_queue.h>

namespace engine::ui {

/**
 * @brief Renders UI elements in screen-space.
 */
class UiRenderSystem {
 public:
  UiRenderSystem();
  void Render(ecs::Registry& reg, int window_width, int window_height);

 private:
  std::unique_ptr<graphics::Camera> ui_camera_;
  graphics::utils::RenderQueue ui_render_queue_;
};

}  // namespace engine::ui

#endif  // INCLUDE_ENGINE_UI_RENDER_SYSTEM_H_
